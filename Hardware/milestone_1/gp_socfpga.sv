//
// Copyright (c) 2015 A. Theodore Markettos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Top level file for DE1-SoC board with
// Cambridge display board

// Uncomment this if you have an HPS (ARM CPU) in your design
`define ENABLE_HPS

module gp_socfpga(

      // Analogue-digital converter
      inout              ADC_CS_N,
      output             ADC_DIN,
      input              ADC_DOUT,
      output             ADC_SCLK,

      // Audio DAC
      input              AUD_ADCDAT,
      inout              AUD_ADCLRCK,
      inout              AUD_BCLK,
      output             AUD_DACDAT,
      inout              AUD_DACLRCK,
      output             AUD_XCK,

      // Clocks
      input              CLOCK_50,
      input              CLOCK2_50,
      input              CLOCK3_50,
      input              CLOCK4_50,

      // FPGA-side SDRAM
      output      [12:0] DRAM_ADDR,
      output      [1:0]  DRAM_BA,
      output             DRAM_CAS_N,
      output             DRAM_CKE,
      output             DRAM_CLK,
      output             DRAM_CS_N,
      inout       [15:0] DRAM_DQ,
      output             DRAM_LDQM,
      output             DRAM_RAS_N,
      output             DRAM_UDQM,
      output             DRAM_WE_N,

      // Fan control (unused on native board)
      output             FAN_CTRL,

      // FPGA I2C
      output             FPGA_I2C_SCLK,
      inout              FPGA_I2C_SDAT,

      // General purpose I/O
      inout     [35:0]         GPIO_0,
 
      // Hex LEDs
      output      [6:0]  HEX0,
      output      [6:0]  HEX1,
      output      [6:0]  HEX2,
      output      [6:0]  HEX3,
      output      [6:0]  HEX4,
      output      [6:0]  HEX5,

`ifdef ENABLE_HPS
      // ARM Cortex A9 Hard Processor System
      inout              HPS_CONV_USB_N,
      output      [14:0] HPS_DDR3_ADDR,
      output      [2:0]  HPS_DDR3_BA,
      output             HPS_DDR3_CAS_N,
      output             HPS_DDR3_CKE,
      output             HPS_DDR3_CK_N,
      output             HPS_DDR3_CK_P,
      output             HPS_DDR3_CS_N,
      output      [3:0]  HPS_DDR3_DM,
      inout       [31:0] HPS_DDR3_DQ,
      inout       [3:0]  HPS_DDR3_DQS_N,
      inout       [3:0]  HPS_DDR3_DQS_P,
      output             HPS_DDR3_ODT,
      output             HPS_DDR3_RAS_N,
      output             HPS_DDR3_RESET_N,
      input              HPS_DDR3_RZQ,
      output             HPS_DDR3_WE_N,
      output             HPS_ENET_GTX_CLK,
      inout              HPS_ENET_INT_N,
      output             HPS_ENET_MDC,
      inout              HPS_ENET_MDIO,
      input              HPS_ENET_RX_CLK,
      input       [3:0]  HPS_ENET_RX_DATA,
      input              HPS_ENET_RX_DV,
      output      [3:0]  HPS_ENET_TX_DATA,
      output             HPS_ENET_TX_EN,
      inout       [3:0]  HPS_FLASH_DATA,
      output             HPS_FLASH_DCLK,
      output             HPS_FLASH_NCSO,
      inout              HPS_GSENSOR_INT,
      inout              HPS_I2C1_SCLK,
      inout              HPS_I2C1_SDAT,
      inout              HPS_I2C2_SCLK,
      inout              HPS_I2C2_SDAT,
      inout              HPS_I2C_CONTROL,
      inout              HPS_KEY,
      inout              HPS_LED,
      inout              HPS_LTC_GPIO,
      output             HPS_SD_CLK,
      inout              HPS_SD_CMD,
      inout       [3:0]  HPS_SD_DATA,
      output             HPS_SPIM_CLK,
      input              HPS_SPIM_MISO,
      output             HPS_SPIM_MOSI,
      inout              HPS_SPIM_SS,
      input              HPS_UART_RX,
      output             HPS_UART_TX,
      input              HPS_USB_CLKOUT,
      inout       [7:0]  HPS_USB_DATA,
      input              HPS_USB_DIR,
      input              HPS_USB_NXT,
      output             HPS_USB_STP,
`endif /*ENABLE_HPS*/

      // Infra-red
      input              IRDA_RXD,
      output             IRDA_TXD,

      // Push buttons on DE1-SoC mainboard
      input       [3:0]  KEY,

      // Red LED row
      output      [9:0]  LEDR,

      // PS2 port
      inout              PS2_CLK,
      inout              PS2_CLK2,
      inout              PS2_DAT,
      inout              PS2_DAT2,

      // Slide switches
      input       [9:0]  SW,

      // TMDS
      input              TD_CLK27,
      input      [7:0]  TD_DATA,
      input             TD_HS,
      output             TD_RESET_N,
      input             TD_VS,


      // VGA video
      output      [7:0]  VGA_B,
      output             VGA_BLANK_N,
      output             VGA_CLK,
      output      [7:0]  VGA_G,
      output             VGA_HS,
      output      [7:0]  VGA_R,
      output             VGA_SYNC_N,
      output             VGA_VS,

      // Cambridge display board (plugged into GPIO1 port)

      // rotary dials
      input       [1:0]  DIALL,
      input       [1:0]  DIALR,
      // LED pixel ring (inverted before reaching ring)
      output             LEDRINGn,
      
      // LCD display
      output      [7:0]  LCD_R,
      output      [7:0]  LCD_G,
      output      [7:0]  LCD_B,
      // -- only LCD_R[7:2], LCD_G[7:1], LCD_B[7:2] are wired
      // through to display board, low-order pins are ignored
        
      output             LCD_HSYNC,
      output             LCD_VSYNC,
      output             LCD_DEN,
      output             LCD_DCLK,
      output             LCD_ON,	    // set high to enable LCD panel
      output             LCD_BACKLIGHT, // set high to turn on backlight, PWM to dim
      
      // shift register for buttons on display board
      output             SHIFT_CLKIN,
      output             SHIFT_LOAD,
      input              SHIFT_OUT,

      // capacitive touch sensor reset (high=enabled)
      output             TOUCH_WAKE,
      // I2C for touch, temperature and EEPROM
      inout              DISPLAY_SDA,
      inout              DISPLAY_SCL

);


always_comb begin
    LCD_BACKLIGHT = 1'b1;
    LCD_ON = 1'b1;
end


gp_socfpga_system u0 (
    .clk_clk                         (CLOCK_50),
    .reset_reset_n                   (KEY[0]),
    
    // Clarvi Debug
    //.core_0_debug_export                   (HEX0),
    //.core_1_debug_export                   (HEX1),
    //.core_2_debug_export                   (HEX2),
    //.core_3_debug_export                   (HEX3),
    //.core_4_debug_export                   (HEX4),
    //.core_5_debug_export                   (HEX5),
    
    // PixelStream
    .pixelstream_0_conduit_end_0_lcd_red   (LCD_R),
    .pixelstream_0_conduit_end_0_lcd_green (LCD_G),
    .pixelstream_0_conduit_end_0_lcd_blue  (LCD_B),
    .pixelstream_0_conduit_end_0_lcd_hsync (LCD_HSYNC),
    .pixelstream_0_conduit_end_0_lcd_vsync (vsync),
    .pixelstream_0_conduit_end_0_lcd_de    (LCD_DEN),
    .pixelstream_0_conduit_end_0_lcd_dclk  (LCD_DCLK),
    
    // Ethernet
    .hps_io_hps_io_emac1_inst_TX_CLK (HPS_ENET_GTX_CLK),
    .hps_io_hps_io_emac1_inst_TXD0   (HPS_ENET_TX_DATA[0]),
    .hps_io_hps_io_emac1_inst_TXD1   (HPS_ENET_TX_DATA[1]),
    .hps_io_hps_io_emac1_inst_TXD2   (HPS_ENET_TX_DATA[2]),
    .hps_io_hps_io_emac1_inst_TXD3   (HPS_ENET_TX_DATA[3]),
    .hps_io_hps_io_emac1_inst_RXD0   (HPS_ENET_RX_DATA[0]),
    .hps_io_hps_io_emac1_inst_MDIO   (HPS_ENET_MDIO),
    .hps_io_hps_io_emac1_inst_MDC    (HPS_ENET_MDC),
    .hps_io_hps_io_emac1_inst_RX_CTL (HPS_ENET_RX_DV),
    .hps_io_hps_io_emac1_inst_TX_CTL (HPS_ENET_TX_EN),
    .hps_io_hps_io_emac1_inst_RX_CLK (HPS_ENET_RX_CLK),
    .hps_io_hps_io_emac1_inst_RXD1   (HPS_ENET_RX_DATA[1]),
    .hps_io_hps_io_emac1_inst_RXD2   (HPS_ENET_RX_DATA[2]),
    .hps_io_hps_io_emac1_inst_RXD3   (HPS_ENET_RX_DATA[3]),
      
    // Flash
    .hps_io_hps_io_qspi_inst_IO0     (HPS_FLASH_DATA[0]),
    .hps_io_hps_io_qspi_inst_IO1     (HPS_FLASH_DATA[1]),
    .hps_io_hps_io_qspi_inst_IO2     (HPS_FLASH_DATA[2]),
    .hps_io_hps_io_qspi_inst_IO3     (HPS_FLASH_DATA[3]),
    .hps_io_hps_io_qspi_inst_SS0     (HPS_FLASH_NCSO),
    .hps_io_hps_io_qspi_inst_CLK     (HPS_FLASH_DCLK),
    
    // SD Card
    .hps_io_hps_io_sdio_inst_CMD     (HPS_SD_CMD),
    .hps_io_hps_io_sdio_inst_D0      (HPS_SD_DATA[0]),
    .hps_io_hps_io_sdio_inst_D1      (HPS_SD_DATA[1]),
    .hps_io_hps_io_sdio_inst_CLK     (HPS_SD_CLK),
    .hps_io_hps_io_sdio_inst_D2      (HPS_SD_DATA[2]),
    .hps_io_hps_io_sdio_inst_D3      (HPS_SD_DATA[3]),
    
    // USB
    .hps_io_hps_io_usb1_inst_D0      (HPS_USB_DATA[0]),
    .hps_io_hps_io_usb1_inst_D1      (HPS_USB_DATA[1]),
    .hps_io_hps_io_usb1_inst_D2      (HPS_USB_DATA[2]),
    .hps_io_hps_io_usb1_inst_D3      (HPS_USB_DATA[3]),
    .hps_io_hps_io_usb1_inst_D4      (HPS_USB_DATA[4]),
    .hps_io_hps_io_usb1_inst_D5      (HPS_USB_DATA[5]),
    .hps_io_hps_io_usb1_inst_D6      (HPS_USB_DATA[6]),
    .hps_io_hps_io_usb1_inst_D7      (HPS_USB_DATA[7]),
    .hps_io_hps_io_usb1_inst_CLK     (HPS_USB_CLKOUT),
    .hps_io_hps_io_usb1_inst_STP     (HPS_USB_STP),
    .hps_io_hps_io_usb1_inst_DIR     (HPS_USB_DIR),
    .hps_io_hps_io_usb1_inst_NXT     (HPS_USB_NXT),
    
    // SPIM
    .hps_io_hps_io_spim1_inst_CLK    (HPS_SPIM_CLK),
    .hps_io_hps_io_spim1_inst_MOSI   (HPS_SPIM_MOSI),
    .hps_io_hps_io_spim1_inst_MISO   (HPS_SPIM_MISO),
    .hps_io_hps_io_spim1_inst_SS0    (HPS_SPIM_SS),
    
    // UART
    .hps_io_hps_io_uart0_inst_RX     (HPS_UART_RX),
    .hps_io_hps_io_uart0_inst_TX     (HPS_UART_TX),
    
    // I2C
    .hps_io_hps_io_i2c0_inst_SDA     (HPS_I2C1_SDAT),
    .hps_io_hps_io_i2c0_inst_SCL     (HPS_I2C1_SCLK),
    .hps_io_hps_io_i2c1_inst_SDA     (HPS_I2C2_SDAT),
    .hps_io_hps_io_i2c1_inst_SCL     (HPS_I2C2_SCLK),
    
    // GPIO
    .hps_io_hps_io_gpio_inst_GPIO09  (HPS_CONV_USB_N),
    .hps_io_hps_io_gpio_inst_GPIO35  (HPS_ENET_INT_N),
    .hps_io_hps_io_gpio_inst_GPIO40  (HPS_LTC_GPIO),
    .hps_io_hps_io_gpio_inst_GPIO48  (HPS_I2C_CONTROL),
    .hps_io_hps_io_gpio_inst_GPIO53  (HPS_LED),
    .hps_io_hps_io_gpio_inst_GPIO54  (HPS_KEY),
    .hps_io_hps_io_gpio_inst_GPIO61  (HPS_GSENSOR_INT),

    // DDR3 Memory
    .memory_mem_a                    (HPS_DDR3_ADDR),
    .memory_mem_ba                   (HPS_DDR3_BA),
    .memory_mem_ck                   (HPS_DDR3_CK_P),
    .memory_mem_ck_n                 (HPS_DDR3_CK_N),
    .memory_mem_cke                  (HPS_DDR3_CKE),
    .memory_mem_cs_n                 (HPS_DDR3_CS_N),
    .memory_mem_ras_n                (HPS_DDR3_RAS_N),
    .memory_mem_cas_n                (HPS_DDR3_CAS_N),
    .memory_mem_we_n                 (HPS_DDR3_WE_N),
    .memory_mem_reset_n              (HPS_DDR3_RESET_N),
    .memory_mem_dq                   (HPS_DDR3_DQ),
    .memory_mem_dqs                  (HPS_DDR3_DQS_P),
    .memory_mem_dqs_n                (HPS_DDR3_DQS_N),
    .memory_mem_odt                  (HPS_DDR3_ODT),
    .memory_mem_dm                   (HPS_DDR3_DM),
    .memory_oct_rzqin                (HPS_DDR3_RZQ)
);



endmodule

  