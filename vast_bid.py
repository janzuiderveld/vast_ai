import json
import subprocess
import argparse

#TODO set 3090 mode.

parser = argparse.ArgumentParser(description='Run a command on a remote host.')
parser.add_argument('bidding_tactic', type=str, default='cheap', choices=['cheap', 'cheap8', 'cheap4', 'best1', 'auto1', 'auto1_bid', 'cheap_od'], help='bidding tactic')
# parser.add_argument('min_bid', type=str, default='1')
parser.add_argument('project_name', type=str, default='', help='project name')
parser.add_argument('--dummy', type=int, default=0, help='')
parser.add_argument('--direct', type=int, default=1, choices=[1, 0], help='direct connection, use true when doing file transfers')
args = parser.parse_args()

if args.project_name == "midialogue":
    # image = "pytorch/pytorch:1.0.1-cuda10.0-cudnn7-devel"
    image = "pytorch/pytorch:1.10.0-cuda11.3-cudnn8-runtime"
else:
    image = "pytorch/pytorch"

# run vast shell command "./vast search offers" to get prices 
if args.bidding_tactic == 'cheap':
    vast_cmd = './vast search offers -b "direct_port_count>1" --raw -o "dph"'
if args.bidding_tactic == 'cheap_od':
    vast_cmd = './vast search offers -d "direct_port_count>1" --raw -o "dph"'

if args.bidding_tactic == 'cheap4':
    vast_cmd = './vast search offers -b "direct_port_count>1 num_gpus == 4" --raw -o "dph"' 
if args.bidding_tactic == 'cheap8':
    vast_cmd = './vast search offers -b "direct_port_count>1 num_gpus == 8" --raw -o "dph"' 
elif args.bidding_tactic == 'auto1_bid':
    vast_cmd = './vast search offers -b "direct_port_count>1 num_gpus == 1 gpu_ram > 20" --raw'   
elif args.bidding_tactic == 'auto1':
    vast_cmd = './vast search offers -d "direct_port_count>1 num_gpus == 1 gpu_ram > 20" --raw'   
elif args.bidding_tactic == 'best1':
    vast_cmd = './vast search offers -d "direct_port_count>1 num_gpus == 1 gpu_ram > 20" --raw -o "dlperf-"'   

# open api key from /home/p/keys/vast_api_key.txt
api_key = "export VAST_API_KEY=$(cat /home/p/keys/vast_api_key.txt)"
subprocess.check_output(api_key, shell=True)

vast_output = subprocess.check_output(vast_cmd, shell=True)

# print(vast_output)
top_sellers = json.loads(vast_output.decode("utf-8"))

for i in range(1):
    print(top_sellers[i]["dph_base"],
             top_sellers[i]["min_bid"],
              top_sellers[i]["dlperf"],
               top_sellers[i]["gpu_ram"],
                top_sellers[i]["direct_port_count"])

# top_id = top_sellers[0]["id"]
top_id = top_sellers[0]["id"]

# If instance is allowed to be interrupted
if args.bidding_tactic == 'cheap' or args.bidding_tactic == "cheap8" or args.bidding_tactic == "cheap4" or args.bidding_tactic == "auto1_bid": 
    bid = top_sellers[0]["min_bid"]
    # vast_book_cmd = f"./vast create instance {top_id} --price {bid} --image {image} --disk 30 --onstart startup_scripts/{args.project_name}.sh"
    vast_book_cmd = f"./vast create instance " + str(top_id) + " --price " + str(bid) + f" --image {image} --disk 100 --ssh --direct --onstart startup_scripts/" + args.project_name + ".sh"
else:
    # vast_book_cmd = f"./vast create instance {top_id} --image {image} --disk 30 --onstart startup_scripts/{args.project_name}.sh"
    vast_book_cmd = f"./vast create instance " + str(top_id) + f" --image {image} --disk 100 --ssh --direct --onstart startup_scripts/" + args.project_name + ".sh"

# if args.direct:
# #     vast_book_cmd += " --direct"
#     import os, json
#     out = os.popen("../vast show instances --raw").read()
#     out = json.loads(out)[0]
#     public_ip = (out["public_ipaddr"])
#     open_port = (out["machine_dir_ssh_port"])
#     print(public_ip, open_port)

vast_output = subprocess.check_output(vast_book_cmd, shell=True)

print(vast_output)
print(top_sellers[0])
