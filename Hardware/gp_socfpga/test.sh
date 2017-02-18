#!/bin/bash

function trydownload(){
	make -C software/renderer download
}

# Try to download a few times in case the JTAG connection is initializing the first time
n=0
until [ $n -ge 5 ]
do
	trydownload && break  # substitute your command here
	n=$[$n+1]
	sleep 1
done

if [ $n -eq 5 ]
then
	echo "Failed to download to FPGA after 5 retries... Is it connected?"
	exit 1
fi

# SSH into the ARM and run the test
ssh jamie@172.31.60.16 -t 'sudo /home/jamie/run_test.sh'
