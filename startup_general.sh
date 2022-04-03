#!/bin/bash
apt-get install git wget curl tar -y
git clone https://github.com/janzuiderveld/vast_ai.git
bash vast_ai/"$1".sh