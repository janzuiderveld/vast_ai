import socket
import numpy
import time
import base64
import sys
from datetime import datetime
import utils
import argparse
import os
import subprocess
import pretty_midi
import tempfile

#TODO FILTER CORRUPT MIDI FILES: MAYBE FIXED 
# normalizesmf <filename>  ?

#TODO bij minder input kanalen nogg steeds mappen naar goede kanaal > zet ghost notes bij begin openames >  sendmidi
# Check

#TODO 2 kanalen gewshcaald voor synth en model > NOTEMAP
# ROUTEMIDI? 
# Usage: sendmidi --out <port> ( --note-off <channel> <note> <velocity> | --note-on <channel> <note> <velocity> | --key-pressure <channel> <note> <amount> | --control-change <channel> <number> <value> | --program-change <channel> <number> | --channel-pressure <channel> <amount> | --pitch-wheel <channel> <amount> | --panic )
# notemap --in <port> --out <port> [ --transpose <n> ] [ --map <filename.xml> ]

# TODO ERROR:
# Generating continuation for /workspace/vast_ai/midialogue/midi_in/midi_0022.mid │·················································································································
# ['WT_8820']                                                                     │·················································································································
# startup: len tokens: 2                                                          │·················································································································
# transcribing tx1 to midi:                                                       │·················································································································
# sh: 1: Syntax error: word unexpected (expecting ")")                            │·················································································································
# Traceback (most recent call last):                                              │·················································································································
#   File "startup.py", line 533, in main                                          │·················································································································
#     midi_continuation(midi_path, args.midi_out_folder)                          │·················································································································
#   File "startup.py", line 498, in midi_continuation                             │·················································································································
#     midi = tx1_to_midi(tx1_answer, output_folder)                               │·················································································································
#   File "startup.py", line 447, in tx1_to_midi                                   │·················································································································
#     name_to_pitch[name] = int(tokens[2])                                        │·················································································································
# ValueError: invalid literal for int() with base 10: '33WT'   

# TODO sometime..
# NETMIDIC AND NETMIDID
# These utilities speak NetMIDI, a trivial network protocol I created which sends standard MIDI messages over a TCP/IP connection as fast as possible. They can be used to connect up the MIDI systems on two different machines over a network connection, even if they are running different operating systems. The client forwards messages from the local MIDI system to a NetMIDI server. The server forwards messages sent by the client to the local MIDI system.




class ClientSocket:
    def __init__(self, ip, port, input_fp):
        self.TCP_SERVER_IP = ip
        self.TCP_SERVER_PORT = port
        self.input_fp = args.input_fp
        self.output_fp = args.output_fp
        self.dummy = args.dummy
        self.connectCount = 0
        # self.establish_ssh()
        self.connectServer()

    def establish_ssh(self):
        while True:
            try:
                out = os.popen("../vast ssh-url").read().split(":")
                if not out[0]: continue
                print(out)
                ssh_address = out[1][2:]
                port = out[2].split("\n")[0]
            except:
                continue

            if port != "None" or ssh_address !="None": break

        print(ssh_address, port)

        # terminate pipe_proc if it exists
        # if 'self.pipe_proc' in locals():
        #     self.pipe_proc.terminate()

        # self.pipe_proc = subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080", "-N"])

        self.connectCount += 1
        if self.connectCount == 10:
            print(u'Connect fail %d times. exit program'%(self.connectCount))
            sys.exit()
        print(u'%d times try to connect with server'%(self.connectCount))
    
        # time.sleep(5)

    def connectServer(self):
        try:
            self.sock = socket.socket()
            self.sock.connect((self.TCP_SERVER_IP, self.TCP_SERVER_PORT))
            print(u'Client socket is connected with Server socket [ TCP_SERVER_IP: ' + self.TCP_SERVER_IP + ', TCP_SERVER_PORT: ' + str(self.TCP_SERVER_PORT) + ' ]')
            self.connectCount = 0
            self.sendImages()
        except Exception as e:
            print(e)
            # self.establish_ssh()


            # while True:
            #     try:
            #         out = os.popen("../vast ssh-url").read().split(":")
            #         if not out[0]: continue
            #         print(out)
            #         ssh_address = out[1][2:]
            #         port = out[2].split("\n")[0]
            #     except:
            #         continue

            #     if port != "None" or ssh_address !="None": break

            # print(ssh_address, port)

            # pipe_proc = subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080", "-N"])

            # self.connectCount += 1
            # if self.connectCount == 10:
            #     print(u'Connect fail %d times. exit program'%(self.connectCount))
            #     sys.exit()
            # print(u'%d times try to connect with server'%(self.connectCount))
            
            time.sleep(5)
            self.connectServer()


    def sendImages(self):
        cnt = 0
        while True:
            try:
                if (cnt < 10):
                    cnt_str = '000' + str(cnt)
                elif (cnt < 100):
                    cnt_str = '00' + str(cnt)
                elif (cnt < 1000):
                    cnt_str = '0' + str(cnt)
                else:
                    cnt_str = str(cnt)

                if self.dummy:
                    time.sleep(2)
                    self.filepath = "/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/Python-TCP-Image-Socket/client.py"
                else:
                    self.filepath = utils.wait_new_file(self.input_fp)

                    # while True:
                    #     try:
                    #         print("trying to load midi file")
                    #         # Load MIDI file
                    #         with open(self.filepath, "rb") as fh:
                    #             data = fh.read()
                    #         with tempfile.NamedTemporaryFile('wb') as mf:
                    #             mf.write(data)
                    #             mf.seek(0)
                    #             midi_data = pretty_midi.PrettyMIDI(mf.name)
                    #             # count notes
                    #             note_count = midi_data.get_piano_roll().sum(axis=0)
                    #             if note_count: break
                    #     except Exception as e:
                    #         print(f"error on line {sys.exc_info()[-1].tb_lineno}: {e}")
                    #         print(u'%s is not a valid midi file'%(self.filepath))
                    #         break
                    #     time.sleep(0.1)

                # frame = cv2.imread(self.filepath)
                # frame = cv2.imread("/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/test.png")
                # resize_frame = cv2.resize(frame, dsize=(256, 256), interpolation=cv2.INTER_AREA)

                start = time.time()
                stime = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')
                    
                # encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                # result, imgencode = cv2.imencode('.jpg', resize_frame, encode_param)
                # data = numpy.array(imgencode)
                with open(self.filepath, "rb") as fp:
                    data = fp.read()

                stringData = base64.b64encode(data)
                length = str(len(stringData))
                self.sock.sendall(length.encode('utf-8').ljust(64))
                self.sock.send(stringData)
                self.sock.send(stime.encode('utf-8').ljust(64))
                print(u'send images %d'%(cnt))

                # response = self.sock.recv(64)
                length = self.recvall(64)
                while True:
                    try:
                        length1 = length.decode('utf-8')
                        break
                    except:
                        print(u'length decode error')
                        continue
                stringData = self.recvall(int(length1))

                data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                save_path = f"{self.output_fp}/midi_{cnt_str}.mid"
                with open(save_path, "wb") as fp:
                    fp.write(data)
                
                # decimg = cv2.imdecode(data, 1)

                # cv2.imwrite(self.output_fp + "/output_" + cnt_str + ".jpg" , decimg)
                end = time.time()
                cnt+=1

                print("server took %f seconds"%(end-start))

            except Exception as e:
                print('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
                self.sock.close()
                time.sleep(1)
                self.connectServer()
                self.sendImages()

    def recvall(self, count):
        buf = b''
        while count:
            newbuf = self.sock.recv(count)
            if not newbuf: return None
            buf += newbuf
            count -= len(newbuf)
        return buf    

def main(args):
    TCP_IP = 'localhost' 
    TCP_PORT = 8080 
    client = ClientSocket(TCP_IP, TCP_PORT, args)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='TCP client')
    parser.add_argument('--input_fp', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_in', help='ftp filepath')
    parser.add_argument('--output_fp', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_out', help='ftp filepath')
    parser.add_argument('--dummy', type=int, default=0, help='ftp filepath')

    args = parser.parse_args()
    os.makedirs(args.input_fp, exist_ok=True)
    os.makedirs(args.output_fp, exist_ok=True)
    main(args)


# write a txt file
# with open('/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/out_imgs/output_' + cnt_str + '.txt', 'w') as f: