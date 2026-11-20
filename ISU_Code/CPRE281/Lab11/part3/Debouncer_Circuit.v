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
// CREATED		"Tue Apr 23 21:40:35 2024"

module Debouncer_Circuit(
	Board,
	Manual,
	Smooth
);


input wire	Board;
input wire	Manual;
output reg	Smooth;

wire	SYNTHESIZED_WIRE_0;
wire	SYNTHESIZED_WIRE_4;
wire	SYNTHESIZED_WIRE_2;

assign	SYNTHESIZED_WIRE_4 = 1;





clock_divider_1024	b2v_inst1(
	.CLK_IN(SYNTHESIZED_WIRE_0),
	.CLK_OUT(SYNTHESIZED_WIRE_2));


clock_divider_1024	b2v_inst12(
	.CLK_IN(Board),
	.CLK_OUT(SYNTHESIZED_WIRE_0));


always@(posedge SYNTHESIZED_WIRE_2 or negedge SYNTHESIZED_WIRE_4 or negedge SYNTHESIZED_WIRE_4)
begin
if (!SYNTHESIZED_WIRE_4)
	begin
	Smooth <= 0;
	end
else
if (!SYNTHESIZED_WIRE_4)
	begin
	Smooth <= 1;
	end
else
	begin
	Smooth <= Manual;
	end
end


endmodule
