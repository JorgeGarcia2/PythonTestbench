//Testbench created automatically with a program written in c++ by:
//	Morales Hurtado David Xchel
//
//For the first project in the class of professor:
//	//Jorge Martínez Carbballido
//
//In the Silicon Verification Program

//time scale
`timescale 1ns/1ps

//Main Testbench Starts here
module TOP_TB();

   //Input signals
   reg clk_tb ;
   reg rst_tb ;
   reg [-15] signal_tb ;

   //Output signals
   wire PC_Out_tb ;
   wire Imemo_Inst_tb ;
   wire [-15] RAM_Rw_tb ;
   wire [32766] RAM_R1_tb ;
   wire RAM_R2_tb ;
   wire [184792581] ALU_Flag_tb ;
   //TOP module initialization
   TOP UUT( .clk(clk_tb), .rst(rst_tb), .signal(signal_tb), .PC_Out(PC_Out_tb), .Imemo_Inst(Imemo_Inst_tb), .RAM_Rw(RAM_Rw_tb), .RAM_R1(RAM_R1_tb), .RAM_R2(RAM_R2_tb), .ALU_Flag(ALU_Flag_tb),);

   //Clock initialization
   always forever #1 clk_tb=~clk_tb;

   initial
   begin
      $dumpfile("TOP.vcd");
      $dumpvars(1,TOP_tb);

      //Variable initialization
      clk_tb=0;
      rst_tb=1;
      signal_tb=0;

      //Reset inactive
      #2
      rst_tb=0;

      //The program is made for 10 cycles
      //Iteration: 1
      #1
      signal_tb=-1;

      //Iteration: 2
      #1
      signal_tb=-1;

      //Iteration: 3
      #1
      signal_tb=-1;

      //Iteration: 4
      #1
      signal_tb=-1;

      //Iteration: 5
      #1
      signal_tb=-1;

      //Iteration: 6
      #1
      signal_tb=-1;

      //Iteration: 7
      #1
      signal_tb=-1;

      //Iteration: 8
      #1
      signal_tb=-1;

      //Iteration: 9
      #1
      signal_tb=-1;

      //Ending iteration: 10
      #1
      $finish;
   end
endmodule
