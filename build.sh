#!/bin/sh

mkdir -p build

cd ~/dev_s/hvd190d_pi_basic/build

cmake ..

make

sudo ./demo ./wf_char_test.csv
#sudo ./demo ./wf_arb.csv
#./demo ./../src/apps/wf_trig.csv
#./demo ./../src/apps/wf.csv
#./demo ./../src/apps/wf_max_speed_trig.csv
#./demo ./../src/apps/wf_max_speed.csv
