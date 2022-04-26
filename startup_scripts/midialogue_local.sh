#!/bin/bash
lsof -ti:8080 | xargs kill -9

/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/lsmidiins
read -p 'Which midi input port do you want to listen to?: ' in_port
echo "using midi input port $in_port"

/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/lsmidiouts
read -p 'Which midi output port do you want to send to?: ' out_port
echo "using midi output port $out_port"

model_port=10
timeout=2

# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --in 0 --virtual-out 10 0 &
/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --in $in_port --out $out_port &
/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --in $in_port --virtual-out $model_port &

# Split channel 4 for different mappings?
# out_scaling_port=11
# model_scaling_port=12

# ROUTEMIDI
# routemidi lets you define multiple busses, each of which reads from multiple input ports, merges the streams together, and copies the result to multiple output ports. You can route channels freely between busses. On Linux and MacOS it also lets you establish virtual ports for other applications to connect to later.
# Usage: routemidi [ --bus | --in <port> | --out <port> | --virtual-in <port> | --virtual-out <port> | --channel <input bus number> <input channel number> <output bus number> <output channel number> ] ...

# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --in $in_port  &
# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --virtual-out $out_scaling_port &
# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --bus --virtual-out $model_scaling_port &
# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --channel 1 4 2 4 &
# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi --channel 1 4 3 4 &
# /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin/routemidi  --bus --in $in_port | --virtual-out 10 11
# notemap --in $out_scaling_port --out $out_port --map /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/note_mappings/x_to_out.xml.xml
# notemap --in $model_scaling_port --out $model_port --map /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/note_mappings/x_to_model.xml.xml

# cd /workspace/vast_ai/midialogue/LakhNES/

python3 -m venv dream_machine/dream_machine_env
source dream_machine/dream_machine_env/bin/activate

python3 -m pip install -r midialogue/Python-TCP-Image-Socket/requirements.txt
python3 -m pip install requests
python3 -m pip install pretty_midi

# apt-get update
# apt-get install ffmpeg libsm6 libxext6  -y

# apt install inotify-tools -y
# python3 -m pip install pyinotify

cd midialogue
# remove midi_in and midi_out folders
rm -rf midi_in midi_out

# python3 Python-TCP-Image-Socket/client_receive_socket.py 2>&1 | tee _client_receive.log &



# until [ -f /tmp/examplefile.txt ]
# do
#      sleep 5
# done
# echo "SERVER READY"


# looks for files to be added to /workspace/vast_ai/dream_machine/Sketch-Simulator/out.log 
# (when inotify script is running and detecting files in Sketch-Simulator/out/to_send) 
# files are copied to out_imgs.



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


# looks for files to appear in /Users/janzuiderveld/Documents/GitHub/vast_ai/dream_machine/in_imgs, sends them over tcp
# python3 Python-TCP-Image-Socket/client.py 2>&1 | tee _client_send.log &
# python3 ./Python-TCP-Image-Socket/client.py


python3 midi_autoplay.py --midi_out_port $out_port &

python3 ./Python-TCP-Image-Socket/client.py &


cd /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin
prefix=$"/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_in/"
# ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]


echo "model listens on virtual port $model_port"
# saves files which will be sent to model
./brainstorm  --in $model_port --prefix $prefix --timeout $timeout --confirmation 'echo "saved a midi file"'

