#!/bin/bash

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

ROOT_DIR=$PWD
python3 -m venv $ROOT_DIR/dream_machine/dream_machine_env
source $ROOT_DIR/dream_machine/dream_machine_env/bin/activate

python3 -m pip install requests

# wget https://raw.githubusercontent.com/vast-ai/vast-python/master/vast.py -O vast; chmod +x vast;
python3 vast_bid.py auto1 dream_machine
# python3 vast_bid.py a100 dream_machine
# python3 vast_bid.py best1 dream_machine
# python3 vast_bid.py cheap dream_machine
# python3 vast_bid.py cheap4 dream_machine


echo "Wating 60 sec before starting local script"
sleep 60

# sudo chmod a+x startup_scripts/dream_machine_local.sh
./startup_scripts/dream_machine_local.sh 





# #!/bin/bash
# python3 vast_bid.py auto1 dream_machine
# # python3 vast_bid.py a100 dream_machine
# # python3 vast_bid.py best1 dream_machine
# # python3 vast_bid.py cheap dream_machine
# # python3 vast_bid.py cheap4 dream_machine


# echo "Wating 60 sec before starting local script"
# sleep 60

# # sudo chmod a+x startup_scripts/dream_machine_local.sh
# ./startup_scripts/dream_machine_local.sh 






