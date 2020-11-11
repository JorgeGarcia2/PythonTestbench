//time scale
`timescale 1ns/1ps
//Main Testbench Starts here
module TOP_TB;
reg rst_TB;
reg wireclk_TB;
reg [15:0]signal_TB;
wire [0:31]PC_Out_TB;
wire [0:31]Imemo_Inst_TB;
wire [0:31]RAM_Rw_TB;
wire [0:31]RAM_R1_TB;
wire [0:31]RAM_R2_TB;
wire ALU_Flag_TB;

TOP UUT(.rst(rst_TB), .wireclk(wireclk_TB), .signal(signal_TB), .PC_Out(PC_Out_TB), .Imemo_Inst(Imemo_Inst_TB), .RAM_Rw(RAM_Rw_TB), .RAM_R1(RAM_R1_TB), .RAM_R2(RAM_R2_TB), .ALU_Flag(ALU_Flag_TB));

initial
	begin
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_TB);

		rst_TB = 1;
		wireclk_TB = 1'b0;
		signal_TB = 16'b0;

		#2
		rst_TB = 0;

		#1
		wireclk_TB = 1'd0;
		signal_TB = 16'd1;

		#1
		wireclk_TB = 1'd0;
		signal_TB = 16'd2;

		#1
		wireclk_TB = 1'd0;
		signal_TB = 16'd3;

		#1
		wireclk_TB = 1'd0;
		signal_TB = 16'd4;

		#1
		wireclk_TB = 1'd0;
		signal_TB = 16'd5;

		#1
		$finish;
	end
endmodule