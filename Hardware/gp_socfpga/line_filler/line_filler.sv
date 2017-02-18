module line_filler (
    input clk,
    input reset,
    
    output logic avm_fbuff_write,
    output logic [15:0] avm_fbuff_address,
    output logic [31:0] avm_fbuff_writedata,
    output logic [3:0] avm_fbuff_byteenable,
    input logic avm_fbuff_waitrequest,
    
    input logic avs_slave_write,
    input logic [16:0] avs_slave_address,
    input logic [31:0] avs_slave_writedata,
    input logic [3:0] avs_slave_byteenable,
    output logic avs_slave_waitrequest
);

typedef struct packed {
    logic [8:0] ycoord;
    logic [8:0] left;
    logic [8:0] right;
    logic [7:0] colour;
    logic buffernum;
    logic valid;
} operation_t;

// Forward declaration of state
logic state;

/*
 * WRITE THROUGH
 *
 * Allows the host to write directly through the filler to the framebuffer.
 */
logic write_through;
logic wt_write;
logic [15:0] wt_address;
logic [31:0] wt_writedata;
logic [3:0] wt_byteenable;
logic wt_waitrequest;

always_comb begin
    write_through = avs_slave_address[16];
end

always_ff @(posedge clk) begin
    if(reset) begin
        wt_write <= 0;
        wt_address <= '0;
        wt_writedata <= '0;
        wt_waitrequest <= 0;
        wt_byteenable <= '0;
    end else begin
        if(write_through && avs_slave_write) begin
            wt_address <= avs_slave_address[15:0];
            wt_writedata <= avs_slave_writedata;
            wt_byteenable <= avs_slave_byteenable;
            wt_waitrequest <= avm_fbuff_waitrequest;
            wt_write <= 1;
        end else begin
            wt_write <= 0;
        end
    end
end


/*
 * DECODE
 * Reads a value from the input.
 */
operation_t pending_op;
operation_t de_ex_op;
logic de_waitrequest;

always_ff @(posedge clk) begin
    if(reset) begin
        pending_op <= '0;
        de_ex_op <= '0;
    end else begin
        de_waitrequest <= pending_op.valid; // Force to wait if there's already a pending operation
        if(avs_slave_write && !write_through) begin
            // Read a pending instruction
            if(!pending_op.valid) begin // Don't already have a pending operation
                pending_op.ycoord <= avs_slave_address[8:0];
                pending_op.buffernum <= avs_slave_address[15];
                pending_op.left <= avs_slave_writedata[8:0];
                pending_op.right <= avs_slave_writedata[17:9];
                pending_op.colour <= avs_slave_writedata[25:18];
                pending_op.valid <= 1;
            end
        end
        // If no operation being executed, and one is pending
        if(state == 0) begin
            if(pending_op.valid) begin
                // Copy in our pending instruction so it can be executing
                de_ex_op.left <= pending_op.left;
                de_ex_op.right <= pending_op.right;
                de_ex_op.ycoord <= pending_op.ycoord;
                de_ex_op.buffernum <= pending_op.buffernum;
                de_ex_op.colour <= pending_op.colour;
                de_ex_op.valid <= 1;
                pending_op.valid <= 0;
            end else begin
                de_ex_op.valid <= 0;
            end
        end
    end
end


/*
 * EXECUTE
 * Draws a line between the points in de_ex_op.
 * An operation will stay in this stage for many cycles.
 */
logic [8:0] curr;
logic [16:0] yoffset;
logic done;
logic fill_write;
logic [22:0] fill_address;
logic [31:0] fill_writedata;
logic [3:0] fill_byteenable;

always_comb begin
    done = curr > de_ex_op.right;
end

function logic [3:0] compute_byteenable(logic [1:0] align);
    case(align)
        0: return 4'b1111;
        1: return 4'b1110;
        2: return 4'b1100;
        3: return 4'b1000;
    endcase
endfunction

// We always fill in the pixel at 'right', so fill in one more byte than obvious
function logic [3:0] compute_bytemask(logic [16:0] remaining);
    if(remaining >= 3) return 4'b1111;
    case(remaining)
        0: return 4'b0001;
        1: return 4'b0011;
        2: return 4'b0111;
    endcase
endfunction

always_ff @(posedge clk) begin
    if(reset) begin
        curr <= '0;
        yoffset <= '0;
        state <= 0;
    end else begin
        if (state == 0) begin // INITIALISE
            // If we have something to execute
            if(de_ex_op.valid) begin
                curr <= de_ex_op.left;
                yoffset <= de_ex_op.ycoord * 480;
                state <= 1;
            end
        end else if (state == 1) begin // FILL
            if (done) begin
                fill_write <= 0;
                state <= 0;
            end else begin
                if(!write_through) begin
                    fill_address <= (de_ex_op.buffernum << 15) | (yoffset[16:2] + curr[8:2]); // 32-bit align
                    fill_writedata <= {4{de_ex_op.colour}};
                    fill_byteenable <= compute_byteenable(curr[1:0]) & (compute_bytemask(de_ex_op.right - curr) << curr[1:0]);
                    fill_write <= 1;
                    curr <= {{curr[8:2] + 1}, 2'b00}; // Advance to the next set of pixels
                end
            end
        end
    end
end

/*
 * MEMORY
 */

always_comb begin
    avm_fbuff_write = wt_write || fill_write;
    avm_fbuff_address = wt_write ? wt_address : fill_address;
    avm_fbuff_byteenable = wt_write ? wt_byteenable : fill_byteenable;
    avm_fbuff_writedata = wt_write ? wt_writedata : fill_writedata;
    avs_slave_waitrequest = wt_waitrequest || de_waitrequest;
end

endmodule
