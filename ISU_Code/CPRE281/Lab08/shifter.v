// Copyright (C) 2023  Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions 
// and other software and tools, and any partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License 
// Subscription Agreement, the Intel Quartus Prime License Agreement,
// the Intel FPGA IP License Agreement, or other applicable license
// agreement, including, without limitation, that your use is for
// the sole purpose of programming logic devices manufactured by
// Intel and sold by Intel or its authorized distributors.  Please
// refer to the applicable agreement for further details, at
// https://fpgasoftware.intel.com/eula.

// PROGRAM		"Quartus Prime"
// VERSION		"Version 22.1std.1 Build 917 02/14/2023 SC Standard Edition"
// CREATED		"Mon Apr  1 15:26:41 2024"

module shifter(
	X3,
	X2,
	X1,
	X0,
	Xc2,
	Xc1,
	Xc3,
	S,
	F
);


input wire	X3;
input wire	X2;
input wire	X1;
input wire	X0;
input wire	Xc2;
input wire	Xc1;
input wire	Xc3;
input wire	[1:0] S;
output wire	[3:0] F;

wire	[3:0] F_ALTERA_SYNTHESIZED;





mux2to1	b2v_0(
	.W3(X3),
	.W2(X2),
	.W1(X1),
	.W0(X0),
	.S(S),
	.F(F_ALTERA_SYNTHESIZED[3]));


mux2to1	b2v_1(
	.W3(X2),
	.W2(X1),
	.W1(X0),
	.W0(Xc1),
	.S(S),
	.F(F_ALTERA_SYNTHESIZED[2]));


mux2to1	b2v_2(
	.W3(X1),
	.W2(X0),
	.W1(Xc1),
	.W0(Xc2),
	.S(S),
	.F(F_ALTERA_SYNTHESIZED[1]));


mux2to1	b2v_3(
	.W3(X0),
	.W2(Xc1),
	.W1(Xc2),
	.W0(Xc3),
	.S(S),
	.F(F_ALTERA_SYNTHESIZED[0]));

assign	F = F_ALTERA_SYNTHESIZED;

endmodule
