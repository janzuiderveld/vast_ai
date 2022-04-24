import socket
import numpy
import time
import base64
import sys
from datetime import datetime
import utils
import argparse
import os

class ClientSocket:
    def __init__(self, ip, port, input_fp):
        self.TCP_SERVER_IP = ip
        self.TCP_SERVER_PORT = port
        self.input_fp = args.input_fp
        self.output_fp = args.output_fp
        self.connectCount = 0
        self.connectServer()

    def connectServer(self):
        try:
            self.sock = socket.socket()
            self.sock.connect((self.TCP_SERVER_IP, self.TCP_SERVER_PORT))
            print(u'Client socket is connected with Server socket [ TCP_SERVER_IP: ' + self.TCP_SERVER_IP + ', TCP_SERVER_PORT: ' + str(self.TCP_SERVER_PORT) + ' ]')
            self.connectCount = 0
            self.sendfiles()
        except Exception as e:
            print(e)
    
            import subprocess

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

            pipe_proc = subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080", "-N"])

            self.connectCount += 1
            if self.connectCount == 10:
                print(u'Connect fail %d times. exit program'%(self.connectCount))
                sys.exit()
            print(u'%d times try to connect with server'%(self.connectCount))
            self.connectServer()

    def sendfile(self, file, client):
        try:
            fd = open(file, 'rb')
        except:
            print('Can not open specific file for sending')
            return 0


        resp = client.recv(128)

        # other side is ready, give them the info
        if resp == '[ack]'.encode():
            print("recived ack")
            buf = fd.read(128)
            print(buf)
            while buf:
                #client.send(buf)
                client.send(buf)
                buf = fd.read(128)
            fd.close()
            client.send('[done]'.encode())
            print("send done")
            return 1
        else:
            fd.close()
            return 0

    def recvfile(self, file, client):
            try:
                fd = open(file, 'wb+')
            except:
                print('Can not open specific file for receiving')
                return 0

            # ready give me the info
            client.send('[ack]'.encode())
            print("send [ack]")
            #buf = client.recv(128)
            buf = client.recv(128)
            while buf:
                print(buf)
                if buf != '[done][ack]'.encode():
                    fd.write(buf)
                    buf = client.recv(128)
                else:
                    print("received [done]")
                    fd.close()
                    return 1
            return 0

    def sendfiles(self):
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

                time.sleep(3)
                self.filepath = "/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_in/1_midi.mid"
                # self.filepath = utils.wait_new_file(self.input_fp)
                print(f"Sending {self.filepath}")
                self.sendfile(self.filepath, self.sock)
                print(u'send files %d'%(cnt))

                print(u'Waiting for response')
                self.recvfile(f"{self.output_fp}/{cnt_str}.mid", self.sock)
                print(f'received files {self.output_fp}/{cnt_str}.mid')
                # response = self.sock.recv(128)
                # length = self.recvall(128)
                # length1 = length.decode('utf-8')
                # stringData = self.recvall(int(length1))

                # data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                # decimg = cv2.imdecode(data, 1)

                # cv2.imwrite(self.output_fp + "/output_" + cnt_str + ".jpg" , decimg)
                # end = time.time()
                cnt+=1

                print("server took %f seconds"%(end-start))

            except Exception as e:
                print('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
                self.sock.close()
                time.sleep(1)
                self.connectServer()
                self.sendfiles()

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

    args = parser.parse_args()
    os.makedirs(args.input_fp, exist_ok=True)
    os.makedirs(args.output_fp, exist_ok=True)
    main(args)


# write a txt file
# with open('/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/out_imgs/output_' + cnt_str + '.txt', 'w') as f: