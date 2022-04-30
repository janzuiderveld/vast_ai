import os
import socket
import base64
import glob
import sys
import time
import threading
from datetime import datetime
import utils
import argparse
import shutil
import subprocess
# import numpy
# import cv2

class ServerSocket:
    def __init__(self, ip, port, args):
        self.TCP_IP = ip
        self.TCP_PORT = port
        self.input_fp = args.input_fp
        self.socketOpen()
        print("starting receive loop")
        self.receiveFiles()

    def socketClose(self):
        self.sock.close()
        print(u'Server socket [ TCP_IP: ' + self.TCP_IP + ', TCP_PORT: ' + str(self.TCP_PORT) + ' ] is close')

    def socketOpen(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.TCP_IP, self.TCP_PORT))
        self.sock.listen(1)
        print(u'Server socket [ TCP_IP: ' + self.TCP_IP + ', TCP_PORT: ' + str(self.TCP_PORT) + ' ] is open')
        self.conn, self.addr = self.sock.accept()
        print(u'Server socket [ TCP_IP: ' + self.TCP_IP + ', TCP_PORT: ' + str(self.TCP_PORT) + ' ] is connected with client')

    def receiveFiles(self):
        cnt_str = ''
        cnt = 0

        try:
            while True:
                if (cnt < 10):
                    cnt_str = '000' + str(cnt)
                elif (cnt < 100):
                    cnt_str = '00' + str(cnt)
                elif (cnt < 1000):
                    cnt_str = '0' + str(cnt)
                else:
                    cnt_str = str(cnt)
                if cnt == 0: startTime = time.localtime()

                length = self.recvall(self.conn, 64)
                length1 = length.decode('utf-8')
                stringData = self.recvall(self.conn, int(length1))
                data = base64.b64decode(stringData)

                save_path = self.input_fp + "/image_" + cnt_str + ".jpg"

                stime = self.recvall(self.conn, 64)


                print('send time: ' + stime.decode('utf-8'))
                now = time.localtime()
                print('receive time: ' + datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f'))

                save_path = self.input_fp + '/input_' + cnt_str + '.jpg'
                with open(save_path, 'wb') as f:
                    f.write(data)
                cnt += 1


                # data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                # data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                # decimg = cv2.imdecode(data, 1)
                # cv2.imwrite(save_path, decimg)

                # self.socketClose()
                # self.socketOpen()
                
        except Exception as e:
            print('server: Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            self.socketClose()

            self.socketOpen()
            self.receiveThread = threading.Thread(target=self.receiveFiles)
            self.receiveThread.start()


    def recvall(self, sock, count):
        buf = b''
        while count:
            newbuf = sock.recv(count)
            if not newbuf: return None
            buf += newbuf
            count -= len(newbuf)
        return buf    

    def convertfile(self, fnum, count, now):
        img_array = []
        cnt = 0
        for filename in glob.glob('./' + str(self.TCP_PORT) + '_files' + fnum + '/*.jpg'):
            if (cnt == count):
                break
            cnt = cnt + 1
            img = cv2.imread(filename)
            height, width, layers = img.shape
            size = (width, height)
            img_array.append(img)
        
        file_date = self.getDate(now)
        file_time = self.getTime(now)
        name = 'video(' + file_date + ' ' + file_time + ').mp4'
        file_path = './videos/' + name
        out = cv2.VideoWriter(file_path, cv2.VideoWriter_fourcc(*'.mp4'), 20, size)

        for i in range(len(img_array)):
            out.write(img_array[i])
        out.release()
        print(u'complete')

    def getDate(self, now):
        year = str(now.tm_year)
        month = str(now.tm_mon)
        day = str(now.tm_mday)

        if len(month) == 1:
            month = '0' + month
        if len(day) == 1:
            day = '0' + day
        return (year + '-' + month + '-' + day)

    def getTime(self, now):
        file_time = (str(now.tm_hour) + '_' + str(now.tm_min) + '_' + str(now.tm_sec))
        return file_time

def main(args):
    server = ServerSocket(args.TCP_SERVER_IP, args.TCP_SERVER_PORT, args)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='TCP client')
    parser.add_argument('--input_fp', type=str, default='test_input', help='ftp filepath')
    parser.add_argument('--TCP_SERVER_IP', type=str, default='localhost', help='TCP server ip')
    parser.add_argument('--TCP_SERVER_PORT', type=int, default=8080, help='TCP server port')

    args = parser.parse_args()

    # # add environment variable ROOT_DIR to the path
    # args.input_fp = os.path.join(os.environ['ROOT_DIR'], args.input_fp)
    # args.output_fp = os.path.join(os.environ['ROOT_DIR'], args.output_fp)

    os.makedirs(args.input_fp, exist_ok=True)
    main(args)