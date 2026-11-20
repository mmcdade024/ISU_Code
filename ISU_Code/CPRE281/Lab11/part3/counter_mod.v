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
// CREATED		"Tue Apr 23 21:35:52 2024"

module counter_mod(
	W,
	Clock,
	A,
	B,
	C,
	D,
	E,
	F,
	G
);


input wire	W;
input wire	Clock;
output wire	A;
output wire	B;
output wire	C;
output wire	D;
output wire	E;
output wire	F;
output wire	G;

wire	Y0;
wire	Y2;
wire	SYNTHESIZED_WIRE_29;
reg	SYNTHESIZED_WIRE_30;
wire	SYNTHESIZED_WIRE_31;
wire	SYNTHESIZED_WIRE_32;
wire	SYNTHESIZED_WIRE_3;
wire	SYNTHESIZED_WIRE_33;
wire	SYNTHESIZED_WIRE_6;
wire	SYNTHESIZED_WIRE_7;
wire	SYNTHESIZED_WIRE_11;
reg	SYNTHESIZED_WIRE_34;
reg	SYNTHESIZED_WIRE_35;
wire	SYNTHESIZED_WIRE_14;
wire	SYNTHESIZED_WIRE_15;
wire	SYNTHESIZED_WIRE_16;
wire	SYNTHESIZED_WIRE_36;
wire	SYNTHESIZED_WIRE_20;
wire	SYNTHESIZED_WIRE_22;
wire	SYNTHESIZED_WIRE_23;
wire	SYNTHESIZED_WIRE_24;
wire	SYNTHESIZED_WIRE_25;

assign	SYNTHESIZED_WIRE_31 = 1;
assign	SYNTHESIZED_WIRE_11 = 0;



assign	SYNTHESIZED_WIRE_7 = SYNTHESIZED_WIRE_29 & SYNTHESIZED_WIRE_30;

assign	SYNTHESIZED_WIRE_29 =  ~W;


always@(posedge SYNTHESIZED_WIRE_32 or negedge SYNTHESIZED_WIRE_31 or negedge SYNTHESIZED_WIRE_31)
begin
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_35 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_35 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_35 <= SYNTHESIZED_WIRE_3;
	end
end

assign	SYNTHESIZED_WIRE_6 = W & SYNTHESIZED_WIRE_33;

assign	Y0 = SYNTHESIZED_WIRE_6 | SYNTHESIZED_WIRE_7;


always@(posedge SYNTHESIZED_WIRE_32 or negedge SYNTHESIZED_WIRE_31 or negedge SYNTHESIZED_WIRE_31)
begin
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_34 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_34 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_34 <= Y2;
	end
end


seven_seg_decoder	b2v_inst2(
	.X_3(SYNTHESIZED_WIRE_11),
	.X_2(SYNTHESIZED_WIRE_34),
	.X_1(SYNTHESIZED_WIRE_35),
	.X_0(SYNTHESIZED_WIRE_30),
	.A(A),
	.B(B),
	.C(C),
	.D(D),
	.E(E),
	.F(F),
	.G(G));

assign	SYNTHESIZED_WIRE_33 =  ~SYNTHESIZED_WIRE_30;

assign	SYNTHESIZED_WIRE_16 = SYNTHESIZED_WIRE_29 & SYNTHESIZED_WIRE_35;

assign	SYNTHESIZED_WIRE_14 = SYNTHESIZED_WIRE_33 & SYNTHESIZED_WIRE_35;

assign	SYNTHESIZED_WIRE_3 = SYNTHESIZED_WIRE_14 | SYNTHESIZED_WIRE_15 | SYNTHESIZED_WIRE_16;

assign	SYNTHESIZED_WIRE_15 = W & SYNTHESIZED_WIRE_30 & SYNTHESIZED_WIRE_36;

assign	SYNTHESIZED_WIRE_36 =  ~SYNTHESIZED_WIRE_35;

assign	SYNTHESIZED_WIRE_22 = SYNTHESIZED_WIRE_29 & SYNTHESIZED_WIRE_34;


clock_generator	b2v_inst28(
	.CLK_IN(Clock),
	.CLK_OUT(SYNTHESIZED_WIRE_32));

assign	SYNTHESIZED_WIRE_25 = SYNTHESIZED_WIRE_33 & SYNTHESIZED_WIRE_34;

assign	SYNTHESIZED_WIRE_23 = W & SYNTHESIZED_WIRE_30 & SYNTHESIZED_WIRE_35 & SYNTHESIZED_WIRE_20;

assign	SYNTHESIZED_WIRE_24 = SYNTHESIZED_WIRE_36 & SYNTHESIZED_WIRE_34;

assign	Y2 = SYNTHESIZED_WIRE_22 | SYNTHESIZED_WIRE_23 | SYNTHESIZED_WIRE_24 | SYNTHESIZED_WIRE_25;

assign	SYNTHESIZED_WIRE_20 =  ~SYNTHESIZED_WIRE_34;




always@(posedge SYNTHESIZED_WIRE_32 or negedge SYNTHESIZED_WIRE_31 or negedge SYNTHESIZED_WIRE_31)
begin
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_30 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_31)
	begin
	SYNTHESIZED_WIRE_30 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_30 <= Y0;
	end
end


endmodule
