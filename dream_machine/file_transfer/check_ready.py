import os
import subprocess
import time
import json
import sys

while True:
    try:
        # this will only complete until instance has launched
        out = os.popen("../vast show instances --raw").read()
        out = json.loads(out)[0]
        public_ip = (out["public_ipaddr"])
        open_port = (out["ports"]["22/tcp"][0]["HostPort"])
        inst_ID = out["id"]
        break
    except Exception as e:
        print("Error on line {}".format(sys.exc_info()[-1].tb_lineno))
        print("Server not ready yet")
        continue

    if port != "None" or ssh_address !="None": break

# save public_ip and open_port to environment variables
os.environ["PUBLIC_IP"] = public_ip
os.environ["OPEN_PORT"] = open_port

while True:
    try:
        # out = os.system(f"scp -o StrictHostKeyChecking=no -P {open_port} root@{public_ip}:/workspace/vast_ai/dream_machine/READY.log READY.log")
        out = os.system(f"../vast copy {inst_ID}:/workspace/vast_ai/dream_machine/READY.log READY.log")
        if out != 0:
            continue
        else:
            break
        time.sleep(1)

    except KeyboardInterrupt:
        print("KeyboardInterrupt")
        break