import sys
save_stdout = sys.stdout
save_stderr = sys.stderr
import warnings
warnings.filterwarnings("ignore")

print("Running Lakhnes script")
# from collections import defaultdict
import os

import glob
import argparse

#TODO FILTER CORRUPT MIDI FILES

sys.path.extend("/workspace/vast_ai/midialogue/LakhNES/model")
sys.path.extend("/workspace/vast_ai/midialogue/LakhNES")
sys.path.extend("/workspace/vast_ai/midialogue/LakhNES/utils")
import model.mem_transformer

from collections import defaultdict
import tempfile
import pretty_midi

# NOTES
# -answer_add_silence', type=int, default=3*44100) # this is added as wait time to end of input after last midi signal. Might influence output?

# P1 RANGE: 33 TM 108
# P2 RANGE: 33 TM 108
# TR RANGE: 21 TM 108
# NO RANGE: 1 TM 16

code_model_dir = './model'
code_utils_dir = os.path.join(code_model_dir, 'utils')
sys.path.extend([code_model_dir, code_utils_dir])

MODEL_FP = '/workspace/vast_ai/midialogue/LakhNES/model/pretrained/LakhNES/model.pt'
VOCAB_FP = '/workspace/vast_ai/midialogue/LakhNES/model/pretrained/LakhNES/vocab.txt'
USE_CUDA = True

import torch
import torch.nn.functional as F
import numpy as np

sys.stdout = sys.__stdout__

device = torch.device("cuda" if USE_CUDA else "cpu")

# Load the best saved model.
with open(MODEL_FP, 'rb') as f:
    model = torch.load(f)
model.backward_compatible()
model = model.to(device)

# Make sure model uses vanilla softmax.
if model.sample_softmax > 0:
  raise NotImplementedError()
if model.crit.n_clusters != 0:
  raise NotImplementedError()

# Load the vocab.
idx2sym = ['<S>']
with open(VOCAB_FP, 'r') as f:
  for line in f:
    idx2sym.append(line.strip().split(',')[-1])
sym2idx = {s:i for i, s in enumerate(idx2sym)}
wait_amts = set([int(s[3:]) for s in idx2sym if s[:2] == 'WT'])
print(len(idx2sym)) 

TX1_PATH = '/workspace/vast_ai/midialogue/LakhNES/data/nesmdb_tx1/test/*.tx1.txt'

def get_tokens_tx1_paths(tx1):
    TX1_FPS = sorted(glob.glob(tx1))

    fn_to_ids = {}
    for fp in TX1_FPS:
        fn = os.path.split(fp)[1].split('.')[0]
        with open(fp, 'r') as f:
            prime = f.read().strip().splitlines()
        
        if len(prime) > 0 and prime[0][:2] == 'WT':
            prime = prime[1:]
        if len(prime) > 0 and prime[-1][:2] == 'WT':
            prime = prime[:-1]
            
        prime = ['<S>'] + prime + ['<S>']

        prime_ids = []
        for s in prime:
            if s in sym2idx:
                prime_ids.append(sym2idx[s])
            else:
                assert s[:2] == 'WT'
                wait_amt = int(s.split('_')[1])
                closest = min(wait_amts, key=lambda x:abs(x - wait_amt))
                prime_ids.append(sym2idx['WT_{}'.format(closest)])
        
        fn_to_ids[fn] = prime_ids
    return fn_to_ids

def get_tokens_tx1_var(tx1):
    tx1 = tx1.splitlines()
    print(tx1)
    if len(tx1) > 0 and tx1[0][:2] == 'WT':
        tx1 = tx1[1:]
    if len(tx1) > 0 and tx1[-1][:2] == 'WT':
        tx1 = tx1[:-1]
        
    tx1 = ['<S>'] + tx1 + ['<S>']

    tx1_ids = []
    for s in tx1:
        if s in sym2idx:
            tx1_ids.append(sym2idx[s])
        else:
            print(sym2idx)
            print(s)
            assert s[:2] == 'WT'
            wait_amt = int(s.split('_')[1])
            closest = min(wait_amts, key=lambda x:abs(x - wait_amt))
            tx1_ids.append(sym2idx['WT_{}'.format(closest)])
    
    # fn_to_ids[fn] = tx1_ids

    return tx1_ids

# fn_to_ids = get_tokens_tx1_paths(TX1_PATH)
# get_tokens_tx1_var(tx1)
# sym2idx

# import numpy as np
# import torch
# import torch.nn.functional as F

def load_vocab(vocab_fp):
  idx2sym = ['<S>']
  wait_amts = []
  with open(vocab_fp, 'r') as f:
    for line in f:
      idx2sym.append(line.strip().split(',')[-1])
      if line[:2] == 'WT':
        wait_amts.append(int(line[3:]))
  sym2idx = {s:i for i, s in enumerate(idx2sym)}
  return idx2sym, sym2idx, wait_amts


def quantize_wait_event(wait_event):
  wait_time = int(wait_event[3:])
  diff = float('inf')
  candidate = None

  for t in wait_amts:
    cur_diff = abs(wait_time - t)
    if cur_diff < diff:
      diff = cur_diff
      candidate = t
    else:
      break
  return 'WT_{}'.format(candidate)


class TxlSimpleSampler:
  def __init__(self, model, device, tgt_len=1, mem_len=896, ext_len=0):
    if tgt_len != 1:
      raise ValueError()
    if ext_len != 0:
      raise ValueError()
    self.model = model
    self.model.eval()
    self.model.reset_length(1, ext_len, mem_len)
    self.device = device
    self.reset()
    
          
  def reset(self):
    self.mems = []
    self.generated = []
  
  
  @torch.no_grad()
  def sample_next_token_updating_mem(self, last_token=None, temp=1., topk=None, exclude_eos=True):
    last_token = last_token if last_token is not None else 0

    # Ensure that user is always passing 0 on first call
    if len(self.generated) == 0:
      assert len(self.mems) == 0
      if last_token != 0:
        raise Exception()

    # Ensure that user isn't passing 0 after first call
    if last_token == 0 and len(self.generated) > 0:
      raise Exception()

    # Sanitize sampling params
    if temp < 0:
      raise ValueError()
    if topk is not None and topk < 1:
      raise ValueError()
    
    # Append last input token because we've officially selected it
    self.generated.append(last_token)
    
    # Create input array
    _inp = [last_token]
    _inp = np.array(_inp, dtype=np.int64)[:, np.newaxis]
    inp = torch.from_numpy(_inp).to(self.device)

    # Evaluate the model, saving its memory.
    ret = self.model.forward_generate(inp, *self.mems)
    all_logits, self.mems = ret[0], ret[1:]
    
    # Select last timestep, only batch item
    logits = all_logits[-1, 0]
    
    if exclude_eos:
      logits = logits[1:]
    
    # Handle temp 0 (argmax) case
    if temp == 0:
      probs = torch.zeros_like(logits)
      probs[logits.argmax()] = 1.
    else:    
      # Apply temperature spec
      if temp != 1:
        logits /= temp

      # Compute softmax
      probs = F.softmax(logits, dim=-1)
    
    if exclude_eos:
      probs = F.pad(probs, [1, 0])
    
    # Select top-k if specified
    if topk is not None:
      _, top_idx = torch.topk(probs, topk)
      mask = torch.zeros_like(probs)
      mask[top_idx] = 1.
      probs *= mask
      probs /= probs.sum()
    
    # Sample from probabilities
    token = torch.multinomial(probs, 1)
    token = int(token.item())
    
    return token, probs

def TX1_continuation(tx1, temp, topk, fn):
  tokens = get_tokens_tx1_var(tx1)

  print("startup: len tokens: {}".format(len(tokens)))
  if len(tokens) < 4:
      answer_str = '\n'.join(['<S>', 'P1_NOTEON_33' 'WT_8820', 'P1_NOTEOFF', '<S>'])
      return answer_str

  sampler = TxlSimpleSampler(model, device, mem_len=512)

  inp = 0
  nll = 0.
  cont = []
  answer = []
  # for i in range(primelen):

      # tar = prime_ids[i + 1]
  for i in tokens[1:-1]:
      tar = i
      _, probs = sampler.sample_next_token_updating_mem(inp, exclude_eos=False)
      p = probs[tar].cpu().item()
      nll += -np.log(p)
      inp = tar
      cont.append(tar)
  print('Prime PPL: {}'.format(np.exp(nll / len(tokens[1:-1]))))
  # print(tokens)
  nll = 0.
  for i in range(len(tokens[1:-1])):
      print(i)
      gen, probs = sampler.sample_next_token_updating_mem(inp, temp=temp, topk=topk)
      p = probs[gen].cpu().item()
      nll += -np.log(p)
      inp = gen
      cont.append(gen)
      answer.append(gen)
  print('Gen PPL: {}'.format(np.exp(nll / len(tokens[1:-1]))))

  answer_str = '\n'.join(map(lambda x: idx2sym[x], answer))
  # for i in answer:
  #     idx2sym

  return answer_str

def load_midi_fp(fp):
  pretty_midi.pretty_midi.MAX_TICK = 1e16
  with open(fp, "rb") as fh:
      fp = fh.read()

  # Load MIDI file
  with tempfile.NamedTemporaryFile('wb') as mf:
    mf.write(fp)
    mf.seek(0)
    print(mf.name)
    
    print("startup: loading midi")
    midi = pretty_midi.PrettyMIDI(mf.name)

    assert len(midi.instruments) == 4

    print("startup: loading midi done")

    # print(midi.instruments)
    # if len(midi.instruments) > 4:
    #     del midi.instruments[4:]
    # if len(midi.instruments) < 4:
    #     for i in range(4 - len(midi.instruments)):
    #         midi.instruments.append(pretty_midi.Instrument(program=0))

    midi.instruments[0].name = "p1"
    midi.instruments[1].name = "p2"
    midi.instruments[2].name = "tr"
    midi.instruments[3].name = "no"

    print(midi.instruments)



    # instr_map = {}
    # # check the velocity of the notes
    # for i in range(4):
    #     # get velocity of the first note
    #     if len(midi.instruments[i].notes) > 0:
    #         track_no = midi.instruments[i].notes[0].velocity - 101
    #         instr_map[i] = track_no
    #     else:
    #         # TODO make sure this is needed like this. might be bad for generations
    #         # make sure there is a note for every instrument
    #         midi.instruments[i].notes.append(pretty_midi.Note(velocity=100, pitch=60, start=0, end=.5))
    #         instr_map[i] = i
    
    # for i in reversed(range(4)):
    #     midi.instruments[instr_map[i]] = midi.instruments[i]




    # map notes on "no" as follows:
    # 60 > 2
    # 62 > 5
    # 64 > 8
    # 65 > 11
    # 67 > 13
    # 69 > 15

    for note in midi.instruments[3].notes:
        if note.pitch == 60:
            note.pitch = 2
        elif note.pitch == 62:
            note.pitch = 5
        elif note.pitch == 64:
            note.pitch = 8
        elif note.pitch == 65:
            note.pitch = 11
        elif note.pitch == 67:
            note.pitch = 13
        elif note.pitch == 69:
            note.pitch = 15

    return midi

def scale_number(unscaled, to_min, to_max, from_min, from_max):
    return (to_max-to_min)*(unscaled-from_min)/(from_max-from_min)+to_min

def scale_list(l, to_min, to_max):
    return [scale_number(i, to_min, to_max, min(l), max(l)) for i in l]

def midi_to_tx1(fp):
  # pretty_midi.pretty_midi.MAX_TICK = 1e16

  # if "/" in p1:
  #   with open(p1, "rb") as fh:
  #     p1 = fh.read()

  # # if "/" in no:
  # #   with open(no, "rb") as fh:
  # #     no = fh.read()

  #   # Load MIDI file
  #   with tempfile.NamedTemporaryFile('wb') as mf:
  #     mf.write(p1)
  #     mf.seek(0)
  #     midi = pretty_midi.PrettyMIDI(mf.name)
  #     midi.instruments[0].name = "p1"
  #     print(midi.instruments)

  # if no:
  #   midi.instruments.append(no)

  midi = load_midi_fp(fp)

  ins_names = ['p1', 'p2', 'tr', 'no']
  instruments = sorted(midi.instruments, key=lambda x: ins_names.index(x.name))
  samp_to_events = defaultdict(list)
  for ins in instruments:
    instag = ins.name.upper()

    last_start = -1
    last_end = -1
    last_pitch = -1
    for note in ins.notes:
      pitch = note.pitch
      # filter out notes according to range:
      # P1 RANGE: 33 - 108
      # P2 RANGE: 33 - 108
      # TR RANGE: 21 - 108
      # NO RANGE: 1 - 16

      # scale notes between 0 and 127 to 0-16 if not lasers
      if instag == 'NO' and not args.lasers:
          pitch = round(scale_number(pitch, 1, 16, 0, 127))
      
      filtered = False
      if (instag == 'P1' and pitch < 33) or (instag == 'P2' and pitch < 33) or (instag == 'TR' and pitch < 21) or (instag == 'NO' and pitch < 1):
        filtered = True
        continue
      if (instag == 'P1' and pitch > 108) or (instag == 'P2' and pitch > 108) or (instag == 'TR' and pitch > 108) or (instag == 'NO' and pitch > 16):
        filtered = True
        continue
      if filtered: print('warning: filtered out notes out of range')
      

      start = (note.start * 44100) + 1e-6
      end = (note.end * 44100) + 1e-6
      print(instag, note.start, note.end)

      print(start, int(start))
      print(end, int(end))

      # assert start - int(start) < 1e-3
      # assert end - int(end) < 1e-3

      start = int(start)
      end = int(end)

      if not start > last_start:
          continue
      if not start >= last_end:
          continue


      if last_end >= 0 and last_end != start:
        samp_to_events[last_end].append('{}_NOTEOFF'.format(instag))
      samp_to_events[start].append('{}_NOTEON_{}'.format(instag, pitch))

      last_start = start
      last_end = end
      last_pitch = pitch

    if last_pitch != -1:
      samp_to_events[last_end].append('{}_NOTEOFF'.format(instag))

  tx1 = []
  last_samp = 0
  for samp, events in sorted(samp_to_events.items(), key=lambda x: x[0]):
    wt = samp - last_samp
    assert last_samp == 0 or wt > 0
    if wt > 0:
      tx1.append('WT_{}'.format(wt))
    tx1.extend(events)
    last_samp = samp

  # nsamps = int((midi.time_signature_changes[-1].time * 44100) + 1e-6)
  # if nsamps > last_samp:
  tx1.append('WT_{}'.format(args.answer_add_silence))

  tx1 = '\n'.join(tx1)
  return tx1


def tx1_to_midi(tx1, save_folder):
  tx1 = tx1.strip().splitlines()
  nsamps = sum([int(x.split('_')[1]) for x in tx1 if x[:2] == 'WT'])

  # Create MIDI instruments
  p1_prog = pretty_midi.instrument_name_to_program('Lead 1 (square)')
  p2_prog = pretty_midi.instrument_name_to_program('Lead 2 (sawtooth)')
  tr_prog = pretty_midi.instrument_name_to_program('Synth Bass 1')
  no_prog = pretty_midi.instrument_name_to_program('Breath Noise')
  p1 = pretty_midi.Instrument(program=p1_prog, name='p1', is_drum=False)
  p2 = pretty_midi.Instrument(program=p2_prog, name='p2', is_drum=False)
  tr = pretty_midi.Instrument(program=tr_prog, name='tr', is_drum=False)
  no = pretty_midi.Instrument(program=no_prog, name='no', is_drum=True)

  name_to_ins = {'P1': p1, 'P2': p2, 'TR': tr, 'NO': no}
  name_to_pitch = {'P1': None, 'P2': None, 'TR': None, 'NO': None}
  name_to_start = {'P1': None, 'P2': None, 'TR': None, 'NO': None}
  name_to_max_velocity = {'P1': 100, 'P2': 100, 'TR': 100, 'NO': 100}

  samp = 0
  for event in tx1:
    if event[:2] == 'WT':
      samp += int(event[3:])
    else:
      tokens = event.split('_')
      if tokens[0] == '<S>': continue
      name = tokens[0]
      ins = name_to_ins[tokens[0]]

      old_pitch = name_to_pitch[name]
      if tokens[1] == 'NOTEON':
        if old_pitch is not None:

          if tokens[0] == 'NO' and not args.lasers:
            pitch = round(scale_number(old_pitch, 0, 127, 1, 16))

          ins.notes.append(pretty_midi.Note(
              velocity=name_to_max_velocity[name],
              pitch=old_pitch,
              start=name_to_start[name] / 44100.,
              end=samp / 44100.))
        name_to_pitch[name] = int(tokens[2])
        name_to_start[name] = samp
      else:
        if old_pitch is not None:
          pitch = name_to_pitch[name]
          if tokens[0] == 'NO' and not args.lasers:
            pitch = round(scale_number(pitch, 0, 127, 1, 16))

          ins.notes.append(pretty_midi.Note(
              velocity=name_to_max_velocity[name],
              pitch=pitch,
              start=name_to_start[name] / 44100.,
              end=samp / 44100.))

        name_to_pitch[name] = None
        name_to_start[name] = None

  # Deactivating this for generated files
  #for name, pitch in name_to_pitch.items():
  #  assert pitch is None

  # Create MIDI and add instruments
  midi = pretty_midi.PrettyMIDI(initial_tempo=120, resolution=22050)
  midi.instruments.extend([p1, p2, tr, no])
  
  # map notes on "no" as follows:
  # 1, 2, 3 > 60 
  # 4, 5, 6 > 62 
  # 7 , 8 , 9 > 64 
  # 10, 11, 12 > 65 
  # 13, 14 > 67 
  # 15, 16 > 69 

  for note in midi.instruments[3].notes:
    if note.pitch == 1:
      note.pitch = 60
    elif note.pitch == 2:
      note.pitch = 60
    elif note.pitch == 3:
      note.pitch = 60
    elif note.pitch == 4:
      note.pitch = 62
    elif note.pitch == 5:
      note.pitch = 62
    elif note.pitch == 6:
      note.pitch = 62
    elif note.pitch == 7:
      note.pitch = 64
    elif note.pitch == 8:
      note.pitch = 64
    elif note.pitch == 9:
      note.pitch = 64
    elif note.pitch == 10:
      note.pitch = 65
    elif note.pitch == 11:
      note.pitch = 65
    elif note.pitch == 12:
      note.pitch = 65
    elif note.pitch == 13:
      note.pitch = 67
    elif note.pitch == 14:
      note.pitch = 67
    elif note.pitch == 15:
      note.pitch = 69
    elif note.pitch == 16:
      note.pitch = 69

  # Create indicator for end of song
  eos = pretty_midi.TimeSignature(1, 1, nsamps / 44100.)
  midi.time_signature_changes.append(eos)

  with tempfile.NamedTemporaryFile('rb') as mf:
    midi.write(mf.name)
    midi = mf.read()

  filepath = get_incremental_fn(save_folder)
  with open(filepath, 'wb') as f:
    f.write(midi)

  return midi

def get_incremental_fn(folder, fn="midii.mid"):
  i = 0
  new_fp = f"{folder}/{str(i)}_{fn}"
  while os.path.exists(new_fp) :
      i += 1
      new_fp = f"{folder}/{str(i)}_{fn}"
  return new_fp



def midi_continuation(fp, output_folder, fn="test", temp=0.96, topk=64): 
  print("transcribing midi file to txq: ", fp)
  tx1 = midi_to_tx1(fp)
  print("Generating continuation for {}".format(fp))
  tx1_answer = TX1_continuation(tx1, temp, topk, fn)
  print("transcribing tx1 to midi: ")
  midi = tx1_to_midi(tx1_answer, output_folder)

def wait_for_new_midi(midi_folder):
    init_midis = glob.glob(f"{midi_folder}/*.mid") 
    print(f"startup : Waiting for new *mid in {midi_folder}")
    while True:
        current_midis = glob.glob(f"{midi_folder}/*.mid")
        # if 1:
            # new_midi = current_midis[0]
        if len(current_midis) > len(init_midis):
            new_midi = list(set(current_midis).symmetric_difference(set(init_midis)))[0]
            print(f"startup : New midi found: {new_midi}")

            # try:
            #     load_midi_fp(new_midi)
                
            # except Exception as e:
            #     error = ('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            #     filepath = get_incremental_fn(args.midi_out_folder)
            #     os.system(f"echo {error} > {filepath}")
            #     print(f"startup : Failed to load {new_midi}")
            #     return 0
                
            return new_midi


def main(args):
  sys.stdout = sys.__stdout__
  sys.stderr = sys.__stderr__
  os.system("echo READY > /workspace/vast_ai/midialogue/READY.log")
  print("startup: running main")
  while True:
      print("startup: waiting for new midi")
      midi_path = wait_for_new_midi(args.midi_in_folder)
      print(f"startup : new midi found: {midi_path}")
      if midi_path:
        try:
            midi_continuation(midi_path, args.midi_out_folder)
        except Exception as e:
            # filepath = get_incremental_fn(args.midi_out_folder)
            # os.system(f"echo {error} > {filepath}")
            
            filepath = get_incremental_fn(args.midi_out_folder)
            os.system(f"cp {midi_path} {filepath}")

            error = ('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            print(f"startup : Failed to continue new_midi: {error}")
      

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--midi_in_folder', type=str, default='/workspace/vast_ai/midialogue/midi_in')
  parser.add_argument('--midi_out_folder', type=str, default='/workspace/vast_ai/midialogue/midi_out')
  parser.add_argument('--answer_add_silence', type=int, default=0) # this is added as wait time to end of input after last midi signal. Might influence output significantly?
  parser.add_argument('--temp', type=float, default=0.96)
  parser.add_argument('--topk', type=int, default=64)
  parser.add_argument('--lasers', type=int, default=1)
  args = parser.parse_args()

  os.makedirs(args.midi_out_folder, exist_ok=True)
  os.makedirs(args.midi_in_folder, exist_ok=True)

  main(args)
