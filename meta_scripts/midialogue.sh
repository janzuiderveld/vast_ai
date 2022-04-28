#!/bin/bash
wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;

apt-get -y install python3-pip python3-venv
# yes | apt-get install python3-pip python3-venv

ROOT_DIR=$PWD
python3 -m venv $ROOT_DIR/midialogue/midialogue_env
source $ROOT_DIR/midialogue/midialogue_env/bin/activate
python3 -m pip install requests

python3 vast_bid.py cheap midialogue
# python3 vast_bid.py auto1_bid midialogue

# sudo chmod a+x startup_scripts/dream_machine_local.sh
./startup_scripts/midialogue_local.sh 
