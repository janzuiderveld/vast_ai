#!/bin/bash
git clone https://github.com/chrisdonahue/LakhNES

python3 - << EOF
import os
os.makedirs("/root/LakhNES/model/pretrained/LakhNES", exist_ok=True)
EOF

wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1ND27trP3pTAl6eAk5QiYE9JjLOivqGsd' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1ND27trP3pTAl6eAk5QiYE9JjLOivqGsd" -O /root/LakhNES/model/pretrained/model.tar.gz && rm -rf /tmp/cookies.txt

cd /root/LakhNES/model/pretrained
tar -xf model.tar.gz && rm model.tar.gz
cd /root/LakhNES