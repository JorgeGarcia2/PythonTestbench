// Logic with Mux2:1 Code

// Luis Fernando Rodriguez Contreras
// 18-10-2020

// Include the Mux2:1, Not Gate and Or Gate modules
`include "mux2-1.sv"
`include "not_gate.sv"
`include "or_gate.sv"

module Logic_mux2(
  input a, b, c,
  output reg y
  );
  
  logic not_output, or_output;
  
  not_gate not_gate1(c, not_output);
  
  or_gate or_gate1(b, not_output, or_output);
  
  mux2 mux2_1(or_output, b, a, y);
  
endmodule