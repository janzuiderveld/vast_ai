cd /Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi-utilities/bin
prefix=$"/Users/janzuiderveld/Documents/GitHub/vast_ai/midialogue/midi_in/"
# ./brainstorm  --in <port> [ --prefix <filename prefix> ] [ --timeout <seconds> ] [ --confirmation <command line> ]
./brainstorm  --in 0 --prefix $prefix --timeout 2 --confirmation 'echo "saved a midi"'