//time scale
`timescale 1ns/1ps
//Main Testbench Starts here
module Logic_mux2_TB;
reg a_TB;
reg b_TB;
reg c_TB;
wire y_TB;

Logic_mux2 UUT(.a(a_TB), .b(b_TB), .c(c_TB), .y(y_TB));

initial
	begin
		$dumpfile("Logic_mux2.vcd");
		$dumpvars(1, Logic_mux2_TB);

		a_TB = 1'b0;
		b_TB = 1'b0;
		c_TB = 1'b0;

		#1
		a_TB = 1'd0;
		b_TB = 1'd1;
		c_TB = 1'd0;

		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd1;

		#1
		a_TB = 1'd0;
		b_TB = 1'd1;
		c_TB = 1'd0;

		#1
		$finish;
	end
endmodule