set -x
cd /workspace/vast_ai/midialogue/LakhNES
echo "starting LakhNES server"

python3 ../print.py # This is in a different location, but printing works now...??

python3 startup.py | tee -a /workspace/vast_ai/midialogue/LakhNES/logs/startup.log
