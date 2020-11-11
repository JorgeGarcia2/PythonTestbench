//time scale
`timescale 1ns/1ps
//Main Testbench Starts here
module TOP_TB;
reg clock_TB;
reg rst_TB;
reg rts_TB;
reg [0:6]halo_TB;
reg [0:6]hola_TB;
wire [0:31]PC_Out_TB;
wire [0:31]Imemo_Inst_TB;
wire [0:31]RAM_Rw_TB;
wire [0:31]RAM_R1_TB;
wire [0:31]RAM_R2_TB;
wire ALU_Flag_TB;

TOP UUT(.clock(clock_TB), .rst(rst_TB), .rts(rts_TB), .halo(halo_TB), .hola(hola_TB), .PC_Out(PC_Out_TB), .Imemo_Inst(Imemo_Inst_TB), .RAM_Rw(RAM_Rw_TB), .RAM_R1(RAM_R1_TB), .RAM_R2(RAM_R2_TB), .ALU_Flag(ALU_Flag_TB));

	always forever #1 clock_TB = ~clock_TB;

initial
	begin
		$dumpfile("TOP.vcd");
		$dumpvars(1, TOP_TB);

		clock_TB = 0;
		rst_TB = 1;
		rts_TB = 1'b0;
		halo_TB = 7'b0;
		hola_TB = 7'b0;

		#2
		rst_TB = 0;

		#1
		rts_TB = 1'b0;
		halo_TB = 7'b10110;
		hola_TB = 7'b10101;

		#1
		rts_TB = 1'b1;
		halo_TB = 7'b10111;
		hola_TB = 7'b10110;

		#1
		rts_TB = 1'b0;
		halo_TB = 7'b11000;
		hola_TB = 7'b10111;

		#1
		rts_TB = 1'b1;
		halo_TB = 7'b11001;
		hola_TB = 7'b11000;

		#1
		rts_TB = 1'b0;
		halo_TB = 7'b11010;
		hola_TB = 7'b11001;

		#1
		rts_TB = 1'b1;
		halo_TB = 7'b11011;
		hola_TB = 7'b11010;

		#1
		rts_TB = 1'b0;
		halo_TB = 7'b11100;
		hola_TB = 7'b11011;

		#1
		rts_TB = 1'b1;
		halo_TB = 7'b11101;
		hola_TB = 7'b11100;

		#1
		rts_TB = 1'b0;
		halo_TB = 7'b11110;
		hola_TB = 7'b11101;

		#1
		rts_TB = 1'b1;
		halo_TB = 7'b11111;
		hola_TB = 7'b11110;

		#1
		$finish;
	end
endmodule