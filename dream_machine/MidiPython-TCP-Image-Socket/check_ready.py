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
        print(public_ip, open_port)
        break
    except Exception as e:
        print("Error on line {}".format(sys.exc_info()[-1].tb_lineno))
        print("Server not ready yet")
        continue

    if port != "None" or ssh_address !="None": break

# print(ssh_address, port)

# subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080", "-N"])

while True:
    try:
        #out = os.system("scp -o StrictHostKeyChecking=no -P {} {}:/workspace/vast_ai/dream_machine/READY.log READY.log".format(port, ssh_address))
        out = os.system(f"scp -o StrictHostKeyChecking=no -P {open_port} root@{public_ip}:/workspace/vast_ai/dream_machine/READY.log READY.log")
        if out != 0:
            continue
        else:
            break
        time.sleep(1)

    except KeyboardInterrupt:
        print("KeyboardInterrupt")
        break

with open("ssh_pipe.cmd", "w") as f:
    # f.write(f"ssh -p {open_port} root@{public_ip} -L 8080:localhost:8080 -tt")
    # f.write(f"ssh -p {port} {ssh_address} -L 8080:localhost:8080 -N")
    f.write("ssh -p {} {} -L 8080:localhost:8080 -tt".format(open_port, public_ip))