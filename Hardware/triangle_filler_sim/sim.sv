module sim ();

logic clk = 0;
logic reset = 0;

initial begin
	forever #5 clk = ~clk;
end

logic [8:0] x1;
logic [8:0] x2;
logic [8:0] y1;
logic [8:0] y2;
logic [8:0] x3;
logic [8:0] y3;
logic start;
logic done;

triangle_filler dut( .clk, .reset, .x1, .x2, .y1, .y2, .x3, .y3, .start, .done);

initial begin
	reset = 1;
	#20 reset = 0;
	#30 x1 = 5;
	x2 = 12;
	x3 = 8;
	y1 = 4;
	y2 = 8;
	y3 = 10;
	start = 1;
	#10 start = 0;
	@(posedge done) // Wait for done to be asserted
	$display("done!");
	#30 x1 = 5;
	x2 = 12;
	x3 = 8;
	y1 = 4;
	y2 = 8;
	y3 = 10;
	start = 1;
	#10 start = 0;
	@(posedge done) // Wait for done to be asserted
	$display("done!");
end
endmodule
