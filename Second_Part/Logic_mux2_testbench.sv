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
reg [0:2]a_TB;
reg [0:2]b_TB;
reg [0:2]c_TB;
wire y_TB;

Logic_mux2 UUT(.a(a_TB), .b(b_TB), .c(c_TB), .y(y_TB));

initial
	begin
		$dumpfile("Logic_mux2.vcd");
		$dumpvars(1, Logic_mux2_TB);

		//Initializing values
		a_TB = 3'b0;
		b_TB = 3'b0;
		c_TB = 3'b0;

		//The program will iterate 10 times
		//Iteration: 1
		#1
		a_TB = 3'd1;
		b_TB = 3'd0;
		c_TB = 3'd2;

		//Iteration: 2
		#1
		a_TB = 3'd2;
		b_TB = 3'd1;
		c_TB = 3'd3;

		//Iteration: 3
		#1
		a_TB = 3'd3;
		b_TB = 3'd2;
		c_TB = 3'd4;

		//Iteration: 4
		#1
		a_TB = 3'd4;
		b_TB = 3'd3;
		c_TB = 3'd5;

		//Iteration: 5
		#1
		a_TB = 3'd5;
		b_TB = 3'd4;
		c_TB = 3'd6;

		//Iteration: 6
		#1
		a_TB = 3'd6;
		b_TB = 3'd5;
		c_TB = 3'd7;

		//Iteration: 7
		#1
		a_TB = 3'd7;
		b_TB = 3'd6;
		c_TB = 3'd0;

		//Iteration: 8
		#1
		a_TB = 3'd0;
		b_TB = 3'd7;
		c_TB = 3'd1;

		//Iteration: 9
		#1
		a_TB = 3'd1;
		b_TB = 3'd0;
		c_TB = 3'd2;

		//Iteration: 10
		#1
		a_TB = 3'd2;
		b_TB = 3'd1;
		c_TB = 3'd3;

		$finish;
	end
endmodule