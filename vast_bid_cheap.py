import json
import subprocess

# run vast shell command "./vast search offers" to get prices 
vast_cmd = './vast search offers -b --raw -o "dph, dlperf"'
vast_output = subprocess.check_output(vast_cmd, shell=True)

# print(vast_output)
top_sellers = json.loads(vast_output)

for i in range(10):
    print(top_sellers[i]["dph_base"], top_sellers[i]["dlperf"])

top_id = top_sellers[0]["id"]
min_bid = top_sellers[0]["min_bid"]
vast_book_cmd = f"./vast create instance {top_id} --price {min_bid} --image pytorch/pytorch --disk 5 --onstart 'startup_general.sh'"

print(min_bid)
vast_output = subprocess.check_output(vast_book_cmd, shell=True)
print(vast_output)