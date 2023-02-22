#!/bin/bash
# loop until killed
while true; do
    # wait for new recording to be saved
    python3 new_recording_handler.py 
    
    # copy to server
    ./vast copy $ROOT_DIR/midialogue/midi_in $ID:/workspace/vast_ai/midialogue 2> /dev/null
    
    # turn off input connection   
    kill $input_PID
    
    # do light patterns until answer is returned
    ./vast copy $ID:/workspace/vast_ai/midialogue/midi_out $ROOT_DIR/midialogue 2> /dev/null
    
    # wait for autoplay to finish
    
    # send note off panic to synths
    
    done