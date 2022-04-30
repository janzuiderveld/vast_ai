#!/bin/bash
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

# make sure there are no more tunnels on port 8080 the machine
lsof -ti:8080 | xargs kill -9

ROOT_DIR=$PWD

cd $ROOT_DIR/dream_machine

python3 -u -m venv dream_machine_env
source dream_machine_env/bin/activate

python3 -u -m pip install -r Python-TCP-Image-Socket/requirements.txt
python3 -u -m pip install requests

echo "waiting for server to be ready..."
kill -9 $(lsof -t -i:8080)
python3 -u MidiPython-TCP-Image-Socket/check_ready.py
sleep 2
kill -9 $(lsof -t -i:8080)
sleep 2

echo $(lsof -i:8080)

cmd=`cat ssh_pipe.cmd` 
$cmd > test_ssh.thrash &

sleep 3

echo $(lsof -i:8080)

echo "Server ready"

python3 -u MidiPython-TCP-Image-Socket/client.py --dummy 1