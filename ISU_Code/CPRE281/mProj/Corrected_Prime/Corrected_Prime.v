module corrected_Prime (P,W,X,Y,Z);

	input W,X,Y,Z;
	output P;
	
	assign P = ((Y & Z) | (X & Z) | (~W & ~X & Y));
	
endmodule 