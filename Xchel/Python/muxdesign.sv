module Logic_mux2(
  input [1:2] a;
  input [5:7] b,c;
  input clk, rst,i,j;
  output reg y
  );
  
  logic not_output, or_output;
  
  not_gate not_gate1(c, not_output);
  
  or_gate or_gate1(b, not_output, or_output);
  
  mux2 mux2_1(or_output, b, a, y);
  
endmodule