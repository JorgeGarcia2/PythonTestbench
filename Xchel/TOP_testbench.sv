//Testbench created automatically with a program written in Python 3.8 by:
//	García Vidal Jorge Alberto
//	Guevara Zavala Arturo
//	Morales Hurtado David Xchel
//	Rodriguez Contreras Luis Fernando
//
//For the first project in the class of professor:
//	Carolina Rosas Huerta
//
//In the Silicon Verification Program
//
//And modified according to the criteria of:
//	David Xchel Morales Hurtado

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module TOP_TB();

	//Clock and reset signal instantiation as registers
	reg clk_TB;
	reg rst_TB;

	//Instantiation of other input signals
	reg clock_TB;
	reg rts_TB;
	reg [0:6]halo_TB;
	reg [0:6]hola_TB;

	//Instantiation of output signals
	wire [0:31]PC_Out_TB[0:5];
	wire [0:31]Imemo_Inst_TB[0:5];
	wire [0:31]RAM_Rw_TB[0:5];
	wire [0:31]RAM_R1_TB[0:5];
	wire [0:31]RAM_R2_TB;
	wire ALU_Flag_TB[0:5][0:7];

	//Instantiation of the TOP module
	TOP UUT(.clk(clk_TB), .rst(rst_TB), .clock(clock_TB), .rts(rts_TB), .halo(halo_TB), .hola(hola_TB), .PC_Out(PC_Out_TB), .Imemo_Inst(Imemo_Inst_TB), .RAM_Rw(RAM_Rw_TB), .RAM_R1(RAM_R1_TB), .RAM_R2(RAM_R2_TB), .ALU_Flag(ALU_Flag_TB));

	//Inicialization of the clock signal
	always forever #1 clk_TB = ~clk_TB;

	//Initialization of the testing values
	initial
	begin
		//File and variables to simulate
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_TB);

		//Initial value of the input signals with reset on
		clk_TB = 0;
		rst_TB = 1;
		clock_TB = 1'b0;
		rts_TB = 1'b0;
		halo_TB = 7'b0;
		hola_TB = 7'b0;

		//Turning off reset signal for test begining
		#2
		rst_TB = 0;

		//The testbench will iterate over 10 cycles
		//Iteration: 1
		#1
		clock_TB = 1'b0;
		rts_TB = 1'b1;
		halo_TB = 7'b100000;
		hola_TB = 7'b101110;

		//Iteration: 2
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b1;
		halo_TB = 7'b100011;
		hola_TB = 7'b10;

		//Iteration: 3
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b1;
		halo_TB = 7'b1100;
		hola_TB = 7'b11111;

		//Iteration: 4
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b1;
		halo_TB = 7'b11001;
		hola_TB = 7'b101111;

		//Iteration: 5
		#1
		clock_TB = 1'b0;
		rts_TB = 1'b0;
		halo_TB = 7'b101110;
		hola_TB = 7'b110110;

		//Iteration: 6
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b0;
		halo_TB = 7'b111110;
		hola_TB = 7'b110100;

		//Iteration: 7
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b0;
		halo_TB = 7'b111100;
		hola_TB = 7'b101111;

		//Iteration: 8
		#1
		clock_TB = 1'b0;
		rts_TB = 1'b1;
		halo_TB = 7'b110001;
		hola_TB = 7'b1101;

		//Iteration: 9
		#1
		clock_TB = 1'b0;
		rts_TB = 1'b0;
		halo_TB = 7'b110101;
		hola_TB = 7'b101110;

		//Iteration: 10
		#1
		clock_TB = 1'b1;
		rts_TB = 1'b0;
		halo_TB = 7'b101100;
		hola_TB = 7'b1011;


		//Ending iteration
		#1
		$finish;
	end
endmodule