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

cd $ROOT_DIR/midialogue

echo "ROOT_DIR: $ROOT_DIR"

if [ ! -d "$ROOT_DIR/midialogue/midi-utilities" ]; then
  # Take action if $DIR exists. #
    unzip -n midi-utilities-20210908.zip
    cp brainstorm.c $ROOT_DIR/midialogue/midi-utilities/src/brainstorm/brainstorm.c
    cp Makefile.unix $ROOT_DIR/midialogue/midi-utilities/src/Makefile.unix
    cd $ROOT_DIR/midialogue/midi-utilities/src/
    make -f Makefile.unix
fi

echo "waiting for server to start up (3 minutes)"
# sleep 180

echo ""
echo "Midi Utilities: midi ins" 

$ROOT_DIR/midialogue/midi-utilities/bin/lsmidiins
echo ""
read -p 'Which midi input port do you want to listen to?: ' in_port
echo "using midi input port $in_port"

echo ""
echo "Midi Utilities: midi outs"

$ROOT_DIR/midialogue/midi-utilities/bin/lsmidiouts
echo ""
read -p 'Which midi output port do you want to send to?: ' out_port
echo "using midi output port $out_port"
echo ""

case "$(uname -s)" in

   Darwin)
     echo 'Mac OS X'
     model_port=10
     ;;

   Linux)
     echo 'Linux'
     model_port=3
     ;;
esac

timeout=1

$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --in $in_port --out $out_port &
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --in $in_port --virtual-out $model_port &

# ============================================================
# Split channel 4 for different mappings?
# out_scaling_port=11
# model_scaling_port=12

# ROUTEMIDI
# routemidi lets you define multiple busses, each of which reads from multiple input ports, merges the streams together, and copies the result to multiple output ports. You can route channels freely between busses. On Linux and MacOS it also lets you establish virtual ports for other applications to connect to later.
# Usage: routemidi [ --bus | --in <port> | --out <port> | --virtual-in <port> | --virtual-out <port> | --channel <input bus number> <input channel number> <output bus number> <output channel number> ] ...

# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --in $in_port  &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-out $out_scaling_port &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-out $model_scaling_port &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --channel 1 4 2 4 &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --channel 1 4 3 4 &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi  --bus --in $in_port | --virtual-out 10 11
# notemap --in $out_scaling_port --out $out_port --map $ROOT_DIR/midialogue/note_mappings/x_to_out.xml.xml
# notemap --in $model_scaling_port --out $model_port --map $ROOT_DIR/midialogue/note_mappings/x_to_model.xml.xml

# cd /workspace/vast_ai/midialogue/LakhNES/
# ============================================================

python3 -m venv $ROOT_DIR/midialogue/midialogue_env
source $ROOT_DIR/midialogue/midialogue_env/bin/activate

python3 -m pip install wheel cython
python3 -m pip install -r $ROOT_DIR/midialogue/Python-TCP-Image-Socket/requirements.txt
python3 -m pip install pretty_midi

cd $ROOT_DIR/midialogue
# remove midi_in and midi_out folders
rm -rf midi_in midi_out


echo "waiting for server to be ready..."
kill -9 $(lsof -t -i:8080)
python3 ./Python-TCP-Image-Socket/check_ready.py
sleep 2
kill -9 $(lsof -t -i:8080)
sleep 2

echo $(lsof -i:8080)

cmd=`cat ssh_pipe.cmd` 
$cmd > test_ssh.thrash &

sleep 3

echo $(lsof -i:8080)

echo "Server ready"

python3 midi_autoplay.py --midi_out_port $out_port &

python3 Python-TCP-Image-Socket/client.py &

cd $ROOT_DIR/midialogue/midi-utilities/bin
prefix=$"$ROOT_DIR/midialogue/midi_in/"
# ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]

echo "model listens on virtual port $model_port"
# saves files which will be sent to model
./brainstorm  --in $model_port --prefix $prefix --timeout $timeout --confirmation 'echo "saved a midi file"'
