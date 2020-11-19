/******************************************************************
* Testbench created automatically with a program written in C++ by:
*	 - Morales Hurtado David Xchel
*
* For the first project in the class of professor:
*	 - Carolina Rosas Huerta
* In the Silicon Verification Program
*
* 	Design Name : reg
* 	File Name : reg_testbench.sv
* 	Date: Mon Nov 16 11:18:25 2020

*
* The program mode used was Automatic
******************************************************************/

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module reg_TB();

   //Input signals

   //Output signals
   //TOP module initialization
   reg UUT();

   //Clock initialization

   initial
   begin
      $dumpfile("reg.vcd");
      $dumpvars(1,reg_tb);

      //Variable initialization

      //Reset inactive
      #2

      //The program is made for 10 cycles
      //Iteration: 1
      #1

      //Iteration: 2
      #1

      //Iteration: 3
      #1

      //Iteration: 4
      #1

      //Iteration: 5
      #1

      //Iteration: 6
      #1

      //Iteration: 7
      #1

      //Iteration: 8
      #1

      //Iteration: 9
      #1

      //Ending iteration: 10
      #1
      $finish;
   end
endmodule
