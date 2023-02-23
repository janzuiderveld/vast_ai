#!/bin/bash
function finish {
  python3 midialogue/midi_scripts/note_off_all.py 

  pkill -P $$
  echo "killed $$"
  cd $ROOT_DIR
  ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")
  echo "destroying instance $ID"
  ./vast destroy instance $ID

  exit
}
trap finish EXIT
trap finish SIGINT

timeout=1

#TODO
# cut beginning midis that are too long
# remove 1st hit on all instruments ?

# make drums repeat on hold
# make sure wave guide send is set on boot

# autoreboot if loading takes too long?
#  1 make bash script reboot on crash
#  2 make loading script check if it is still running after 5 minutes and send reboot command otherwise


# seems te be ok:
# cut final silence of midi recording 

# make sure there are no more tunnels on port 8080 the machine
lsof -ti:8080 | xargs kill -9

cd vast_ai

ROOT_DIR=$PWD

echo "ROOT_DIR: $ROOT_DIR"

echo "waiting for server to start up (3 minutes)"
# sleep 180

# kill light pattern PIDs
kill $(ps aux | grep 'light_pattern' | awk '{print $2}')

python3 midialogue/reset_usb.py search Teensy

echo ""
echo "Midi Utilities: midi ins" 
$ROOT_DIR/midialogue/midi-utilities/bin/lsmidiins

midi_devices=$($ROOT_DIR/midialogue/midi-utilities/bin/lsmidiins)

blo="Blofeld:Blofeld Blofeld"
esi1="ESI MIDIMATE eX:ESI MIDIMATE eX MIDI 1"
esi2="ESI MIDIMATE eX:ESI MIDIMATE eX MIDI 2"
tee="Teensy MIDI:Teensy MIDI MIDI 1"
model_in_p="Midi Through:Midi Through Port-0"


echo ""
echo $midi_devices
echo ""

blo_index=${midi_devices%%$blo*}
esi1_index=${midi_devices%%$esi1*}
esi2_index=${midi_devices%%$esi2*}
tee_index=${midi_devices%%$tee*}
model_index=${midi_devices%%$model_in_p*}

blo_index=${#blo_index}
esi1_index=${#esi1_index}
esi2_index=${#esi2_index}
tee_index=${#tee_index}
model_index=${#model_index}

let blo_index=$blo_index-2
let esi1_index=$esi1_index-2
let esi2_index=$esi2_index-2
let tee_index=$tee_index-2
let model_index=$model_index-2

blo_port=${midi_devices:$blo_index:2}
in_port=${midi_devices:$tee_index:2}
es1_port=${midi_devices:$esi1_index:2}
es2_port=${midi_devices:$esi2_index:2}
model_port=${midi_devices:$model_index:2}

all_synths_port=4
model_and_synths_port=5

echo "in_port: $in_port"
echo "blo_port: $blo_port"
echo "es1_port: $es1_port"
echo "es2_port: $es2_port"
echo "model_port: $model_port"
echo "all_synths_port: $all_synths_port"
echo "model_and_synths_port: $model_and_synths_port"

in_port=$in_port | xargs echo
blo_port=$blo_port | xargs echo
es1_port=$es1_port | xargs echo
es2_port=$es2_port | xargs echo
model_port=$model_port | xargs echo
all_synths_port=$all_synths_port | xargs echo
model_and_synths_port=$model_and_synths_port | xargs echo

export in_port
export blo_port
export es1_port
export es2_port
export model_port
export all_synths_port
export model_and_synths_port

# WORKING 
# # bus 4: placeh0lder
# echo "bus 4: placeh0lder"
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --virtual-in 4 --virtual-out 4 &
# sleep 1

# # bus 5: placeh0lder2
# echo "bus 5: placeh0lder2"
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --virtual-in 5 --virtual-out 5 &
# sleep 1

# bus 6: to all synths 
echo "bus $all_synths_port: to all synths"
# save in variable
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-in $all_synths_port --out $es1_port --out $es2_port &
sleep 1

# bus 7: input to model and all synths
echo "bus $model_and_synths_port: to model and all synths"
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --in $in_port --out $model_port --out $all_synths_port &
sleep 1

# echo "bus $model_and_synths_port: to model and all synths"
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --in $in_port --out $model_and_synths_port &
# input_PID=$!
# export input_PID
# sleep 1

# send a midi note 10 to teensy to make sure midi is send
echo "send a midi note 10 to teensy to make sure midi is send"
$ROOT_DIR/midialogue/midi-utilities/bin/sendmidi --out $in_port --note-on 1 10 1

cd $ROOT_DIR/midialogue
sudo rm -rf midi_in midi_out

# sleep 999999
# python3 $ROOT_DIR/midialogue/midi_scripts/brainstorm_custom.py &

echo "waiting for server to be ready..."
kill -9 $(lsof -t -i:8080)
python3 ./Python-TCP-Image-Socket/check_ready.py
sleep 2
kill -9 $(lsof -t -i:8080)
sleep 2
echo $(lsof -i:8080)
cmd=`cat ssh_pipe.cmd` 
$cmd > test_ssh.thrash &
sleep 3
echo $(lsof -i:8080)
echo "Server ready"

cd $ROOT_DIR
ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")

export ID
export ROOT_DIR

mkdir $ROOT_DIR/midialogue/midi_in
mkdir $ROOT_DIR/midialogue/midi_out

python3 $ROOT_DIR/midialogue/midi_scripts/brainstorm_custom.py &
echo "model listens on virtual port 0"

python3 ./midialogue/midi_autoplay.py --midi_send_port $all_synths_port --midi_in_port $in_port | tee autoplay.log