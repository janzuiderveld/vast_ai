#!/bin/bash
function finish {
  pkill -P $$
  echo "killed $$"
  cd $ROOT_DIR
  ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")
  echo "destroying instance $ID"
  ./vast destroy instance $ID
  exit
}
trap finish EXIT
trap finish SIGINT

# make sure there are no more tunnels on port 8080 the machine
lsof -ti:8080 | xargs kill -9

ROOT_DIR=$PWD

cd $ROOT_DIR/dream_machine

python3 -u -m venv $ROOT_DIR/dream_machine/dream_machine_env
source $ROOT_DIR/dream_machine/dream_machine_env/bin/activate

python3 -u -m pip install -r $ROOT_DIR/dream_machine/Python-TCP-Image-Socket/requirements.txt
python3 -u -m pip install requests 
python3 -u -m pip install pillow 

# apt-get update
# apt-get install ffmpeg libsm6 libxext6  -y

# apt install inotify-tools -y
# python3 -u -m pip install pyinotify

# cd $ROOT_DIR/dream_machine
# python3 -u Python-TCP-Image-Socket/client_receive_socket.py 2>&1 | tee _client_receive.log &


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
python3 -u ./MidiPython-TCP-Image-Socket/check_ready.py
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
# python3 -u Python-TCP-Image-Socket/client.py 2>&1 | tee _client_send.log &
python3 -u ./MidiPython-TCP-Image-Socket/client.py

sleep 5

sudo python3 -u ./serial/first.py &
python3 -u ./MidiPython-TCP-Image-Socket/sender_3.4.py





# #!/bin/bash
# python3 -m venv dream_machine/dream_machine_env
# source dream_machine/dream_machine_env/bin/activate

# python3 -m pip install -r dream_machine/Python-TCP-Image-Socket/requirements.txt
# python3 -m pip install requests

# # apt-get update
# # apt-get install ffmpeg libsm6 libxext6  -y

# # apt install inotify-tools -y
# # python3 -m pip install pyinotify

# cd dream_machine
# # python3 Python-TCP-Image-Socket/client_receive_socket.py 2>&1 | tee _client_receive.log &


# # until [ -f /tmp/examplefile.txt ]
# # do
# #      sleep 5
# # done
# # echo "SERVER READY"


# # looks for files to be added to /workspace/vast_ai/dream_machine/Sketch-Simulator/out.log 
# # (when inotify script is running and detecting files in Sketch-Simulator/out/to_send) 
# # files are copied to out_imgs.

# echo "waiting for server to be ready..."

# python3 ./Python-TCP-Image-Socket/check_ready.py

# echo "Server ready"

# # looks for files to appear in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/in_imgs, sends them over tcp
# # python3 Python-TCP-Image-Socket/client.py 2>&1 | tee _client_send.log &
# python3 ./Python-TCP-Image-Socket/client.py 


