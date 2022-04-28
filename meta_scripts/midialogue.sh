#!/bin/bash
wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;
python3 vast_bid.py cheap midialogue
# python3 vast_bid.py auto1_bid midialogue

# sudo chmod a+x startup_scripts/dream_machine_local.sh
./startup_scripts/midialogue_local.sh 
