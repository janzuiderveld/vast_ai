import socket
import cv2
import numpy
import time
import base64
import sys
from datetime import datetime
import utils
import argparse

class ClientSocket:
    def __init__(self, ip, port, ftp_filepath):
        self.TCP_SERVER_IP = ip
        self.TCP_SERVER_PORT = port
        self.ftp_filepath = ftp_filepath
        self.connectCount = 0
        self.filepath = utils.wait_new_file(self.ftp_filepath)
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
            self.connectCount += 1
            if self.connectCount == 10:
                print(u'Connect fail %d times. exit program'%(self.connectCount))
                sys.exit()
            print(u'%d times try to connect with server'%(self.connectCount))
            self.connectServer()

    def sendImages(self):
        cnt = 0
        while True:
            try:
                frame = cv2.imread(self.filepath)
                # frame = cv2.imread("/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/test.png")
                resize_frame = cv2.resize(frame, dsize=(256, 256), interpolation=cv2.INTER_AREA)

                now = time.localtime()
                stime = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')
                    
                encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                result, imgencode = cv2.imencode('.jpg', resize_frame, encode_param)
                data = numpy.array(imgencode)
                stringData = base64.b64encode(data)
                length = str(len(stringData))
                self.sock.sendall(length.encode('utf-8').ljust(64))
                self.sock.send(stringData)
                self.sock.send(stime.encode('utf-8').ljust(64))
                print(u'send images %d'%(cnt))
                cnt+=1
                time.sleep(0.095)
            except Exception as e:
                print(e)
                self.sock.close()
                time.sleep(1)
                self.connectServer()
                self.sendImages()

            self.filepath = utils.wait_new_file(self.ftp_filepath)

def main(ftp_filepath):
    TCP_IP = 'localhost' 
    TCP_PORT = 8080 
    client = ClientSocket(TCP_IP, TCP_PORT, ftp_filepath)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='TCP client')
    parser.add_argument('--ftp_filepath', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine', help='ftp filepath')

    args = parser.parse_args()
    main(args.ftp_filepath)