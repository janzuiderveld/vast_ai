cd /workspace
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git

cd /workspace/vast_ai/dream_machine
rm -r /workspace/vast_ai/dream_machine/Sketch-Simulator
git clone https://github.com/janzuiderveld/Sketch-Simulator.git

cd /workspace/vast_ai/dream_machine/Sketch-Simulator 
bash setup.sh | tee ../_sketch_setup.log
