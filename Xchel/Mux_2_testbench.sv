/******************************************************************
* Testbench created automatically with a program written in C++ by:
*	 - Morales Hurtado David Xchel
*
* For the first project in the class of professor:
*	 - Carolina Rosas Huerta
* In the Silicon Verification Program
*
* 	Design Name : Mux_2
* 	File Name : Mux_2_testbench.sv
* 	Date: Mon Nov 16 22:50:13 2020

*
* The program mode used was Automatic
******************************************************************/

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module Mux_2_TB();

   //Input signals
   reg In_A_tb;
   reg In_B_tb;
   reg In_C_tb;

   //Output signals
   wire Out_Y_tb;

   //TOP module initialization
   Mux_2 UUT( .In_A(In_A_tb), .In_B(In_B_tb), .In_C(In_C_tb), .Out_Y(Out_Y_tb));

   initial
   begin
      $dumpfile("Mux_2.vcd");
      $dumpvars(1,Mux_2_tb);

      //Variable initialization
      In_A_tb='b0;
      In_B_tb='b0;
      In_C_tb='b0;

      //The program is made for 10 cycles
      //Iteration: 1
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b0;
      In_C_tb = 1'b1;

      //Iteration: 2
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b1;

      //Iteration: 3
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b1;

      //Iteration: 4
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b1;

      //Iteration: 5
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b0;

      //Iteration: 6
      #1
      In_A_tb = 1'b1;
      In_B_tb = 1'b0;
      In_C_tb = 1'b1;

      //Iteration: 7
      #1
      In_A_tb = 1'b1;
      In_B_tb = 1'b1;
      In_C_tb = 1'b0;

      //Iteration: 8
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b0;

      //Iteration: 9
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b0;
      In_C_tb = 1'b0;

      //Iteration: 10
      #1
      In_A_tb = 1'b0;
      In_B_tb = 1'b1;
      In_C_tb = 1'b0;

      //Ending iteration
      #1
      $finish;
   end
endmodule
