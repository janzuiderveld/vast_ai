#!/bin/bash
python3 -m venv midialogue/midialogue_env
source midialogue/midialogue_env/bin/activate

python3 -m pip install -r midialogue/Python-TCP-Image-Socket/requirements.txt
python3 -m pip install requests

# apt-get update
# apt-get install ffmpeg libsm6 libxext6  -y

# apt install inotify-tools -y
# python3 -m pip install pyinotify

cd midialogue
# python3 Python-TCP-Image-Socket/client_receive_socket.py 2>&1 | tee _client_receive.log &


# until [ -f /tmp/examplefile.txt ]
# do
#      sleep 5
# done
# echo "SERVER READY"


# looks for files to be added to /workspace/vast_ai/dream_machine/Sketch-Simulator/out.log 
# (when inotify script is running and detecting files in Sketch-Simulator/out/to_send) 
# files are copied to out_imgs.

echo "waiting for server to be ready..."
kill -9 $(lsof -t -i:8080)
python3 ./Python-TCP-Image-Socket/check_ready.py
sleep 2
kill -9 $(lsof -t -i:8080)
sleep 2

echo $(lsof -i:8080)

cmd=`cat ssh_pipe.cmd` 
$cmd > test_ssh.thrash &

sleep 3

echo $(lsof -i:8080)
echo "Server ready"

# looks for files to appear in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/in_imgs, sends them over tcp
# python3 Python-TCP-Image-Socket/client.py 2>&1 | tee _client_send.log &
python3 ./Python-TCP-Image-Socket/client.py --dummy 1 2>&1 | tee _client_send.log &

