#!/bin/bash

case "$(uname -s)" in

   Darwin)
     echo 'Mac OS X'
    #  curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    #  python3 get-pip.py
    # venv?
    export DEVICE=Mac

     ;;

   Linux)
     echo 'Linux'
     apt-get -y install python3-pip python3-venv lsof
    export DEVICE=Linux
     ;;
esac

ROOT_DIR=$PWD
python3 -m venv $ROOT_DIR/dream_machine/dream_machine_env
source $ROOT_DIR/dream_machine/dream_machine_env/bin/activate

# wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;
# python3 vast_bid.py auto1 dream_machine
# python3 vast_bid.py a100 dream_machine
# python3 vast_bid.py best1 dream_machine
# python3 vast_bid.py cheap4 dream_machine
# python3 vast_bid.py cheap dream_machine

# python3 vast_bid.py cheap_od dream_machine_NEW

# check if there is already a running instance
ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")
if [ -z "$ID" ]
then
      echo "No instance running"
      python3 vast_bid.py flops dream_machine_NEW
else
      echo "Instance $ID already running"
      # copy startup script to instance 
      ./vast copy ./startup_scripts/dream_machine_NEW.sh $ID:/workspace/dream_machine_NEW.sh
      # run startup script
      ./vast execute $ID "bash /workspace/dream_machine_NEW.sh"
fi

echo "starting local script"
./startup_scripts/dream_machine_local_NEW.sh 