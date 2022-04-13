apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git

python3 -m pip install -r dream_machine/Python-TCP-Image-Socket/requirements.txt
apt-get update
apt-get install ffmpeg libsm6 libxext6  -y

cd dream_machine
python3 Python-TCP-Image-Socket/server_receive_socket.py.py 2>&1 | _server_receive.log &
python3 Python-TCP-Image-Socket/server_send_socket.py.py 2>&1 | _server_send.log &

git clone https://github.com/janzuiderveld/Sketch-Simulator.git
python3 -m pip install -r Sketch-Simulator/requirements.txt 

cd Sketch-Simulator 
python3 train_folder_server.py 2>&1 | ../_sketch_sim.log &
python3 train_folder_server.py 2>&1 | ../_sketch_sim.log &
#args to set 
# save_root
# input_dir
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


