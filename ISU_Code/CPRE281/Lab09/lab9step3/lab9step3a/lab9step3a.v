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
// CREATED		"Tue Apr  2 17:34:36 2024"

module lab9step3a(
	CLK,
	D,
	Q,
	QN
);


input wire	CLK;
input wire	D;
output wire	Q;
output wire	QN;

wire	SYNTHESIZED_WIRE_0;
wire	SYNTHESIZED_WIRE_1;
wire	SYNTHESIZED_WIRE_2;
wire	SYNTHESIZED_WIRE_3;
wire	SYNTHESIZED_WIRE_14;
wire	SYNTHESIZED_WIRE_15;
wire	SYNTHESIZED_WIRE_7;
wire	SYNTHESIZED_WIRE_10;
wire	SYNTHESIZED_WIRE_11;
wire	SYNTHESIZED_WIRE_12;
wire	SYNTHESIZED_WIRE_13;

assign	Q = SYNTHESIZED_WIRE_13;
assign	QN = SYNTHESIZED_WIRE_10;



assign	SYNTHESIZED_WIRE_2 = ~(CLK & D);

assign	SYNTHESIZED_WIRE_3 = ~(SYNTHESIZED_WIRE_0 & CLK);

assign	SYNTHESIZED_WIRE_0 =  ~D;

assign	SYNTHESIZED_WIRE_14 = ~(SYNTHESIZED_WIRE_1 & SYNTHESIZED_WIRE_2);

assign	SYNTHESIZED_WIRE_1 = ~(SYNTHESIZED_WIRE_3 & SYNTHESIZED_WIRE_14);

assign	SYNTHESIZED_WIRE_11 = ~(SYNTHESIZED_WIRE_15 & SYNTHESIZED_WIRE_14);

assign	SYNTHESIZED_WIRE_12 = ~(SYNTHESIZED_WIRE_7 & SYNTHESIZED_WIRE_15);

assign	SYNTHESIZED_WIRE_7 =  ~SYNTHESIZED_WIRE_14;

assign	SYNTHESIZED_WIRE_13 = ~(SYNTHESIZED_WIRE_10 & SYNTHESIZED_WIRE_11);

assign	SYNTHESIZED_WIRE_10 = ~(SYNTHESIZED_WIRE_12 & SYNTHESIZED_WIRE_13);

assign	SYNTHESIZED_WIRE_15 =  ~CLK;


endmodule
