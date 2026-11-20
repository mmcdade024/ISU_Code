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
// CREATED		"Wed Mar 20 14:06:52 2024"

module adder_sub(
	X3,
	Controle,
	Y3,
	X2,
	Y2,
	X1,
	Y1,
	X0,
	Y0,
	Overflow,
	Cout,
	S3,
	S2,
	S1,
	S0
);


input wire	X3;
input wire	Controle;
input wire	Y3;
input wire	X2;
input wire	Y2;
input wire	X1;
input wire	Y1;
input wire	X0;
input wire	Y0;
output wire	Overflow;
output wire	Cout;
output wire	S3;
output wire	S2;
output wire	S1;
output wire	S0;

wire	SYNTHESIZED_WIRE_0;
wire	SYNTHESIZED_WIRE_1;
wire	SYNTHESIZED_WIRE_2;
wire	SYNTHESIZED_WIRE_3;





adder_4bit	b2v_inst(
	.X3(X3),
	.Y3(SYNTHESIZED_WIRE_0),
	.X2(X2),
	.Y2(SYNTHESIZED_WIRE_1),
	.X1(X1),
	.Y1(SYNTHESIZED_WIRE_2),
	.X0(X0),
	.Y0(SYNTHESIZED_WIRE_3),
	.Cin(Controle),
	.Ov(Overflow),
	.Cout(Cout),
	.S3(S3),
	.S2(S2),
	.S1(S1),
	.S0(S0));

assign	SYNTHESIZED_WIRE_0 = Controle ^ Y3;

assign	SYNTHESIZED_WIRE_1 = Controle ^ Y2;

assign	SYNTHESIZED_WIRE_2 = Controle ^ Y1;

assign	SYNTHESIZED_WIRE_3 = Controle ^ Y0;


endmodule
