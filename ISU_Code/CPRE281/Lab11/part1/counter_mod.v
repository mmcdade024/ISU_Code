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
// CREATED		"Wed Apr 24 16:59:08 2024"

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
wire	SYNTHESIZED_WIRE_26;
reg	SYNTHESIZED_WIRE_27;
wire	SYNTHESIZED_WIRE_28;
wire	SYNTHESIZED_WIRE_2;
wire	SYNTHESIZED_WIRE_29;
wire	SYNTHESIZED_WIRE_5;
wire	SYNTHESIZED_WIRE_6;
wire	SYNTHESIZED_WIRE_9;
reg	SYNTHESIZED_WIRE_30;
reg	SYNTHESIZED_WIRE_31;
wire	SYNTHESIZED_WIRE_12;
wire	SYNTHESIZED_WIRE_13;
wire	SYNTHESIZED_WIRE_14;
wire	SYNTHESIZED_WIRE_32;
wire	SYNTHESIZED_WIRE_18;
wire	SYNTHESIZED_WIRE_20;
wire	SYNTHESIZED_WIRE_21;
wire	SYNTHESIZED_WIRE_22;
wire	SYNTHESIZED_WIRE_23;

assign	SYNTHESIZED_WIRE_28 = 1;
assign	SYNTHESIZED_WIRE_9 = 0;



assign	SYNTHESIZED_WIRE_6 = SYNTHESIZED_WIRE_26 & SYNTHESIZED_WIRE_27;

assign	SYNTHESIZED_WIRE_26 =  ~W;


always@(posedge Clock or negedge SYNTHESIZED_WIRE_28 or negedge SYNTHESIZED_WIRE_28)
begin
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_31 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_31 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_31 <= SYNTHESIZED_WIRE_2;
	end
end

assign	SYNTHESIZED_WIRE_5 = W & SYNTHESIZED_WIRE_29;

assign	Y0 = SYNTHESIZED_WIRE_5 | SYNTHESIZED_WIRE_6;


always@(posedge Clock or negedge SYNTHESIZED_WIRE_28 or negedge SYNTHESIZED_WIRE_28)
begin
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_30 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_30 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_30 <= Y2;
	end
end


seven_seg_decoder	b2v_inst2(
	.X_3(SYNTHESIZED_WIRE_9),
	.X_2(SYNTHESIZED_WIRE_30),
	.X_1(SYNTHESIZED_WIRE_31),
	.X_0(SYNTHESIZED_WIRE_27),
	.A(A),
	.B(B),
	.C(C),
	.D(D),
	.E(E),
	.F(F),
	.G(G));

assign	SYNTHESIZED_WIRE_29 =  ~SYNTHESIZED_WIRE_27;

assign	SYNTHESIZED_WIRE_14 = SYNTHESIZED_WIRE_26 & SYNTHESIZED_WIRE_31;

assign	SYNTHESIZED_WIRE_12 = SYNTHESIZED_WIRE_29 & SYNTHESIZED_WIRE_31;

assign	SYNTHESIZED_WIRE_2 = SYNTHESIZED_WIRE_12 | SYNTHESIZED_WIRE_13 | SYNTHESIZED_WIRE_14;

assign	SYNTHESIZED_WIRE_13 = W & SYNTHESIZED_WIRE_27 & SYNTHESIZED_WIRE_32;

assign	SYNTHESIZED_WIRE_32 =  ~SYNTHESIZED_WIRE_31;

assign	SYNTHESIZED_WIRE_20 = SYNTHESIZED_WIRE_26 & SYNTHESIZED_WIRE_30;

assign	SYNTHESIZED_WIRE_23 = SYNTHESIZED_WIRE_29 & SYNTHESIZED_WIRE_30;

assign	SYNTHESIZED_WIRE_21 = W & SYNTHESIZED_WIRE_27 & SYNTHESIZED_WIRE_31 & SYNTHESIZED_WIRE_18;

assign	SYNTHESIZED_WIRE_22 = SYNTHESIZED_WIRE_32 & SYNTHESIZED_WIRE_30;

assign	Y2 = SYNTHESIZED_WIRE_20 | SYNTHESIZED_WIRE_21 | SYNTHESIZED_WIRE_22 | SYNTHESIZED_WIRE_23;

assign	SYNTHESIZED_WIRE_18 =  ~SYNTHESIZED_WIRE_30;




always@(posedge Clock or negedge SYNTHESIZED_WIRE_28 or negedge SYNTHESIZED_WIRE_28)
begin
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_27 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_28)
	begin
	SYNTHESIZED_WIRE_27 <= 1;
	end
else
	begin
	SYNTHESIZED_WIRE_27 <= Y0;
	end
end


endmodule
