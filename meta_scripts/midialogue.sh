#!/bin/bash
# wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;

case "$(uname -s)" in

   Darwin)
     echo 'Mac OS X'
    #  curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
    #  python3 get-pip.py
    # venv?
     ;;

   Linux)
     echo 'Linux'
     apt-get -y install python3-pip python3-venv lsof
     ;;
esac

# yes | apt-get install python3-pip python3-venv

ROOT_DIR=$PWD
python3 -m venv $ROOT_DIR/midialogue/midialogue_env
source $ROOT_DIR/midialogue/midialogue_env/bin/activate

python3 -m pip install requests

# python3 vast_bid.py cheap midialogue
python3 vast_bid.py auto1_bid midialogue

# sudo chmod a+x startup_scripts/midialogue_local.sh
./startup_scripts/midialogue_local.sh 
