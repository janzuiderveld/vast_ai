import os
import socket
import cv2
import numpy
import base64
import glob
import sys
import time
import threading
from datetime import datetime
import utils
import argparse
import shutil

class ServerSocket:
    def __init__(self, ip, port, args):
        self.TCP_IP = ip
        self.TCP_PORT = port
        self.input_fp = args.input_fp
        self.output_fp = args.output_fp
        self.esrgan = args.esrgan
        self.socketOpen()
        self.receiveThread = threading.Thread(target=self.receiveImages)
        self.receiveThread.start()

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

    def receiveImages(self):
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
                stime = self.recvall(self.conn, 64)

                print('send time: ' + stime.decode('utf-8'))
                now = time.localtime()
                print('receive time: ' + datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f'))
                data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                decimg = cv2.imdecode(data, 1)

                save_path = self.input_fp + '/input_' + cnt_str + '.jpg'
                cv2.imwrite(save_path, decimg)
                
                resp_file = utils.wait_new_file(self.output_fp)
                
                # sleep to avoid corrupted file? (libpng error: Read Error)
                time.sleep(1)

                if self.esrgan:
                    tmp_folder = "/workspace/vast_ai/dream_machine/temp_upscale"
                    os.makedirs(tmp_folder, exist_ok=True)
                    os.system(f"python3 ../Real-ESRGAN/inference_realesrgan.py -n RealESRGAN_x4plus -i {resp_file} -o tmp_folder --outscale 4 --half")
                    out_fp = glob.glob(tmp_folder + "/*")[0]
                else:
                    out_fp = resp_file

                frame = cv2.imread(out_fp)
                resize_frame = cv2.resize(frame, dsize=(1024, 1024), interpolation=cv2.INTER_AREA)
                encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                result, imgencode = cv2.imencode('.jpg', resize_frame, encode_param)
                
                data = numpy.array(imgencode)
                stringData = base64.b64encode(data)
                length = str(len(stringData))

                self.conn.sendall(length.encode('utf-8').ljust(64))
                self.conn.send(stringData)

                if self.esrgan:
                    shutil.rmtree(tmp_folder)

                print('responded image')

                cnt += 1

                # self.socketClose()
                # self.socketOpen()
                
        except Exception as e:
            print('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            self.socketClose()

            self.socketOpen()
            self.receiveThread = threading.Thread(target=self.receiveImages)
            self.receiveThread.start()


    def recvall(self, sock, count):
        buf = b''
        while count:
            newbuf = sock.recv(count)
            if not newbuf: return None
            buf += newbuf
            count -= len(newbuf)
        return buf    

    def convertImage(self, fnum, count, now):
        img_array = []
        cnt = 0
        for filename in glob.glob('./' + str(self.TCP_PORT) + '_images' + fnum + '/*.jpg'):
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
    server = ServerSocket('localhost', 8080, args)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='TCP client')
    parser.add_argument('--input_fp', type=str, default='/workspace/vast_ai/dream_machine/incoming_imgs', help='ftp filepath')
    parser.add_argument('--output_fp', type=str, default='/workspace/vast_ai/dream_machine/Sketch-Simulator/out/to_send', help='ftp filepath')
    parser.add_argument('--esrgan', type=int, default=1, help='ftp filepath')

    args = parser.parse_args()
    os.makedirs(args.input_fp, exist_ok=True)
    os.makedirs(args.output_fp, exist_ok=True)
    main(args)