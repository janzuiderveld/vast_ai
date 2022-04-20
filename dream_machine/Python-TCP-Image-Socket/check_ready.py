    import os
    
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
        out = os.popen(f"scp -o StrictHostKeyChecking=no -P {port} {ssh_address}:/workspace/vast_ai/dream_machine/READY.log READY.log").read()
        print(out)