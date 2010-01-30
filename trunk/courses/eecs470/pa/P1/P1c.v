module ps2(req, en, gnt, req_up);
	input	[1:0]	req;
	input			en;
	output	[1:0]	gnt;
	output			req_up;

	assign gnt[1] = en & req[1];
	assign gnt[0] = en & req[0] & !req[1];
	assign req_up = gnt[1] | gnt[0];

endmodule

module ps4(req, en, gnt, req_up);
	input	[3:0]	req;
	input			en;
	output	[3:0]	gnt;
	output			req_up;
	
	wire	[1:0]	in_req_up;
	wire	[1:0]	in_en;
	wire	[3:0]	tmp;

	reg		[3:0]	gnt;

	`define	ENABLE	1'b1

	ps2 left(req[3:2], `ENABLE, tmp[3:2], in_req_up[1]);
	ps2 right(req[1:0], `ENABLE, tmp[1:0], in_req_up[0]);
	ps2 top(in_req_up, en, in_en, req_up);

	always @ *
	begin
		if (in_en[1])
			gnt[3:2] = tmp[3:2];
		else
			gnt[3:2] = 2'b0;
		if (in_en[0])
			gnt[1:0] = tmp[1:0];
		else
			gnt[1:0] = 2'b0;
	end
endmodule

module ps8(req, en, gnt, req_up);
	input	[7:0]	req;
	input			en;
	output	[7:0]	gnt;
	output			req_up;

	wire	[1:0]	in_req_up;
	wire	[1:0]	in_en;
	wire	[7:0]	tmp;

	reg		[7:0]	gnt;

	`define ENABLE	1'b1
	
	ps4 left(req[7:4], `ENABLE, tmp[7:4], in_req_up[1]);
	ps4 right(req[3:0], `ENABLE, tmp[3:0], in_req_up[0]);
	ps2 top(in_req_up, en, in_en, req_up);

	always @ *
	begin
		if (in_en[1])
			gnt[7:4] = tmp[7:4];
		else
			gnt[7:4] = 4'b0000;
		if (in_en[0])
			gnt[3:0] = tmp[3:0];
		else
			gnt[3:0] = 4'b0000;
	end

endmodule
