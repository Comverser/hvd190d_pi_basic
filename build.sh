#!/bin/sh

mkdir -p build

cd ~/dev_s/hvd190d_pi_basic/build

cmake ..

make

./demo ./../src/apps/wf_max_speed.csv
#./demo ./../src/apps/wf.csv
