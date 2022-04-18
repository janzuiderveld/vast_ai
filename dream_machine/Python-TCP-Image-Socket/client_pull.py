import glob
import os
import argparse 
import time
def main(args):
    while True:
        out = os.popen("../vast ssh-url").read().split(":")
        ssh_address = out[1][2:]
        port = out[2].split("\n")[0]

        if port != "None" or ssh_address !="None": break

    print(ssh_address, port)

    while True:
        old_len = 0
        out = os.popen(f"scp -P {port} {ssh_address}:{args.filepath} files.log").read()
        print(out)
        with open("files.log", "r") as f:
            lines = f.read().splitlines()
            if len(lines) > old_len:
                old_len = len(lines)
                print(lines[-1])
                latest_file = lines[-1] 
                out = os.popen(f"scp -P {port} {ssh_address}:{latest_file} out_imgs/").read()
                print(out)
        time.sleep(1)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='')
    # parser.add_argument('ssh_address', type=str, default='', help='')
    # parser.add_argument('port', type=str, default='', help='')
    parser.add_argument('--filepath', type=str, default='/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log', help='ftp filepath')

    args = parser.parse_args()
    main(args)


# scp -P 31840 root@ssh5.vast.ai:/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log files.log