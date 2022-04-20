cd /workspace
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git

apt-get update
apt-get install ffmpeg libsm6 libxext6 -y

cd /workspace/vast_ai/dream_machine

python3 -m pip install -r Python-TCP-Image-Socket/requirements.txt


# Real-ESRGAN

# git clone https://github.com/xinntao/Real-ESRGAN.git
# cd Real-ESRGAN
# python3 -m pip install basicsr facexlib gfpgan
# python3 -m pip install -r  requirements.txt
# python3 setup.py develop


# waits for files send through tcp and saves them in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/8080_images0
python3 Python-TCP-Image-Socket/server.py 2>&1 | tee _server_receive.log &

rm -r /workspace/vast_ai/dream_machine/Sketch-Simulator
git clone https://github.com/janzuiderveld/Sketch-Simulator.git
wget https://github.com/xinntao/Real-ESRGAN/releases/download/v0.1.0/RealESRGAN_x4plus.pth -P experiments/pretrained_models

cd Sketch-Simulator 
bash setup.sh | tee ../_sketch_setup.log
# python3 -m pip install torch==1.9.0+cu111 torchvision==0.10.0+cu111 -f https://download.pytorch.org/whl/torch_stable.html | tee ../_sketch_torch_update.log
python3 train_folder_server.py 2>&1 | tee ../_sketch_sim_server.log &


# python3 Python-TCP-Image-Socket/server_send_socket.py 2>&1 | tee _server_send.log &

#args to set 
# save_root
# input_dirs
# output_dir
# padding
# embedding_avg
# edge_weight

# parser.add_argument('--clip_model', type=str, default='ViT-B/32' )
# parser.add_argument('--width', type=int, default= 600 )
# parser.add_argument('--height', type=int, default= 600 )

# parser.add_argument('--cutn', type=int, default=64 )
# parser.add_argument('--accum_iter', type=int, default=1)
# parser.add_argument('--init_cutn', type=int, default=256)
# parser.add_argument('--num_init_cut_batches', type=int, default=1)
# parser.add_argument('--cut_pow', type=float, default=0.7)
# parser.add_argument('--init_cut_pow', type=int, default=0.3)


