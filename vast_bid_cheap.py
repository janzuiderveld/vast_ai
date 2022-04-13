import json
import subprocess
import argparse

parser = argparse.ArgumentParser(description='Run a command on a remote host.')
parser.add_argument('project_name', type=str, default='', help='project name')
args = parser.parse_args()

# run vast shell command "./vast search offers" to get prices 
vast_cmd = './vast search offers -b --raw -o "dph, dlperf"'
vast_output = subprocess.check_output(vast_cmd, shell=True)

# print(vast_output)
top_sellers = json.loads(vast_output)

for i in range(10):
    print(top_sellers[i]["dph_base"], top_sellers[i]["dlperf"])

top_id = top_sellers[0]["id"]
min_bid = top_sellers[0]["min_bid"]
vast_book_cmd = f"./vast create instance {top_id} --price {min_bid} --image pytorch/pytorch --disk 15 --onstart startup_scripts/{args.project_name}.sh"

print(min_bid)
vast_output = subprocess.check_output(vast_book_cmd, shell=True)
print(vast_output)