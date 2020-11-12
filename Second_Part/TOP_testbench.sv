/*
*Testbench created automatically with a program written in Python 3.8 by:
*	García Vidal Jorge Alberto
*	Guevara Zavala Arturo
*	Morales Hurtado David Xchel
*	Rodriguez Contreras Luis Fernando
*
*For the first project in the class of professor:
*	Carolina Rosas Huerta
*
*In the Silicon Verification Program
*/

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module TOP_TB;

//Signal instantiation
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

initial
	begin
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_TB);

		//Initializing values
		wireclk_TB = 1'b0;
		rst_TB = 1'b0;
		signal_TB = 16'b0;

		//The program will iterate 10 times
		//Iteration: 1
		#1
		wireclk_TB = 1'd0;
		rst_TB = 1'd0;
		signal_TB = 16'd8408;

		//Iteration: 2
		#1
		wireclk_TB = 1'd1;
		rst_TB = 1'd1;
		signal_TB = 16'd8409;

		//Iteration: 3
		#1
		wireclk_TB = 1'd0;
		rst_TB = 1'd0;
		signal_TB = 16'd8410;

		//Iteration: 4
		#1
		wireclk_TB = 1'd1;
		rst_TB = 1'd1;
		signal_TB = 16'd8411;

		//Iteration: 5
		#1
		wireclk_TB = 1'd0;
		rst_TB = 1'd0;
		signal_TB = 16'd8412;

		//Iteration: 6
		#1
		wireclk_TB = 1'd1;
		rst_TB = 1'd1;
		signal_TB = 16'd8413;

		//Iteration: 7
		#1
		wireclk_TB = 1'd0;
		rst_TB = 1'd0;
		signal_TB = 16'd8414;

		//Iteration: 8
		#1
		wireclk_TB = 1'd1;
		rst_TB = 1'd1;
		signal_TB = 16'd8415;

		//Iteration: 9
		#1
		wireclk_TB = 1'd0;
		rst_TB = 1'd0;
		signal_TB = 16'd8416;

		//Iteration: 10
		#1
		wireclk_TB = 1'd1;
		rst_TB = 1'd1;
		signal_TB = 16'd8417;

		$finish;
	end
endmodule