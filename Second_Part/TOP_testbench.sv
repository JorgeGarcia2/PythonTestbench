//time scale
`timescale 1ns/1ps
//Main Testbench Starts here
module TOP_TB;
reg wireclk_TB;
reg rst_TB;
reg [15:0]signal_TB;
wire [0:31]PC_Out_TB;
wire [0:31]Imemo_Inst_TB;
wire [0:31]RAM_Rw_TB;
wire [0:31]RAM_R1_TB;
wire [0:31]RAM_R2_TB;
wire ALU_Flag_TB;

TOP UUT(.wireclk(wireclk_TB), .rst(rst_TB), .signal(signal_TB), .PC_Out(PC_Out_TB), .Imemo_Inst(Imemo_Inst_TB), .RAM_Rw(RAM_Rw_TB), .RAM_R1(RAM_R1_TB), .RAM_R2(RAM_R2_TB), .ALU_Flag(ALU_Flag_TB));

	always forever #1 wireclk_TB = ~wireclk_TB;

initial
	begin
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_TB);

		wireclk_TB = 0;
		rst_TB = 1;
		signal_TB = 16'b0;

		#2
		rst_TB = 0;

		#1
		signal_TB = 16'd10;

		#1
		signal_TB = 16'd12;

		#1
		signal_TB = 16'd14;

		#1
		$finish;
	end
endmodule