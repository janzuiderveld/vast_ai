#!/bin/bash
python3 vast_bid.py auto1 dream_machine


echo "Wating 60 sec before starting local script"
sleep 60

# sudo chmod a+x startup_scripts/dream_machine_local.sh
./startup_scripts/dream_machine_local.sh 
