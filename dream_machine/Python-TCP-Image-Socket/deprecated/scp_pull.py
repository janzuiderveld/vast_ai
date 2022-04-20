import subprocess
import argparse
import inotify.adapters

def main(port_number):
    i = inotify.adapters.Inotify()
    i.add_watch('{ssh_adress}:/workspace/onstart.log')
    # subprocess.call([f'scp -P {port_number} {ssh_adress}:/workspace/onstart.log test.log'], shell=True)

if __name__ == "__main__":
    # argparser
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('port_number', type=str, default='Sketch-Simulator/out/to_send', help='ftp filepath')

    args = parser.parse_args()
    main(args.ftp_filepath)