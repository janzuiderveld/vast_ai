import glob
import os
import argparse 

def main(args):
    out = os.popen("./vast ssh-url").read().split(":")
    ssh_address = out[1][2:]
    port = out[2]

    print(ssh_address)

    while True:
        old_len = 0
        out = os.popen(f"scp -P {port} {ssh_address}:{args.filepath} files.log").read()
        print(out)
        with open("files.log", "r") as f:
            lines = f.read().splitlines()
            if len(lines) > old_len:
                old_len = len(lines)
                print(lines[-1])

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='')
    # parser.add_argument('ssh_address', type=str, default='', help='')
    # parser.add_argument('port', type=str, default='', help='')
    parser.add_argument('--filepath', type=str, default='/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log', help='ftp filepath')

    args = parser.parse_args()
    main(args)


# scp -P 31054 root@ssh4.vast.ai:/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log files.log