/******************************************************************
* Testbench created automatically with a program written in C++ by:
*	 - Morales Hurtado David Xchel
*
* For the first project in the class of professor:
*	 - Carolina Rosas Huerta
* In the Silicon Verification Program
*
* 	Design Name : FSM
* 	File Name : FSM_testbench.sv
* 	Date: Mon Nov 16 23:32:15 2020

*
* The program mode used was Automatic
******************************************************************/

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module FSM_tb();

   //Input signals
   reg clk_tb;
   reg rst_tb;
   reg A_tb;
   reg B_tb;

   //Output signals
   wire Out_tb;

   //TOP module initialization
   FSM UUT( .clk(clk_tb), .rst(rst_tb), .A(A_tb), .B(B_tb), .Out(Out_tb));

   //Clock initialization
   always forever #1 clk_tb=~clk_tb;

   initial
   begin
      $dumpfile("FSM.vcd");
      $dumpvars(1,FSM_tb);

      //Variable initialization
      A_tb='b0;
      B_tb='b0;
      clk_tb='b0;
      rst_tb='b1;

      //Reset inactive
      #2
      rst_tb=~rst_tb;

      //The program is made for 10 cycles
      //Iteration: 1
      #1
      A_tb = 1'b0;
      B_tb = 1'b0;

      //Iteration: 2
      #1
      A_tb = 1'b1;
      B_tb = 1'b1;

      //Iteration: 3
      #1
      A_tb = 1'b1;
      B_tb = 1'b1;

      //Iteration: 4
      #1
      A_tb = 1'b1;
      B_tb = 1'b0;

      //Iteration: 5
      #1
      A_tb = 1'b0;
      B_tb = 1'b0;

      //Iteration: 6
      #1
      A_tb = 1'b0;
      B_tb = 1'b0;

      //Iteration: 7
      #1
      A_tb = 1'b0;
      B_tb = 1'b1;

      //Iteration: 8
      #1
      A_tb = 1'b1;
      B_tb = 1'b0;

      //Iteration: 9
      #1
      A_tb = 1'b1;
      B_tb = 1'b1;

      //Iteration: 10
      #1
      A_tb = 1'b0;
      B_tb = 1'b1;

      //Ending iteration
      #1
      $finish;
   end
endmodule
