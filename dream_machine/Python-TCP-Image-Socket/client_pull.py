import glob
import os
import argparse 
import time
def main(args):
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

    old_len = 0
    while True:
        out = os.popen(f"scp -o StrictHostKeyChecking=no -P {port} {ssh_address}:{args.filepath} files.log").read()
        print(out)
        try:
            with open("files.log", "r") as f:
                lines = f.read().splitlines()
                print(lines)
                if len(lines) > old_len:
                    old_len = len(lines)
                    print(lines[-1])
                    latest_file = lines[-1] 
                    if " " in latest_file: continue
                    out = os.popen(f"scp -P {port} '{ssh_address}:{latest_file}' out_imgs/").read()
                    print(out)
        except:
            pass
        time.sleep(1)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='')
    # parser.add_argument('ssh_address', type=str, default='', help='')
    # parser.add_argument('port', type=str, default='', help='')
    parser.add_argument('--filepath', type=str, default='/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log', help='ftp filepath')
    
    os.makedirs("out_imgs", exist_ok=True)
    args = parser.parse_args()
    main(args)


# scp -P 31840 root@ssh5.vast.ai:/workspace/vast_ai/dream_machine/Sketch-Simulator/out.log files.log