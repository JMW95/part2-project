module palette #(
    parameter NUM_CORES = 8
)(
    input clk,
    input reset,
    
    input avs_slave_read,
    input [23:0] avs_slave_address,
    output logic avs_slave_waitrequest,
    output logic [31:0] avs_slave_readdata,
    output logic avs_slave_readdatavalid,
    
    output logic avm_master_read,
    output logic [23:0] avm_master_address,
    input logic [15:0] avm_master_readdata,
    input logic avm_master_readdatavalid,
    input logic avm_master_waitrequest,
    
    input [7:0] avs_palette_address,
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

logic [15:0] palette[256] = '{
    0: `PIXEL_BLACK,
    1: `PIXEL_WHITE,
    2: `PIXEL_RED,
    3: `PIXEL_GREEN,
    4: `PIXEL_BLUE,
    5: `PIXEL_CYAN,
    6: `PIXEL_MAGENTA,
    7: `PIXEL_YELLOW,
    default: `PIXEL_BLACK
};

logic [7:0] pix1, pix2;
logic valid;
logic [23:0] striped_address;

generate
    if (NUM_CORES == 8) begin
        always_comb begin // [16:1] <= [15:0] changes from 8-16bit alignment
            striped_address[23:17] = avs_slave_address[23:17];
            striped_address[0] = 0;
            if(avs_slave_address[15:0] >= 16'hdf20) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'he0;
            end else if(avs_slave_address[15:0] >= 16'hbf40) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'hc0;
            end else if(avs_slave_address[15:0] >= 16'h9f60) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'ha0;
            end else if(avs_slave_address[15:0] >= 16'h7f80) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h80;
            end else if(avs_slave_address[15:0] >= 16'h5fa0) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h60;
            end else if(avs_slave_address[15:0] >= 16'h3fc0) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h40;
            end else if(avs_slave_address[15:0] >= 16'h1fe0) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h20;
            end else begin
                striped_address[16:1] = avs_slave_address[15:0];
            end
        end
    end
    if (NUM_CORES == 4) begin
        always_comb begin // [16:1] <= [15:0] changes from 8-16bit alignment
            striped_address[23:17] = avs_slave_address[23:17];
            striped_address[0] = 0;
            if(avs_slave_address[15:0] >= 16'hbf40) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'hc0;
            end else if(avs_slave_address[15:0] >= 16'h7f80) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h80;
            end else if(avs_slave_address[15:0] >= 16'h3fc0) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h40;
            end else begin
                striped_address[16:1] = avs_slave_address[15:0];
            end
        end
    end
    if (NUM_CORES == 2) begin
        always_comb begin // [16:1] <= [15:0] changes from 8-16bit alignment
            striped_address[23:17] = avs_slave_address[23:17];
            striped_address[0] = 0;
            if(avs_slave_address[15:0] >= 16'h7f80) begin
                striped_address[16:1] = avs_slave_address[15:0] + 16'h80;
            end else begin
                striped_address[16:1] = avs_slave_address[15:0];
            end
        end
    end
    if (NUM_CORES == 1) begin
        always_comb begin // [16:1] <= [15:0] changes from 8-16bit alignment
            striped_address[23:17] = avs_slave_address[23:17];
            striped_address[16:1] = avs_slave_address[15:0];
            striped_address[0] = 0;
        end
    end
endgenerate

always_ff @(posedge clk) begin
    if(reset) begin
        avs_slave_readdata <= '0;
        avs_slave_readdatavalid <= 0;
        avm_master_read <= 0;
        avm_master_address <= '0;
        avs_slave_waitrequest <= 0;
        pix1 <= '0;
        pix2 <= '0;
        valid <= 0;
    end else begin
    
        if (avs_palette_write) begin
            palette[avs_palette_address] <= avs_palette_writedata;
        end
        //avs_palette_readdata <= palette[avs_palette_address];
        avs_palette_readdata <= '0;
    
        avs_slave_waitrequest <= avm_master_waitrequest;
        
        avm_master_read <= avs_slave_read;
        avm_master_address <= striped_address;
        
        if (avm_master_readdatavalid) begin
            pix1 <= avm_master_readdata[7:0];
            pix2 <= avm_master_readdata[15:8];
            valid <= 1;
        end else begin
            valid <= 0;
        end

        if (valid) begin
            avs_slave_readdata <= { palette[pix1], palette[pix2] };
            avs_slave_readdatavalid <= 1;
        end else begin
            avs_slave_readdatavalid <= 0;
        end
        
    end
end

endmodule
