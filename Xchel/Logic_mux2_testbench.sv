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
module Logic_mux2_TB();

	//Clock and reset signal instantiation as registers

	//Instantiation of other input signals
	reg a_TB;
	reg b_TB;
	reg c_TB;

	//Instantiation of output signals
	wire y_TB;

	//Instantiation of the Logic_mux2 module
	Logic_mux2 UUT(.a(a_TB), .b(b_TB), .c(c_TB), .y(y_TB));

	//Initialization of the testing values
	initial
	begin
		//File and variables to simulate
		$dumpfile("Logic_mux2.vcd");
		$dumpvars(1, Logic_mux2_TB);

		//Initial value of the input signals with reset on
		a_TB = 1'b0;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Turning off reset signal for test begining
		//The testbench will iterate over 10 cycles
		//Iteration: 1
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b1;

		//Iteration: 2
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Iteration: 3
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b1;

		//Iteration: 4
		#1
		a_TB = 1'b0;
		b_TB = 1'b1;
		c_TB = 1'b0;

		//Iteration: 5
		#1
		a_TB = 1'b0;
		b_TB = 1'b1;
		c_TB = 1'b0;

		//Iteration: 6
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Iteration: 7
		#1
		a_TB = 1'b0;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Iteration: 8
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Iteration: 9
		#1
		a_TB = 1'b1;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//Iteration: 10
		#1
		a_TB = 1'b0;
		b_TB = 1'b1;
		c_TB = 1'b0;


		//Ending iteration
		#1
		$finish;
	end
endmodule