`timescale 1ns / 1ps

`include "PC.sv"
`include "Imemo.sv"
`include "RAM.sv"
`include "Add.sv"
`include "Control.sv"
`include "ALUcontrol.sv"
`include "ALU.sv"

module TOP(input wire clk,clock,input rst, rts,input[0:6]halo, hola, output[0:31]PC_Out ,   Imemo_Inst,RAM_Rw, RAM_R1 [0:5], output [0:31] RAM_R2,  output ALU_Flag[0:5][0:7]);
  
  reg [0:5] ControlOp;
  reg RAM_we;
  reg [0:11] ALUFun;
  
  PC U1(.In(PC_In), .clk(clk), .reset(rst), .Out(PC_Out));
  Add U2(.In(PC_Out), .Out(PC_In));
  
  Imemo      UU1(.RA(PC_Out),.clk(clk),.Inst(Imemo_Inst));
  RAM        UU2(.Ra1(Imemo_Inst[6:10]), .Ra2(Imemo_Inst[11:15]), .Raw(Imemo_Inst[16:20]), .Rw(RAM_Rw), .clk(clk), .we(RAM_we), .R1(RAM_R1), .R2(RAM_R2));
  
  Control    UU4(.Op(Imemo_Inst[0:5]),.we(RAM_we),.ALUOp(ControlOp));
  ALUcontrol UU5(.Op(ControlOp),.Fun(Imemo_Inst[26:31]),.Out(ALUFun));
  ALU        UU6(.In1(RAM_R1),.In2(RAM_R2),.Fun(ALUFun),.Out(RAM_Rw),.Flag(ALU_Flag));
  
endmodule