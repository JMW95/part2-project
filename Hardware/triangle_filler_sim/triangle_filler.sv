module triangle_filler (
    input logic clk,
    input logic reset,
    
    input logic [8:0] x1,
    input logic [8:0] y1,
    input logic [8:0] x2,
    input logic [8:0] y2,
    input logic [8:0] x3,
    input logic [8:0] y3,
    
    input logic start,
    
    output logic done
);

logic start1, start2, start3;
logic done1, done2, done3;
logic ack1, ack2, ack3;
logic valid1, valid2, valid3;
logic [8:0] xout1, xout2, xout3;
logic [8:0] yout;

logic [2:0] state;

// We assume points 1-3 are sorted so that y1 lowest, y2 is middle, y3 is highest.

// B1 runs from 1 to 3
bresenhams b1(.clk, .reset, .x1(x1), .y1(y1), .x2(x3), .y2(y3), .done(done1), .ack(ack1), .x(xout1), .y(yout), .valid(valid1), .start(start1));
// B2 runs from 1 to 2
bresenhams b2(.clk, .reset, .x1(x1), .y1(y1), .x2(x2), .y2(y2), .done(done2), .ack(ack2), .x(xout2), .y(), .valid(valid2), .start(start2));
// B3 runs from 2 to 3
bresenhams b3(.clk, .reset, .x1(x2), .y1(y2), .x2(x3), .y2(y3), .done(done3), .ack(ack3), .x(xout3), .y(), .valid(valid3), .start(start3));

always_ff @(posedge clk) begin
    if (reset) begin
        start1 <= 0;
        start2 <= 0;
        start3 <= 0;
        ack1 <= 0;
        ack2 <= 0;
        ack3 <= 0;
        done <= 0;
        state <= 0;
    end else begin
        if (state == 0) begin // Set off the first 2 runnners
            if (start) begin
                start1 <= 1;
                start2 <= 1;
                ack1 <= 0;
                ack2 <= 0;
                done <= 0;
                state <= 1;
            end
        end else if (state == 1) begin // Starting 1 and 2
			start1 <= 0;
			start2 <= 0;
			state <= 2;
		end else if (state == 2) begin // Running 1 and 2
			if (valid1 && valid2) begin // Wait for both to become valid
                ack1 <= 1;
                ack2 <= 1;
                if(!ack1 && !ack2) begin
					// TODO Output xout1, xout2, yout to line_filler
					$display("Fill: x=%d to %d, y=%d", xout1, xout2, yout);
				end
            end else begin
                ack1 <= 0;
                ack2 <= 0;
            end
            if(done2) begin
                start3 <= 1; // Set off the third runner
                ack1 <= 0;
                ack3 <= 0;
                state <= 3;
            end
		end else if (state == 3) begin // Starting 3
			start3 <= 0;
			state <= 4;
        end else if (state == 4) begin // Running 1 and 3
            if (valid1 && valid3) begin // Wait for both to become valid
                ack1 <= 1;
                ack3 <= 1;
                if (!ack1 && !ack3) begin
					// TODO Output xout1, xout2, yout to line_filler
					$display("Fill: x=%d to %d, y=%d", xout1, xout3, yout);
				end
            end else begin
                ack1 <= 0;
                ack3 <= 0;
            end
            if(done1 || done3) begin // TODO: AND or OR?
                // Advance to next stage?
                done <= 1;
                state <= 0;
            end
        end
    end
end

endmodule
