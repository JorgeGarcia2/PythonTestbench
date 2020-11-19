/******************************************************************
* Testbench created automatically with a program written in C++ by:
*	 - Morales Hurtado David Xchel
*
* For the first project in the class of professor:
*	 - Carolina Rosas Huerta
* In the Silicon Verification Program
*
* 	Design Name : Logic_mux2
* 	File Name : Logic_mux2_testbench.sv
* 	Date: Mon Nov 16 22:30:59 2020

*
* The program mode used was Automatic
******************************************************************/

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module Logic_mux2_TB();

   //Input signals
   reg [5] a_tb;
   reg [5] x_tb;
   reg [5] k_tb;
   reg [5] z_tb;
   reg [2] b_tb;
   reg [2] c_tb;
   reg i_tb;
   reg j_tb;

   //Output signals
   wire [5] y_tb;

   //TOP module initialization
   Logic_mux2 UUT( .a(a_tb), .x(x_tb), .k(k_tb), .z(z_tb), .b(b_tb), .c(c_tb), .i(i_tb), .j(j_tb), .y(y_tb),);

   //Clock initialization
   always forever #1 clk_tb=~clk_tb;

   initial
   begin
      $dumpfile("Logic_mux2.vcd");
      $dumpvars(1,Logic_mux2_tb);

      //Variable initialization
      a_tb='d;
      x_tb='d;
      k_tb='d;
      z_tb='d;
      b_tb='d;
      c_tb='d;
      i_tb='d;
      j_tb='d;
      clk_tb='d;
      rst_tb='d;

      //Reset inactive
      #2
      rst_tb=~rst_tb;

      //The program is made for 10 cycles
      //Iteration: 1
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 2
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 3
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 4
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 5
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 6
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 7
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 8
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 9
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Iteration: 10
      #1
      a_tb = 6'd;
      x_tb = 6'd;
      k_tb = 6'd;
      z_tb = 6'd;
      b_tb = 3'd;
      c_tb = 3'd;
      i_tb = 1'd;
      j_tb = 1'd;

      //Ending iteration
      #1
      $finish;
   end
endmodule
