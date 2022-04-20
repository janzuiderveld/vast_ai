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

class ServerSocket:
    def __init__(self, ip, port):
        self.TCP_IP = ip
        self.TCP_PORT = port
        self.createImageDir()
        self.folder_num = 0
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
                cnt += 1

                length = self.recvall(self.conn, 64)
                length1 = length.decode('utf-8')
                stringData = self.recvall(self.conn, int(length1))
                stime = self.recvall(self.conn, 64)

                print('send time: ' + stime.decode('utf-8'))
                now = time.localtime()
                print('receive time: ' + datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f'))
                data = numpy.frombuffer(base64.b64decode(stringData), numpy.uint8)
                decimg = cv2.imdecode(data, 1)
                # cv2.imshow("image", decimg)
                # cv2.imwrite('./' + str(self.TCP_PORT) + '_images' + str(self.folder_num) + '/img' + cnt_str + '.jpg', decimg)
                

                save_path = './' + str(self.TCP_PORT) + '_images0' + '/img' + cnt_str + '.jpg'
                cv2.imwrite(save_path, decimg)
                

                ######################

                # header = ("HTTP/1.1 200 OK\r\n"
                #         "Accept-Ranges: bytes\r\n"
                #         f"Content-Length: {length1}\r\n"
                #         "Keep-Alive: timeout=10, max=100\r\n"
                #         "Connection: Keep-Alive\r\n (or Connection: close)"
                #         "Content-Type: image/png; charset=ISO-8859-1\r\n"
                #         # "Content-Type: image/jpeg; charset=ISO-8859-1\r\n"
                #         "\r\n")
                # print("length: " + length1)
                # self.conn.send(header)

                resize_frame = cv2.resize(decimg, dsize=(256, 256), interpolation=cv2.INTER_AREA)
                encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
                result, imgencode = cv2.imencode('.jpg', resize_frame, encode_param)
                
                data = numpy.array(imgencode)
                stringData = base64.b64encode(data)
                length = str(len(stringData))

                self.conn.sendall(length.encode('utf-8').ljust(64))
                self.conn.send(stringData)
                self.conn.send(stime.encode('utf-8').ljust(64))

                print(u'responded image)
                # with open(save_path, 'rb') as f:
                #     while True:
                #         data = f.read(1024)
                #         if not data:
                #             break
                #         self.conn.send(data)

                self.socketClose()
                self.socketOpen()
                
        except Exception as e:
            print(e)

            print("whyu?")
            # self.convertImage(str(self.folder_num), cnt, startTime)
            self.socketClose()

            self.socketOpen()
            self.receiveThread = threading.Thread(target=self.receiveImages)
            self.receiveThread.start()

    def createImageDir(self):

        folder_name = str(self.TCP_PORT) + "_images0"
        try:
            if not os.path.exists(folder_name):
                os.makedirs(os.path.join(folder_name))
        except OSError as e:
            if e.errno != errno.EEXIST:
                print("Failed to create " + folder_name +  " directory")
                raise

        folder_name = str(self.TCP_PORT) + "_images1"
        try:
            if not os.path.exists(folder_name):
                os.makedirs(os.path.join(folder_name))
        except OSError as e:
            if e.errno != errno.EEXIST:
                print("Failed to create " + folder_name + " directory")
                raise

        folder_name = "videos"
        try:
            if not os.path.exists(folder_name):
                os.makedirs(os.path.join(folder_name))
        except OSError as e:
            if e.errno != errno.EEXIST:
                print("Failed to create " + folder_name + " directory")
                raise
        
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

def main():
    server = ServerSocket('localhost', 8080)

if __name__ == "__main__":
    main()