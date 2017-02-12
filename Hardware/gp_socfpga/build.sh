#!/bin/bash

if [ -z $1 ];
then
	echo "Usage: build.sh <num_cores>"
	exit 1
fi

qsys-script --system=gp_socfpga_system.qsys --cmd="set num_cores $1" --script=core_reconfig.tcl
qsys-generate --synthesis=VERILOG gp_socfpga_system.qsys

quartus_sh --flow compile gp_socfpga
