#!/bin/sh
source ~/.bashrc

cd /workspace
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git

/usr/sbin/sshd -p $DIRECT_PORT_START

cd /workspace/vast_ai/midialogue

python3 -m pip install -r Python-TCP-Image-Socket/requirements.txt
python3 -m pip install pretty_midi

# python3 -m pip install torch==1.0.1.post2 torchvision==0.2.2.post3

# waits for files send through tcp and saves them in vast_ai/midialogue/midi_in
# python3 Python-TCP-Image-Socket/server.py 2>&1 | tee _server_receive.log &
# stdbuf -o0 python3 Python-TCP-Image-Socket/server.py 2>&1 | tee _server_receive.log &

cd /workspace/vast_ai/midialogue
git clone https://github.com/chrisdonahue/LakhNES

mkdir -p /workspace/vast_ai/midialogue/LakhNES/model/pretrained/LakhNES

wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1ND27trP3pTAl6eAk5QiYE9JjLOivqGsd' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1ND27trP3pTAl6eAk5QiYE9JjLOivqGsd" -O /workspace/vast_ai/midialogue/LakhNES/model/pretrained/model.tar.gz && rm -rf /tmp/cookies.txt
wget --no-check-certificate 'https://docs.google.com/uc?export=download&id=1WO6guGagqaw22LH32_NEBeavtbaWy_ar' -O /workspace/vast_ai/midialogue/LakhNES/data/nesmdb_tx1.tar.gz

cd /workspace/vast_ai/midialogue/LakhNES/model/pretrained

tar -xf model.tar.gz && rm model.tar.gz
cd /workspace/vast_ai/midialogue/LakhNES/data
tar -xf /workspace/vast_ai/midialogue/LakhNES/data/nesmdb_tx1.tar.gz

cp /workspace/vast_ai/midialogue/mem_transformer.py /workspace/vast_ai/midialogue/LakhNES/model/mem_transformer.py
cp /workspace/vast_ai/midialogue/startup.py /workspace/vast_ai/midialogue/LakhNES/startup.py

touch /workspace/vast_ai/midialogue/LakhNES/model/utils/__init__.py
touch /workspace/vast_ai/midialogue/LakhNES/model/model/__init__.py
touch /workspace/vast_ai/midialogue/LakhNES/model/__init__.py

sleep 2

cd /workspace/vast_ai/midialogue/

chmod +rwx start_LakhNES.sh
# ./start_LakhNES.sh
bash -i start_LakhNES.sh

# stdbuf -o0 python3 startup.py 2>&1 | tee ../_midialogue_server.log