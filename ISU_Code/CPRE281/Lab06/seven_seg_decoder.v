module seven_seg_decoder (A,B,C,D,E,F,G,X_3,X_2,X_1,X_0);

	input X_3,X_2,X_1,X_0;
	output A,B,C,D,E,F,G;
	
	assign A = (~X_3 & ~X_2 & ~X_1 & X_0) | (~X_3 & X_2 & ~X_1 & ~X_0) | (X_3 & ~X_2 & X_1 & X_0) | (X_3 & X_2 & ~X_1 & X_0);
	
	assign B = (X_2 & X_1 & ~X_0) | (X_3 & X_1 & X_0) | (X_3 & X_2 & ~X_0) | (~X_3 & X_2 & ~X_1 & X_0);
	
	assign C = (X_3 & X_2 & ~X_0) | (X_3 & X_2 & X_1) | (~X_3 & ~X_2 & X_1 & ~X_0);
	
	assign D = (X_2 & X_1 & X_0) | (~X_3 & ~X_2 & ~X_1 & X_0) | (~X_3 & X_2 & ~X_1 & ~X_0) | (X_3 & ~X_2 & X_1 & ~X_0);
	
	assign E = (~X_3 & X_0) | (~X_2 & ~X_1 & X_0) | (~X_3 & X_2 & ~X_1);
	
	assign F = (~X_3 & ~X_2 & X_0) | (~X_3 & ~X_2 & X_1) | (~X_3 & X_1 & X_0) | (X_3 & X_2 & ~X_1 & X_0);
	
	assign G = (~X_3 & ~X_2 & ~X_1) | (~X_3 & X_2 & X_1 & X_0)| (X_3 & X_2 & ~X_1 & ~X_0);
	
endmodule