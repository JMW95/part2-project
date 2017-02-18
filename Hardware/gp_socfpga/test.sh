#!/bin/bash

# Try to download twice in case the JTAG connection is initializing the first time
make -C software/renderer download || make -C software/render download

# SSH into the ARM and run the test
ssh jamie@172.31.60.16 -t 'sudo /home/jamie/run_test.sh'
