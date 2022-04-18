python3 -m venv dream_machine/dream_machine_env
source dream_machine/dream_machine_env/bin/activate

python3 -m pip install -r dream_machine/Python-TCP-Image-Socket/requirements.txt

# apt-get update
# apt-get install ffmpeg libsm6 libxext6  -y

# apt install inotify-tools -y
# python3 -m pip install pyinotify

cd dream_machine
# python3 Python-TCP-Image-Socket/client_receive_socket.py 2>&1 | tee _client_receive.log &
python3 Python-TCP-Image-Socket/client_send_socket.py 2>&1 | tee _client_send.log &

./vast ssh-url > ssh_details.log 
while 
do
    ./vast ssh-url > ssh_details.log
done