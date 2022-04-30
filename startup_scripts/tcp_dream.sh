#!/bin/sh
function finish {
  pkill -P $$
  echo "killed $$"
  exit
}
trap finish EXIT
trap finish SIGINT

# make sure there are no more tunnels on port 8080 the machine
lsof -ti:8080 | xargs kill -9

source ~/.bashrc

cd /workspace
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git
cd /workspace/vast_ai/dream_machine

apt-get update
apt-get install ffmpeg libsm6 libxext6 -y

ROOT_DIR=$PWD
export ROOT_DIR

python3 -m pip install -r Python-TCP-Image-Socket/requirements.txt

echo "READY" > "$ROOT_DIR/READY.log"

# waits for files send through tcp and saves them in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/8080_images0

python3 -u MidiPython-TCP-Image-Socket/server.py --dummy 1 | tee _server_receive.log
