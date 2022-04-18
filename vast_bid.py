import json
import subprocess
import argparse

parser = argparse.ArgumentParser(description='Run a command on a remote host.')
parser.add_argument('bidding_tactic', type=str, default='cheap', choices=['cheap', 'best1', 'auto1'], help='bidding tactic')
# parser.add_argument('min_bid', type=str, default='1')
parser.add_argument('project_name', type=str, default='', help='project name')
parser.add_argument('--dummy', type=int, default=0, help='')
args = parser.parse_args()

# run vast shell command "./vast search offers" to get prices 
if args.bidding_tactic == 'cheap':
    # vast_cmd = './vast search offers -b --raw -o "dph"'
    vast_cmd = './vast search offers -d --raw -o "dph"'
elif args.bidding_tactic == 'best1':
    vast_cmd = './vast search offers -d "num_gpus == 1" --raw -o "dlperf-"'   
elif args.bidding_tactic == 'auto1':
    vast_cmd = './vast search offers -d "num_gpus == 1" --raw'   

vast_output = subprocess.check_output(vast_cmd, shell=True)

# print(vast_output)
top_sellers = json.loads(vast_output)

for i in range(10):
    print(top_sellers[i]["dph_base"],
             top_sellers[i]["min_bid"],
              top_sellers[i]["dlperf"],
               top_sellers[i]["gpu_ram"])

# top_id = top_sellers[0]["id"]
top_id = top_sellers[0]["id"]

# If instance is allowed to be interrupted
if args.bidding_tactic == 'cheap':
    bid = top_sellers[0]["min_bid"]
    vast_book_cmd = f"./vast create instance {top_id} --price {bid} --image pytorch/pytorch --disk 30 --onstart startup_scripts/{args.project_name}.sh"
else:
    vast_book_cmd = f"./vast create instance {top_id} --image pytorch/pytorch --disk 30 --onstart startup_scripts/{args.project_name}.sh"

vast_output = subprocess.check_output(vast_book_cmd, shell=True)
print(vast_output)