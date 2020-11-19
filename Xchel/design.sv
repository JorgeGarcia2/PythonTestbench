`timescale 1ns / 1ps

module FSM(clk,rst,A,B,Out);
  input clk,rst,A,B;
  output reg Out;
  
  parameter S0=2'b00;
  parameter S1=2'b01;
  parameter S2=2'b11;
  reg[0:1] S,Sn;
  
  initial
  begin
    S=2'b00;
    Sn=2'b00;
    Out=0;
  end
  
  always@(posedge clk or rst)
  begin
    S=rst?S0:Sn;
  end
  
  always@*
  begin
    case(S)
      S0: Sn=A?S1:S0;
      S1: Sn=B?S2:S0;
      default:
      begin
        Sn=(A&B)?S2:S0;
        Out=(A&B)?1:0;
      end
    endcase
  end
  
endmodule