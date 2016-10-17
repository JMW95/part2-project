# Clock constraints

create_clock -name "clock_50_1" -period 20.000ns [get_ports {CLOCK_50}]
create_clock -name "clock_50_2" -period 20.000ns [get_ports {CLOCK2_50}]
create_clock -name "clock_50_3" -period 20.000ns [get_ports {CLOCK3_50}]
create_clock -name "clock_50_4" -period 20.000ns [get_ports {CLOCK4_50}]
create_clock -name "clock_27_1" -period 37.000ns [get_ports {TD_CLK27}]

create_clock -period "9 MHZ" LCD_DCLK

set_false_path -to [get_ports {LCD_DCLK}]
set_false_path -from [get_keepers {*mkPixelStream:*:vp_sync|sDataSyncIn*}] -to [get_keepers {*|mkPixelStream:*|SyncRegister:vp_sync|dD_OUT*]}]
set_false_path -from [get_keepers {*mkPixelStream:*:pix_sync|sGEnqPtr*}] -to [get_keepers {*|mkPixelStream:*|SyncFIFO:pix_sync|dSyncReg1*}]
set_false_path -from [get_keepers {*mkPixelStream:*:vp_sync|SyncHandshake:sync|sToggleReg}] -to [get_keepers {*mkPixelStream:*|SyncRegister:vp_sync|SyncHandshake:sync|dSyncReg1}]
set_false_path -from [get_keepers {*mkPixelStream:*:pix_sync|dGDeqPtr*}] -to [get_keepers {*|mkPixelStream:pixelstream_0|SyncFIFO:pix_sync|sSyncReg1*}]
set_false_path -from [get_keepers {*mkPixelStream:*:vp_sync|SyncHandshake:sync|dLastState}] -to [get_keepers {*|mkPixelStream:*|SyncRegister:vp_sync|SyncHandshake:sync|sSyncReg1}]
set_false_path -to [get_pins -nocase -compatibility_mode {*|alt_rst_sync_uq1|altera_reset_synchronizer_int_chain*|clrn}]
set_false_path -from [get_keepers {*|altera_reset_controller:rst_controller|r_sync_rst}] -to [get_keepers {*|mkPixelStream:*|SyncRegister:vp_sync|dD_OUT*}]

set_false_path -from [get_keepers {gp_socfpga:u0|mkPixelStream:pixelstream_0|hdmi_vsd}] -to [get_keepers {gp_socfpga:u0|gp_socfpga_pio_vsync:pio_vsync|readdata[0]}]
set_false_path -from [get_keepers {gp_socfpga:u0|mkPixelStream:pixelstream_0|hdmi_vsd}] -to [get_keepers {gp_socfpga:u0|gp_socfpga_pio_vsync:pio_vsync|d1_data_in}]


# Automatically constrain PLL and other generated clocks
derive_pll_clocks -create_base_clocks

# Automatically calculate clock uncertainty to jitter and other effects.
derive_clock_uncertainty

# tsu/th constraints

# tco constraints

# tpd constraints