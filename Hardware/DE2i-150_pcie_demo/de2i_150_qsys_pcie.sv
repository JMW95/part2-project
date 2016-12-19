// ============================================================================
// Copyright (c) 2012 by Terasic Technologies Inc.
// ============================================================================
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// ============================================================================
//           
//  Terasic Technologies Inc
//  9F., No.176, Sec.2, Gongdao 5th Rd, East Dist, Hsinchu City, 30070. Taiwan
//  
//  
//                     web: http://www.terasic.com/  
//                     email: support@terasic.com
//
// ============================================================================
//Date:  Wed Jun 27 19:19:53 2012
// ============================================================================

module de2i_150_qsys_pcie(
	///////// CLOCK2 /////////
	input		CLOCK2_50,

	///////// CLOCK3 /////////
	input		CLOCK3_50,

	///////// CLOCK /////////
	input		CLOCK_50,

	///////// DRAM /////////
	output 	[12:0] DRAM_ADDR,
	output	[1:0]	DRAM_BA,
	output	DRAM_CAS_N,
	output	DRAM_CKE,
	output	DRAM_CLK,
	output	DRAM_CS_N,
	inout	   [31:0] DRAM_DQ,
	output	[3:0] DRAM_DQM,
	output   DRAM_RAS_N,
	output   DRAM_WE_N,

	///////// EEP /////////
	output   EEP_I2C_SCLK,
	inout    EEP_I2C_SDAT,

	///////// ENET /////////
	output   ENET_GTX_CLK,
	input    ENET_INT_N,
	input    ENET_LINK100,
	output   ENET_MDC,
	inout    ENET_MDIO,
	output   ENET_RST_N,
	input    ENET_RX_CLK,
	input    ENET_RX_COL,
	input    ENET_RX_CRS,
	input    [3:0] ENET_RX_DATA,
	input    ENET_RX_DV,
	input    ENET_RX_ER,
	input    ENET_TX_CLK,
	output   [3:0] ENET_TX_DATA,
	output   ENET_TX_EN,
	output   ENET_TX_ER,

	///////// FAN /////////
	inout    FAN_CTRL,

	///////// FL /////////
	output   FL_CE_N,
	output   FL_OE_N,
	input    FL_RY,
	output   FL_WE_N,
	output   FL_WP_N,
	output   FL_RESET_N,
	///////// FS /////////
	inout    [31:0] FS_DQ,
	output   [26:0] FS_ADDR,
	///////// GPIO /////////
	inout    [35:0] GPIO,

	///////// G /////////
	input    G_SENSOR_INT1,
	output   G_SENSOR_SCLK,
	inout    G_SENSOR_SDAT,

	///////// HEX /////////
	output   [6:0] HEX0,
	output   [6:0] HEX1,
	output   [6:0] HEX2,
	output   [6:0] HEX3,
	output   [6:0] HEX4,
	output   [6:0] HEX5,
	output   [6:0] HEX6,
	output   [6:0] HEX7,

	///////// HSMC /////////
	input    HSMC_CLKIN0,
	input    HSMC_CLKIN_N1,
	input    HSMC_CLKIN_N2,
	input    HSMC_CLKIN_P1,
	input    HSMC_CLKIN_P2,
	output   HSMC_CLKOUT0,
	inout    HSMC_CLKOUT_N1,
	inout    HSMC_CLKOUT_N2,
	inout    HSMC_CLKOUT_P1,
	inout    HSMC_CLKOUT_P2,
	inout    [3:0] HSMC_D,
	output   HSMC_I2C_SCLK,
	inout    HSMC_I2C_SDAT,
	inout    [16:0] HSMC_RX_D_N,
	inout    [16:0] HSMC_RX_D_P,
	inout    [16:0] HSMC_TX_D_N,
	inout    [16:0] HSMC_TX_D_P,

	///////// I2C /////////
	output   I2C_SCLK,
	inout    I2C_SDAT,

	///////// IRDA /////////
	input    IRDA_RXD,

	///////// KEY /////////
	input    [3:0] KEY,

	///////// LCD /////////
	inout    [7:0] LCD_DATA,
	output   LCD_EN,
	output   LCD_ON,
	output   LCD_RS,
	output   LCD_RW,

	///////// LEDG /////////
	output   [8:0] LEDG,

	///////// LEDR /////////
	output   [17:0] LEDR,

	///////// PCIE /////////
	input    PCIE_PERST_N,
	input    PCIE_REFCLK_P,
	input    [0:0] PCIE_RX_P,
	output   [0:0] PCIE_TX_P,
	output   PCIE_WAKE_N,

	///////// SD /////////
	output   SD_CLK,
	inout    SD_CMD,
	inout    [3:0] SD_DAT,
	input    SD_WP_N,

	///////// SMA /////////
	input    SMA_CLKIN,
	output   SMA_CLKOUT,

	///////// SSRAM /////////
	output   SSRAM_ADSC_N,
	output   SSRAM_ADSP_N,
	output   SSRAM_ADV_N,
	output   [3:0] SSRAM_BE,
	output   SSRAM_CLK,
	output   SSRAM_GW_N,
	output   SSRAM_OE_N,
	output   SSRAM_WE_N,
	output   SSRAM0_CE_N,
	output   SSRAM1_CE_N,

	///////// SW /////////
	input    [17:0] SW,

	///////// TD /////////
	input    TD_CLK27,
	input    [7:0] TD_DATA,
	input    TD_HS,
	output   TD_RESET_N,
	input    TD_VS,

	///////// UART /////////
	input    UART_CTS,
	output   UART_RTS,
	input    UART_RXD,
	output   UART_TXD,

	///////// VGA /////////
	output   [7:0] VGA_B,
	output   VGA_BLANK_N,
	output   VGA_CLK,
	output   [7:0] VGA_G,
	output   VGA_HS,
	output   [7:0] VGA_R,
	output   VGA_SYNC_N,
	output   VGA_VS
);

wire vsync;
assign VGA_VS = vsync;

wire vga_c;
wire vga_cen;
assign VGA_CLK = vga_c && vga_cen;

de2i_150_qsys u0 (
	.clk_clk                          		(CLOCK_50),
	.reset_reset_n                    		(KEY[0]),
	.pcie_ip_refclk_export            		(PCIE_REFCLK_P),
	.pcie_ip_pcie_rstn_export         		(PCIE_PERST_N),
	.pcie_ip_rx_in_rx_datain_0        		(PCIE_RX_P[0]),
	.pcie_ip_tx_out_tx_dataout_0      		(PCIE_TX_P[0]),
	.led_external_connection_export	 		(LEDG[3:0]),
	.pio_vsync_external_connection_export  (vsync),
	.pixelstream_0_conduit_end_0_lcd_red        (VGA_R),
	.pixelstream_0_conduit_end_0_lcd_green      (VGA_G),
	.pixelstream_0_conduit_end_0_lcd_blue       (VGA_B),
	.pixelstream_0_conduit_end_0_lcd_hsync      (VGA_HS),
	.pixelstream_0_conduit_end_0_lcd_vsync      (vsync),
	//.pixelstream_0_conduit_end_0_lcd_de         (),
	.pixelstream_0_conduit_end_0_lcd_dclk       (vga_c),
	.pixelstream_0_conduit_end_0_lcd_dclk_en    (vga_cen),
);

assign PCIE_WAKE_N = 1'b1;	 // 07/30/2013, pull-high to avoid system reboot after power off

logic [26:0] hb_clk;
always_ff @(posedge CLOCK_50) begin
	hb_clk += 1;
end
assign LEDG[8] = hb_clk[26];


assign LEDG[7:4] = 4'b1111;


endmodule
