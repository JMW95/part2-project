module palette (
    input clk,
    input reset,
    
    input avs_slave_read,
    input [23:0] avs_slave_address,
    output logic avs_slave_waitrequest,
    output logic [31:0] avs_slave_readdata,
    output logic avs_slave_readdatavalid,
    
    output logic avm_master_read,
    output logic [23:0] avm_master_address,
    input logic [7:0] avm_master_readdata,
    input logic avm_master_readdatavalid,
    input logic avm_master_waitrequest,
    
    input [3:0] avs_palette_address,
    input [15:0] avs_palette_writedata,
    output logic [15:0] avs_palette_readdata,
    input avs_palette_write
);

function logic [15:0] PIXEL16(int r, int g, int b);
    return ((r & 8'h1F)<<11) | ((g & 8'h3F)<<5) | ((b & 8'h1F)<<0);
endfunction

function logic [15:0] PIXEL24(int r, int g, int b);
    return PIXEL16((r>>3), (g>>2), (b>>3));
endfunction

`define PIXEL_WHITE     PIXEL24(8'hFF, 8'hFF, 8'hFF)
`define PIXEL_BLACK     PIXEL24(8'h00, 8'h00, 8'h00)
`define PIXEL_RED       PIXEL24(8'hFF, 8'h00, 8'h00)
`define PIXEL_GREEN     PIXEL24(8'h00, 8'hFF, 8'h00)
`define PIXEL_BLUE      PIXEL24(8'h00, 8'h00, 8'hFF)
`define PIXEL_YELLOW    PIXEL24(8'hFF, 8'hFF, 8'h00)
`define PIXEL_CYAN      PIXEL24(8'h00, 8'hFF, 8'hFF)
`define PIXEL_MAGENTA   PIXEL24(8'hFF, 8'h00, 8'hFF)

logic [15:0] palette[16] = '{
    `PIXEL_BLACK,
    `PIXEL_WHITE,
    `PIXEL_RED,
    `PIXEL_GREEN,
    `PIXEL_BLUE,
    `PIXEL_CYAN,
    `PIXEL_MAGENTA,
    `PIXEL_YELLOW,
    `PIXEL_BLACK,
    `PIXEL_WHITE,
    `PIXEL_RED,
    `PIXEL_GREEN,
    `PIXEL_BLUE,
    `PIXEL_CYAN,
    `PIXEL_MAGENTA,
    `PIXEL_YELLOW
};

logic [3:0] pix1, pix2;
logic [23:0] striped_address;

always_comb begin
    striped_address[23:16] = avs_slave_address[23:16];
    if(avs_slave_address[15:0] >= 16'hbf40) begin
        striped_address[15:0] = avs_slave_address[15:0] + 16'hC0;
    end else if(avs_slave_address[15:0] >= 16'h7f80) begin
        striped_address[15:0] = avs_slave_address[15:0] + 16'h80;
    end else if(avs_slave_address[15:0] >= 16'h3fc0) begin
        striped_address[15:0] = avs_slave_address[15:0] + 16'h40;
    end else begin
        striped_address[15:0] = avs_slave_address[15:0];
    end
end

always_ff @(posedge clk) begin
    if(reset) begin
        avs_slave_readdata <= '0;
        avs_slave_readdatavalid <= 0;
        avm_master_read <= 0;
        avm_master_address <= '0;
        avs_slave_waitrequest <= 0;
    end else begin
    
        if (avs_palette_write) begin
            palette[avs_palette_address] <= avs_palette_writedata;
        end
        avs_palette_readdata <= palette[avs_palette_address];
    
        avs_slave_waitrequest <= avm_master_waitrequest;
        
        avm_master_read <= avs_slave_read;
        avm_master_address <= striped_address;
        avs_slave_readdatavalid <= avm_master_readdatavalid;
        
        if (avm_master_readdatavalid) begin
            {pix1, pix2} = avm_master_readdata;
            avs_slave_readdata = { palette[pix1], palette[pix2] };
        end
        
    end
end

endmodule
