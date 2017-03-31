#!/bin/bash

function trydownload(){
	make -C software/renderer download
}

function dotest(){
	algorithm=$1
	echo $algorithm
	make -C software/renderer ALGORITHM=ALGORITHM_$algorithm clean all update-mem

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
	ssh jamie@192.168.0.16 -t 'cd /home/jamie/part2-project/Software && sudo ./run_test.sh'
}

dotest "BYTE_FILL"
dotest "WORD_FILL"
dotest "UNROLLED"
dotest "LINE_FILLER"
