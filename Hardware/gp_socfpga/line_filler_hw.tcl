# TCL File Generated by Component Editor 16.0
# Sat Feb 18 14:45:32 GMT 2017
# DO NOT MODIFY


# 
# line_filler "Line Filler" v1.0
#  2017.02.18.14:45:32
# 
# 

# 
# request TCL package from ACDS 16.0
# 
package require -exact qsys 16.0


# 
# module line_filler
# 
set_module_property DESCRIPTION ""
set_module_property NAME line_filler
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Line Filler"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL line_filler
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file line_filler.sv SYSTEM_VERILOG PATH line_filler/line_filler.sv TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset reset Input 1


# 
# connection point fbuff
# 
add_interface fbuff avalon start
set_interface_property fbuff addressUnits WORDS
set_interface_property fbuff associatedClock clock
set_interface_property fbuff associatedReset reset
set_interface_property fbuff bitsPerSymbol 8
set_interface_property fbuff burstOnBurstBoundariesOnly false
set_interface_property fbuff burstcountUnits WORDS
set_interface_property fbuff doStreamReads false
set_interface_property fbuff doStreamWrites false
set_interface_property fbuff holdTime 0
set_interface_property fbuff linewrapBursts false
set_interface_property fbuff maximumPendingReadTransactions 0
set_interface_property fbuff maximumPendingWriteTransactions 0
set_interface_property fbuff readLatency 0
set_interface_property fbuff readWaitTime 1
set_interface_property fbuff setupTime 0
set_interface_property fbuff timingUnits Cycles
set_interface_property fbuff writeWaitTime 0
set_interface_property fbuff ENABLED true
set_interface_property fbuff EXPORT_OF ""
set_interface_property fbuff PORT_NAME_MAP ""
set_interface_property fbuff CMSIS_SVD_VARIABLES ""
set_interface_property fbuff SVD_ADDRESS_GROUP ""

add_interface_port fbuff avm_fbuff_write write Output 1
add_interface_port fbuff avm_fbuff_address address Output 16
add_interface_port fbuff avm_fbuff_writedata writedata Output 32
add_interface_port fbuff avm_fbuff_byteenable byteenable Output 4
add_interface_port fbuff avm_fbuff_waitrequest waitrequest Input 1


# 
# connection point slave
# 
add_interface slave avalon end
set_interface_property slave addressUnits WORDS
set_interface_property slave associatedClock clock
set_interface_property slave associatedReset reset
set_interface_property slave bitsPerSymbol 8
set_interface_property slave burstOnBurstBoundariesOnly false
set_interface_property slave burstcountUnits WORDS
set_interface_property slave explicitAddressSpan 0
set_interface_property slave holdTime 0
set_interface_property slave linewrapBursts false
set_interface_property slave maximumPendingReadTransactions 0
set_interface_property slave maximumPendingWriteTransactions 0
set_interface_property slave readLatency 0
set_interface_property slave readWaitTime 1
set_interface_property slave setupTime 0
set_interface_property slave timingUnits Cycles
set_interface_property slave writeWaitTime 0
set_interface_property slave ENABLED true
set_interface_property slave EXPORT_OF ""
set_interface_property slave PORT_NAME_MAP ""
set_interface_property slave CMSIS_SVD_VARIABLES ""
set_interface_property slave SVD_ADDRESS_GROUP ""

add_interface_port slave avs_slave_write write Input 1
add_interface_port slave avs_slave_address address Input 17
add_interface_port slave avs_slave_writedata writedata Input 32
add_interface_port slave avs_slave_byteenable byteenable Input 4
add_interface_port slave avs_slave_waitrequest waitrequest Output 1
set_interface_assignment slave embeddedsw.configuration.isFlash 0
set_interface_assignment slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment slave embeddedsw.configuration.isPrintableDevice 0

