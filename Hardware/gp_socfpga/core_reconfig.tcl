package require -exact qsys 16.0

# Check if num_cores has been specified
if {[expr ! [info exists num_cores]]} {
	puts "Must set num_cores using a --cmd arg"
	puts "e.g. qsys-script --cmd=\"set num_cores 8\" --script=\"core_reconfig.tcl\""
	exit
}

# Check number of cores is 1,2,4 or 8
if { !(($num_cores == 1) || ($num_cores == 2) || ($num_cores == 4) || ($num_cores == 8)) } {
	puts "WRONG NUMBER OF CORES"
	puts "Must be 1, 2, 4 or 8"
	exit
}

# Print a big banner
proc stage { name } {
	puts ""
	puts "****************************************"
	puts "* ${name}"
	puts "****************************************"
}

stage "LOADING SYSTEM FROM .QSYS FILE"
set system_name "gp_socfpga_system"
# Load the .qsys file
load_system "${system_name}.qsys"



stage "RECONFIGURING CORES"
# Go through each core and set its num_cores parameter
for { set i 0 } { $i < 8 } { incr i } {
	set ci core_$i
	set_instance_parameter_value $ci num_cores $num_cores
	if { $i < $num_cores } {
		set_instance_property $ci ENABLED true
	} else {
		set_instance_property $ci ENABLED false
	}
}



stage "RECONFIGURING CONNECTION ADDRESSES"
set step [ expr 0x20000 / $num_cores ]
for { set i 0 } { $i < $num_cores } {incr i } {
	set ci core_$i
	set addr1 [ expr $i*$step ]
	set addr2 [ expr 0x100000 + $addr1 ]
	set_connection_parameter_value palette_0.master/${ci}.fbuff1 baseAddress $addr1
	set_connection_parameter_value palette_0.master/${ci}.fbuff2 baseAddress $addr2
}



stage "SETTING GLOBAL INFO"
set_instance_parameter_value palette_0 NUM_CORES $num_cores
set_instance_parameter_value num_cores_id id $num_cores



stage "SAVING SYSTEM TO .QSYS FILE"
save_system "${system_name}.qsys"
