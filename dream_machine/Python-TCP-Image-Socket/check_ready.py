import os
import subprocess

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

subprocess.Popen(['ssh', f"-o StrictHostKeyChecking=no", f"-p {port}", f"{ssh_address}", "-L 8080:localhost:8080"])

while True:
    out = os.system(f"scp -o StrictHostKeyChecking=no -P {port} {ssh_address}:/workspace/vast_ai/dream_machine/READY.log READY.log")
    if out != 0:
        continue
    else:
        break