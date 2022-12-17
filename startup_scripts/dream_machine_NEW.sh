#!/bin/sh
function finish {
  pkill -P $$
  echo "killed $$"
  exit
}
trap finish EXIT
trap finish SIGINT

cd /workspace
apt-get install git wget curl tar -y
# get rpi branch
git clone -b rpi https://github.com/janzuiderveld/vast_ai.git
cd /workspace/vast_ai/dream_machine

apt-get update
apt-get install ffmpeg libsm6 libxext6 -y

ROOT_DIR=$PWD
export ROOT_DIR

python3 -u -m pip install -r Python-TCP-Image-Socket/requirements.txt 

# Real-ESRGAN
echo "setting up Real-ESRGAN"
git clone https://github.com/xinntao/Real-ESRGAN.git
cd /workspace/vast_ai/dream_machine/Real-ESRGAN
python3 -u -m pip install basicsr facexlib gfpgan
python3 -u -m pip install -r  requirements.txt
python3 -u setup.py develop
wget https://github.com/xinntao/Real-ESRGAN/releases/download/v0.1.0/RealESRGAN_x4plus.pth -P /workspace/vast_ai/dream_machine/Real-ESRGAN/experiments/pretrained_models

echo "setting up dream_machine"
echo "downloading git"
cd /workspace/vast_ai/dream_machine
rm -r /workspace/vast_ai/dream_machine/Sketch-Simulator
git clone https://github.com/janzuiderveld/Sketch-Simulator.git

echo "downloading pretrained model"
cd /workspace/vast_ai/dream_machine/Sketch-Simulator 
pip install torch==1.11.0+cu115 --extra-index-url https://download.pytorch.org/whl/cu115
bash setup.sh | tee ../_sketch_setup.log
# python3 -u -m pip install torch==1.9.0+cu111 torchvision==0.10.0+cu111 -f https://download.pytorch.org/whl/torch_stable.html | tee ../_sketch_torch_update.log


echo "starting dream machine"
# echo "READY" > "$ROOT_DIR/READY.log" # this happens in train_folder_server.py
cd /workspace/vast_ai/dream_machine/Sketch-Simulator
python3 -u train_folder_server.py | tee ../_sketch_sim_server.log
