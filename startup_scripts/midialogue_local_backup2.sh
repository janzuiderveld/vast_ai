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

cd vast_ai

ROOT_DIR=$PWD

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


#$ROOT_DIR/midialogue/midi-utilities/bin/lsmidiins
#echo ""
#read -p 'Which midi input port do you want to listen to?: ' in_port
#echo "using midi input port $in_port"

#echo ""
#echo "Midi Utilities: midi outs"

#$ROOT_DIR/midialogue/midi-utilities/bin/lsmidiouts
#echo ""
#read -p 'Which midi output port do you want to send to?: ' blo_port
#echo "using midi output port $blo_port"
#echo ""

midi_devices=$($ROOT_DIR/midialogue/midi-utilities/bin/lsmidiins)

blo="Blofeld:Blofeld Blofeld"
esi1="ESI MIDIMATE eX:ESI MIDIMATE eX MIDI 1"
esi2="ESI MIDIMATE eX:ESI MIDIMATE eX MIDI 2"
tee="Teensy MIDI:Teensy MIDI MIDI 1"

echo ""
echo $midi_devices
echo ""

blo_index=${midi_devices%%$blo*}
esi1_index=${midi_devices%%$esi1*}
esi2_index=${midi_devices%%$esi2*}
tee_index=${midi_devices%%$tee*}

blo_index=${#blo_index}
esi1_index=${#esi1_index}
esi2_index=${#esi2_index}
tee_index=${#tee_index}

let blo_index=$blo_index-2
let esi1_index=$esi1_index-2
let esi2_index=$esi2_index-2
let tee_index=$tee_index-2

blo_port=${midi_devices:$blo_index:2}
in_port=${midi_devices:$tee_index:2}
es1_port=${midi_devices:$esi1_index:2}
es2_port=${midi_devices:$esi2_index:2}

echo "in_port: $in_port"
echo "blo_port: $blo_port"
echo "es1_port: $es1_port"
echo "es2_port: $es2_port"

in_port=$in_port | xargs echo
blo_port=$blo_port | xargs echo
es1_port=$es1_port | xargs echo
es2_port=$es2_port | xargs echo

case "$(uname -s)" in

   Darwin)
     echo 'Mac OS X'
     model_in_port=10
     ;;

   Linux)
     echo 'Linux'
     model_map_port=10
     model_in_port=11
     out_port=5
     ;;
esac

echo "in_port: $in_port"
echo "blo_port: $blo_port"
echo "es1_port: $es1_port"
echo "es2_port: $es2_port"
echo "model_map_port: $model_map_port"
echo "model_in_port: $model_in_port"
echo "out_port: $out_port"

timeout=1

# WORKING 
# bus 5: to model
echo "bus 5: to model"
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --virtual-in 5 --virtual-out 5 &
sleep 1

# bus 6: to all synths 
echo "bus 6: to all synths"
# save in variable
all_synths=6
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-in 6 --out 2 --out 3 --out 4 &
# $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-in 6 --out $blo_port --out $es1_port --out $es2_port &
sleep 1

# bus 7: to model and all synths
echo "bus 7: to model and all synths"
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --bus --virtual-in 7 --out 0 --out 6 &
sleep 1

# connect input to model and all synths
echo "connect input to relevant bus"
$ROOT_DIR/midialogue/midi-utilities/bin/routemidi --in 1 --out 7 &
input_PID=$!
export input_PID
sleep 1

# python3 $ROOT_DIR/midialogue/midi_scripts/brainstorm_custom.py

# cd $ROOT_DIR/midialogue
# # remove midi_in and midi_out folders
# sudo rm -rf midi_in midi_out
# cd $ROOT_DIR


# source $ROOT_DIR/midialogue/midialogue_env/bin/activate
# # while loop copying over midi files to and from vast and hide output
# cd $ROOT_DIR
# ID=9999999999

# export ID
# export ROOT_DIR

# python3 midialogue/midi_autoplay.py --midi_send_port 6 2>&1 | tee autoplay.log &

# # python3 Python-TCP-Image-Socket/client.py &

# cd $ROOT_DIR/midialogue/midi-utilities/bin
# prefix=$"$ROOT_DIR/midialogue/midi_in/"
# mkdir $ROOT_DIR/midialogue/midi_in
# mkdir $ROOT_DIR/midialogue/midi_out
# ./brainstorm  --in 0 --prefix $prefix --timeout $timeout --confirmation 'echo "saved a midi file"' &
# # ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]
# echo "model listens on virtual port 0"

# cd $ROOT_DIR/midialogue

# cd $ROOT_DIR
# ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")

# export ID
# export ROOT_DIR

# python3 ./midialogue/midi_autoplay.py --midi_send_port 6 | tee autoplay.log &

# python3 $ROOT_DIR/midialogue/midi_scripts/brainstorm_custom.py

# sleep 99999999






# cd $ROOT_DIR/midialogue/midi-utilities/bin
# prefix=$"$ROOT_DIR/midialogue/midi_in/"
# mkdir $ROOT_DIR/midialogue/midi_in
# mkdir $ROOT_DIR/midialogue/midi_out
# ./brainstorm  --in 0 --prefix $prefix --timeout $timeout --confirmation 'echo "saved a midi file"' &
# # ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]
# echo "model listens on virtual port 0"
# sleep 9999999


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
# notemap --in $out_scaling_port --out $blo_port --map $ROOT_DIR/midialogue/note_mappings/x_to_out.xml.xml
# notemap --in $model_scaling_port --out $model_in_port --map $ROOT_DIR/midialogue/note_mappings/x_to_model.xml.xml

# cd /workspace/vast_ai/midialogue/LakhNES/
# ============================================================



# python3 -m venv $ROOT_DIR/midialogue/midialogue_env
# source $ROOT_DIR/midialogue/midialogue_env/bin/activate

# python3 -m pip install wheel cython
# python3 -m pip install -r $ROOT_DIR/midialogue/Python-TCP-Image-Socket/requirements.txt
# python3 -m pip install pretty_midi
# python3 -m pip install requests

cd $ROOT_DIR/midialogue
# remove midi_in and midi_out folders
sudo rm -rf midi_in midi_out


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


cd $ROOT_DIR
ID=$(./vast show instances --raw | python3 -c "import sys, json; print(json.load(sys.stdin)[0]['id'])")

export ID
export ROOT_DIR


# python3 Python-TCP-Image-Socket/client.py &

cd $ROOT_DIR/midialogue/midi-utilities/bin
prefix=$"$ROOT_DIR/midialogue/midi_in/"
mkdir $ROOT_DIR/midialogue/midi_in
mkdir $ROOT_DIR/midialogue/midi_out
# ./brainstorm  --in 0 --prefix $prefix --timeout $timeout --confirmation 'echo "saved a midi file"' &


python3 $ROOT_DIR/midialogue/midi_scripts/brainstorm_custom.py
# ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]
echo "model listens on virtual port 0"

python3 ./midialogue/midi_autoplay.py --midi_send_port 6 | tee autoplay.log &

cd $ROOT_DIR/midialogue
while true; do
  # wait for new recording to be saved
  # python3 new_recording_handler.py 

  # copy to server
  # ./vast copy $ROOT_DIR/midialogue/midi_in $ID:/workspace/vast_ai/midialogue 2> /dev/null

  # turn off input connection   
  # kill $input_PID

  # do light patterns until answer is returned
  # ./vast copy $ID:/workspace/vast_ai/midialogue/midi_out $ROOT_DIR/midialogue 2> /dev/null

  # wait for autoplay to finish

  # send note off panic to synths


  # turn on input connection
  # $ROOT_DIR/midialogue/midi-utilities/bin/routemidi --in $in_port --out 7 &
  # input_PID=$!


  sleep 9999999
done


# TODO add note off after playback
