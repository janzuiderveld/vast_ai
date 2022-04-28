#!/bin/bash
wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;

apt-get -y install python3-pip python3-venv
# yes | apt-get install python3-pip python3-venv

python3 vast_bid.py cheap midialogue
# python3 vast_bid.py auto1_bid midialogue

# sudo chmod a+x startup_scripts/dream_machine_local.sh
./startup_scripts/midialogue_local.sh 
