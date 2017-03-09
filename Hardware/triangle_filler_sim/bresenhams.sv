module bresenhams (
    input logic clk,
    input logic reset,
    input logic [8:0] x1,
    input logic [8:0] y1,
    input logic [8:0] x2,
    input logic [8:0] y2,
    input logic start,
    input logic ack,
    
    output logic [8:0] x,
    output logic [8:0] y,
    output logic valid,
    output logic done
);

logic [1:0] state;
logic [8:0] cx;
logic [8:0] cy;
logic [8:0] dx;
logic [8:0] dy;
logic [8:0] m;
logic cxinc;
logic [8:0] e;

function logic[8:0] abs(logic [8:0] val);
    if (val[8]) begin
        return -val;
    end else begin
        return val;
    end
endfunction

always_ff @(posedge clk) begin
    if (reset) begin
        x <= '0;
        y <= '0;
        valid <= 0;
        done <= 0;
        state <= 0;
        cx <= '0;
        cy <= '0;
        cxinc <= 0;
        e <= 0;
    end else begin
        if (state == 0) begin // Wait for coords
            if (start) begin
                cx <= x1;
                cy <= y1;
                dx = x2 - x1;
                dy <= y2 - y1;
                m = abs(dx);
                if (x2 > x1) begin
                    cxinc <= 1;
                    e <= -1;
                end else begin
                    cxinc <= 0; // -1
                    e <= 0;
                end
                done <= 0;
                state <= 1;
            end
        end else if (state == 1) begin // Compute next point
            if (cy == y2) begin
                state <= 0;
                done <= 1;
            end else begin
                if($signed(e) >= 0) begin
                    cx <= cx + (cxinc == 1 ? 1 : -1);
                    e <= e - dy;
                end else begin
                    x <= cx;
                    y <= cy;
                    valid <= 1;
                    
                    e <= e + m;
                    cy <= cy + 1;
                    
                    state <= 2;
                end
            end
        end else if (state == 2) begin // Wait for ACK
            if (ack) begin
                valid <= 0;
                state <= 1;
            end
        end
    end
end


endmodule
