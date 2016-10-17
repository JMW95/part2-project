# TCL File Generated by Component Editor 16.0
# Mon Oct 17 19:35:59 BST 2016
# DO NOT MODIFY


# 
# palette "4->16bpp Palette" v1.0
# Jamie Wood 2016.10.17.19:35:59
# 
# 

# 
# request TCL package from ACDS 16.0
# 
package require -exact qsys 16.0


# 
# module palette
# 
set_module_property DESCRIPTION ""
set_module_property NAME palette
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR "Jamie Wood"
set_module_property DISPLAY_NAME "4->16bpp Palette"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL palette
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file palette.sv SYSTEM_VERILOG PATH palette/palette.sv TOP_LEVEL_FILE


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
set_interface_property slave maximumPendingReadTransactions 1
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

add_interface_port slave avs_slave_read read Input 1
add_interface_port slave avs_slave_address address Input 24
add_interface_port slave avs_slave_waitrequest waitrequest Output 1
add_interface_port slave avs_slave_readdata readdata Output 32
add_interface_port slave avs_slave_readdatavalid readdatavalid Output 1
set_interface_assignment slave embeddedsw.configuration.isFlash 0
set_interface_assignment slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment slave embeddedsw.configuration.isPrintableDevice 0


# 
# connection point master
# 
add_interface master avalon start
set_interface_property master addressUnits SYMBOLS
set_interface_property master associatedClock clock
set_interface_property master associatedReset reset
set_interface_property master bitsPerSymbol 8
set_interface_property master burstOnBurstBoundariesOnly false
set_interface_property master burstcountUnits WORDS
set_interface_property master doStreamReads false
set_interface_property master doStreamWrites false
set_interface_property master holdTime 0
set_interface_property master linewrapBursts false
set_interface_property master maximumPendingReadTransactions 0
set_interface_property master maximumPendingWriteTransactions 0
set_interface_property master readLatency 0
set_interface_property master readWaitTime 1
set_interface_property master setupTime 0
set_interface_property master timingUnits Cycles
set_interface_property master writeWaitTime 0
set_interface_property master ENABLED true
set_interface_property master EXPORT_OF ""
set_interface_property master PORT_NAME_MAP ""
set_interface_property master CMSIS_SVD_VARIABLES ""
set_interface_property master SVD_ADDRESS_GROUP ""

add_interface_port master avm_master_read read Output 1
add_interface_port master avm_master_address address Output 24
add_interface_port master avm_master_readdata readdata Input 8
add_interface_port master avm_master_readdatavalid readdatavalid Input 1
add_interface_port master avm_master_waitrequest waitrequest Input 1


# 
# connection point palette
# 
add_interface palette avalon end
set_interface_property palette addressUnits WORDS
set_interface_property palette associatedClock clock
set_interface_property palette associatedReset reset
set_interface_property palette bitsPerSymbol 8
set_interface_property palette burstOnBurstBoundariesOnly false
set_interface_property palette burstcountUnits WORDS
set_interface_property palette explicitAddressSpan 0
set_interface_property palette holdTime 0
set_interface_property palette linewrapBursts false
set_interface_property palette maximumPendingReadTransactions 0
set_interface_property palette maximumPendingWriteTransactions 0
set_interface_property palette readLatency 0
set_interface_property palette readWaitTime 1
set_interface_property palette setupTime 0
set_interface_property palette timingUnits Cycles
set_interface_property palette writeWaitTime 0
set_interface_property palette ENABLED true
set_interface_property palette EXPORT_OF ""
set_interface_property palette PORT_NAME_MAP ""
set_interface_property palette CMSIS_SVD_VARIABLES ""
set_interface_property palette SVD_ADDRESS_GROUP ""

add_interface_port palette avs_palette_address address Input 4
add_interface_port palette avs_palette_writedata writedata Input 16
add_interface_port palette avs_palette_readdata readdata Output 16
add_interface_port palette avs_palette_write write Input 1
set_interface_assignment palette embeddedsw.configuration.isFlash 0
set_interface_assignment palette embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment palette embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment palette embeddedsw.configuration.isPrintableDevice 0

