cd /workspace
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git

apt-get update
apt-get install ffmpeg libsm6 libxext6 -y

cd /workspace/vast_ai/midialogue

python3 -m pip install -r Python-TCP-Image-Socket/requirements.txt

echo "READY" > /workspace/vast_ai/midialogue/READY.log

# waits for files send through tcp and saves them in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/8080_images0
python3 Python-TCP-Image-Socket/server.py --dummy 1 2>&1 | tee _server_receive.log &

