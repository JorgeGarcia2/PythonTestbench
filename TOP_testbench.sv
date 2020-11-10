//time scale
`timescale 1ns/1ps
//Main Testbench Starts here
module TOP_tb;
wire [31:0]A_tb;
wire B_tb;
wire C_tb;
reg X_tb;
reg [7:0]Y_tb;
reg [0:5]Z_tb;

TOP UUT(.A(A_tb), .B(B_tb), .C(C_tb), .X(X_tb), .Y(Y_tb), .Z(Z_tb));

initial
	begin
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_tb.vcd);

		A_tb = 32'b0';
		B_tb = 1'b0';
		C_tb = 1'b0';

		#1
		A_tb = 32'd0;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd0;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd0;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd0;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		A_tb = 32'd1;
		B_tb = 1'd;
		C_tb = 1'd;

		#1
		$finish;
	end
endmodule