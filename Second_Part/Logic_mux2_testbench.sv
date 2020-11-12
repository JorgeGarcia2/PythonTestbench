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
module Logic_mux2_TB;

//Signal instantiation
reg a_TB;
reg b_TB;
reg c_TB;
wire y_TB;

Logic_mux2 UUT(.a(a_TB), .b(b_TB), .c(c_TB), .y(y_TB));

initial
	begin
		$dumpfile("Logic_mux2.vcd");
		$dumpvars(1, Logic_mux2_TB);

		//Initializing values
		a_TB = 1'b0;
		b_TB = 1'b0;
		c_TB = 1'b0;

		//The program will iterate 10 times
		//Iteration: 1
		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd0;

		//Iteration: 2
		#1
		a_TB = 1'd0;
		b_TB = 1'd0;
		c_TB = 1'd1;

		//Iteration: 3
		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd0;

		//Iteration: 4
		#1
		a_TB = 1'd0;
		b_TB = 1'd0;
		c_TB = 1'd1;

		//Iteration: 5
		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd0;

		//Iteration: 6
		#1
		a_TB = 1'd0;
		b_TB = 1'd0;
		c_TB = 1'd1;

		//Iteration: 7
		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd0;

		//Iteration: 8
		#1
		a_TB = 1'd0;
		b_TB = 1'd0;
		c_TB = 1'd1;

		//Iteration: 9
		#1
		a_TB = 1'd1;
		b_TB = 1'd1;
		c_TB = 1'd0;

		//Iteration: 10
		#1
		a_TB = 1'd0;
		b_TB = 1'd0;
		c_TB = 1'd1;

		$finish;
	end
endmodule