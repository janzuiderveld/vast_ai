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
import tempfile
import cv2
from PIL import Image
import shutil
import traceback
import json

class ClientSocket:
    def __init__(self, input_fp):
        out = os.popen("../vast show instances --raw").read()
        out = json.loads(out)[0]
        
        self.inst_ID = out["id"]
        self.TCP_SERVER_IP = (out["public_ipaddr"])
        self.TCP_SERVER_PORT = int(out["ports"]["22/tcp"][0]["HostPort"])

        self.input_fp = args.input_fp
        self.output_fp = args.output_fp
        self.push_fp = args.push_fp
        self.pull_fp = args.pull_fp

        self.dummy = args.dummy

        # dimensions of input image reshape
        self.x_size, self.y_size = 565, 400

        self.start()

    def start(self):
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
                    self.filepath = "/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/test.png"
                else:
                    tif_file = utils.wait_new_file(self.input_fp)

                    # convert .TIF file to .jpg
                    while True:
                        try:
                            img = Image.open(tif_file)
                            self.filepath = tif_file.replace('.TIF', '.jpg')
                            img.save(self.filepath)
                            break
                        except Exception as e:
                            print("server comm: failed to convert .TIF file to .jpg file, trying again")
                            print(e)

                start = time.time()
                stime = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')

                # cv2 ops
                frame = cv2.imread(self.filepath)
                frame = cv2.rotate(frame, cv2.ROTATE_90_COUNTERCLOCKWISE)
                resize_frame = cv2.resize(frame, dsize=(self.x_size, self.y_size), interpolation=cv2.INTER_AREA)
                # save image
                cv2.imwrite(self.filepath, resize_frame)

                # send image
                while True:
                    try:
                        out = os.system(f"../vast copy {self.filepath} {self.inst_ID}:{self.push_fp}/{cnt_str}.jpg")
                        if out == 0:
                            break
                    except Exception as e:
                        traceback.print_exc()
                        print("server comm: failed to read image file, trying again")
                        print(e)

                # try to pull image from server
                while True:
                    try:
                        # Rsync, is a bit slower in retrying..
                        # out = subprocess.check_output([f"../vast", "copy", f"{self.inst_ID}:{self.pull_fp}/{cnt_str}.jpg", f"{self.output_fp}/{cnt_str}.jpg"])   
                        # if "0 files to consider" in out.decode("utf-8"):
                        #     print("server comm: file not found, trying again")
                        # else:
                        #     break

                        out = os.system(f"scp -o StrictHostKeyChecking=no -P {self.TCP_SERVER_PORT} root@{self.TCP_SERVER_IP}:{self.pull_fp}/{cnt_str}.jpg {self.output_fp}/{cnt_str}.jpg")
                        if out == 0:
                            break
                    except Exception as e:
                        traceback.print_exc()
                        print("server comm: failed to read image file, trying again")
                        print(e)
                
                end = time.time()
                cnt+=1

                print("server comm: server took %f seconds"%(end-start))

            except Exception as e:
                print('server comm: Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
                traceback.print_exc()


def main(args):
    client = ClientSocket(args)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='TCP client')

    # check exported DEVICE variable
    import os
    if os.environ.get('DEVICE') == 'Mac':
        parser.add_argument('--input_fp', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/in_imgs', help='ftp filepath')
        parser.add_argument('--output_fp', type=str, default='/Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/out_imgs', help='ftp filepath')

    else:
        parser.add_argument('--input_fp', type=str, default='/home/pi/FTP/test', help='ftp filepath')
        parser.add_argument('--output_fp', type=str, default='/home/pi/vast_ai/dream_machine/out_imgs', help='ftp filepath')

    parser.add_argument('--push_fp', type=str, default='/workspace/vast_ai/dream_machine/incoming_imgs', help='ftp filepath')
    parser.add_argument('--pull_fp', type=str, default='/workspace/vast_ai/dream_machine/Sketch-Simulator/out/to_send', help='ftp filepath')

    parser.add_argument('--dummy', type=int, default=0, help='ftp filepath')

    args = parser.parse_args()

    # if os.path.exists(args.input_fp) and os.path.isdir(args.input_fp):
    #     shutil.rmtree(args.input_fp)
    if os.path.exists(args.output_fp) and os.path.isdir(args.output_fp):
        shutil.rmtree(args.output_fp)

    os.makedirs(args.input_fp, exist_ok=True)
    os.makedirs(args.output_fp, exist_ok=True)
    while True:
        try:
            main(args)
        except Exception as err:
            print(err)
            print(traceback.format_exc())
            continue
