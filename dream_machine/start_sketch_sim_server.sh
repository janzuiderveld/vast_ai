#!/bin/sh
source ~/.bashrc
cd /workspace/vast_ai/dream_machine/Sketch-Simulator

echo "starting sketch sim server"
# python3 startup.py 2>&1 | tee _LakhNES.log
python3 -u train_folder_server.py 2>&1 | tee _server.log