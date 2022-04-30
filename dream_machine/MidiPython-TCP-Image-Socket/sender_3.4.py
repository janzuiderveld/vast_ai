import socket
import time
import base64
import sys
from datetime import datetime
import utils
import argparse
import os
import subprocess
import tempfile
# import numpy
# import cv2 


class ClientSocket:
    def __init__(self, ip, port, input_fp):
        self.TCP_SERVER_IP = ip
        self.TCP_SERVER_PORT = port
        self.input_fp = args.input_fp
        self.dummy = args.dummy
        self.connectCount = 0
        self.connectServer()


    def connectServer(self):
        try:
            self.sock = socket.socket()
            self.sock.connect((self.TCP_SERVER_IP, self.TCP_SERVER_PORT))
            print(u'Client socket is connected with Server socket [ TCP_SERVER_IP: ' + self.TCP_SERVER_IP + ', TCP_SERVER_PORT: ' + str(self.TCP_SERVER_PORT) + ' ]')
            self.connectCount = 0
            self.sendImages()
        except Exception as e:
            print(e)
            
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
                    self.filepath = self.dummy
                else:
                    self.filepath = utils.wait_new_file(self.input_fp)

                start = time.time()
                stime = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')
                    
                # frame = cv2.imread(self.filepath)
                # resize_frame = cv2.resize(frame, dsize=(256, 256), interpolation=cv2.INTER_AREA)
                # encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                # result, imgencode = cv2.imencode('.jpg', resize_frame, encode_param)
                # data = numpy.array(imgencode)
                # stringData = base64.b64encode(data)
                
                with open(self.filepath, "rb") as imageFile:
                    stringData = base64.b64encode(imageFile.read())

                length = str(len(stringData))
                self.sock.sendall(length.encode('utf-8').ljust(64))
                self.sock.send(stringData)
                self.sock.send(stime.encode('utf-8').ljust(64))
                print(u'send images %d'%(cnt))

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
    TCP_IP = args.TCP_SERVER_IP
    TCP_PORT = args.TCP_SERVER_PORT
    client = ClientSocket(TCP_IP, TCP_PORT, args)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='TCP client')
    parser.add_argument('--input_fp', type=str, default='test_send', help='ftp filepath')
    parser.add_argument('--TCP_SERVER_IP', type=str, default='localhost', help='TCP server ip')
    parser.add_argument('--TCP_SERVER_PORT', type=int, default=8080, help='TCP server port')
    # parser.add_argument('--dummy', type=str, default="", help='')
    parser.add_argument('--dummy', type=str, default="/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/in_imgs/test.png", help='')

    args = parser.parse_args()
    os.makedirs(args.input_fp, exist_ok=True)
    main(args)

