import os
import subprocess
import time
import json
import sys

while True:
    try:
        out = os.popen("../vast ssh-url").read().split(":")
        if not out[0]: continue
        print(out)
        ssh_address = out[1][2:]
        port = out[2].split("\n")[0]
        out = os.popen("../vast show instances --raw").read()
        out = json.loads(out)[0]
        public_ip = (out["public_ipaddr"])
        open_port = (out["direct_port_start"])
        print(ssh_address, port, public_ip, open_port)
        break
    except Exception as e:
        print(f"Error on line {sys.exc_info()[-1].tb_lineno}")
        print("Server not ready yet")
        continue

    if port != "None" or ssh_address !="None": break

# print(ssh_address, port)

# subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080", "-N"])

while True:
    out = os.system(f"scp -o StrictHostKeyChecking=no -P {port} {ssh_address}:/workspace/vast_ai/midialogue/READY.log READY.log")
    # out = os.system(f"scp -o StrictHostKeyChecking=no -P {open_port} root@{public_ip}:/workspace/vast_ai/midialogue/READY.log READY.log")
    if out != 0:
        continue
    else:
        break
    time.sleep(1)

with open("ssh_pipe.cmd", "w") as f:
    # f.write(f"ssh -p {open_port} root@{public_ip} -L 8080:localhost:8080 -tt")
    # f.write(f"ssh -p {port} {ssh_address} -L 8080:localhost:8080 -N")
    f.write(f"ssh -p {port} {ssh_address} -L 8080:localhost:8080 -tt")