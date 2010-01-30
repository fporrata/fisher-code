module ps4(req, en, gnt);
	input	[3:0]	req;
	input			en;
	output	[3:0]	gnt;

	reg		[3:0]	gnt;

	always @*
	begin
		if (en == 0)
			gnt = 4'b0;
		else
			if (req[3] == 1)
				gnt = 4'b1000;
			else if (req[2] == 1)
				gnt = 4'b0100;
			else if (req[1] == 1)
				gnt = 4'b0010;
			else if (req[0] == 1)
				gnt = 4'b0001;
			else
				gnt = 4'b0;
	end
endmodule
