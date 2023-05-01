#!/bin/bash
function finish {
  pkill -P $$
  echo "killed $$"
  cd $ROOT_DIR
  ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")
  echo "destroying instance $ID"
  ./vast destroy instance $ID
  exit
}
trap finish EXIT
trap finish SIGINT

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
cd $ROOT_DIR/dream_machine

echo "activating venv"
python3 -u -m venv $ROOT_DIR/dream_machine/dream_machine_env
source $ROOT_DIR/dream_machine/dream_machine_env/bin/activate

echo "installing requirements"
python3 -u -m pip install -r $ROOT_DIR/dream_machine/Python-TCP-Image-Socket/requirements.txt
python3 -u -m pip install requests 
python3 -u -m pip install pillow 

if [[ $DEVICE != "Mac" ]]; then
  echo "starting windows XP server"
  python3 -u ./MidiPython-TCP-Image-Socket/sender_3.4.py &
  sleep 5
  echo "starting serial"
  sudo python3 -u ./serial/first.py &
fi

echo "waiting for server to be ready..."
python3 -u ./file_transfer/check_ready.py
echo "Server ready"

python3 -u ./file_transfer/client.py