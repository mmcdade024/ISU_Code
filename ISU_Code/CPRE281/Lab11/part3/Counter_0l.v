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
// CREATED		"Wed Apr 24 13:53:23 2024"

module Counter_0l(
	w,
	Clock,
	A,
	B,
	C,
	D,
	E,
	F,
	G
);


input wire	w;
input wire	Clock;
output wire	A;
output wire	B;
output wire	C;
output wire	D;
output wire	E;
output wire	F;
output wire	G;

wire	Y0;
wire	Y1;
reg	SYNTHESIZED_WIRE_19;
wire	SYNTHESIZED_WIRE_20;
wire	SYNTHESIZED_WIRE_21;
wire	SYNTHESIZED_WIRE_2;
wire	SYNTHESIZED_WIRE_3;
wire	SYNTHESIZED_WIRE_4;
wire	SYNTHESIZED_WIRE_5;
wire	SYNTHESIZED_WIRE_22;
wire	SYNTHESIZED_WIRE_23;
wire	SYNTHESIZED_WIRE_9;
wire	SYNTHESIZED_WIRE_10;
reg	SYNTHESIZED_WIRE_24;
wire	SYNTHESIZED_WIRE_11;
wire	SYNTHESIZED_WIRE_12;
wire	SYNTHESIZED_WIRE_13;

assign	SYNTHESIZED_WIRE_22 = 1;
assign	SYNTHESIZED_WIRE_12 = 0;



assign	SYNTHESIZED_WIRE_10 = SYNTHESIZED_WIRE_19 & SYNTHESIZED_WIRE_20;

assign	SYNTHESIZED_WIRE_9 = w & SYNTHESIZED_WIRE_21;

assign	SYNTHESIZED_WIRE_4 = w & SYNTHESIZED_WIRE_2 & SYNTHESIZED_WIRE_19;

assign	Y1 = SYNTHESIZED_WIRE_3 | SYNTHESIZED_WIRE_4 | SYNTHESIZED_WIRE_5;


always@(posedge SYNTHESIZED_WIRE_23 or negedge SYNTHESIZED_WIRE_22 or negedge SYNTHESIZED_WIRE_22)
begin
if (!SYNTHESIZED_WIRE_22)
	begin
	SYNTHESIZED_WIRE_24 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_22)
	begin
	SYNTHESIZED_WIRE_24 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_24 <= Y1;
	end
end

assign	Y0 = SYNTHESIZED_WIRE_9 | SYNTHESIZED_WIRE_10;

assign	SYNTHESIZED_WIRE_2 =  ~SYNTHESIZED_WIRE_24;

assign	SYNTHESIZED_WIRE_13 = SYNTHESIZED_WIRE_24 | SYNTHESIZED_WIRE_11;


clock_generator	b2v_inst16(
	.CLK_IN(Clock),
	.CLK_OUT(SYNTHESIZED_WIRE_23));


seven_seg_decoder	b2v_inst17(
	.X_3(SYNTHESIZED_WIRE_12),
	.X_2(SYNTHESIZED_WIRE_24),
	.X_1(SYNTHESIZED_WIRE_19),
	.X_0(SYNTHESIZED_WIRE_13),
	.A(A),
	.B(B),
	.C(C),
	.D(D),
	.E(E),
	.F(F),
	.G(G));


assign	SYNTHESIZED_WIRE_11 =  ~SYNTHESIZED_WIRE_19;

assign	SYNTHESIZED_WIRE_3 = SYNTHESIZED_WIRE_24 & SYNTHESIZED_WIRE_21;

assign	SYNTHESIZED_WIRE_20 =  ~w;


always@(posedge SYNTHESIZED_WIRE_23 or negedge SYNTHESIZED_WIRE_22 or negedge SYNTHESIZED_WIRE_22)
begin
if (!SYNTHESIZED_WIRE_22)
	begin
	SYNTHESIZED_WIRE_19 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_22)
	begin
	SYNTHESIZED_WIRE_19 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_19 <= Y0;
	end
end


assign	SYNTHESIZED_WIRE_21 =  ~SYNTHESIZED_WIRE_19;

assign	SYNTHESIZED_WIRE_5 = SYNTHESIZED_WIRE_20 & SYNTHESIZED_WIRE_24;


endmodule
