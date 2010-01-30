module rps2(req, en, sel, gnt, req_up);
	input	[1:0]	req;
	input			en;
	input			sel;
	output	[1:0]	gnt;
	output			req_up;

	reg		[1:0]	gnt;

	always @ *
	begin
		if (sel)
		begin
			gnt[1] = en & req[1];
			gnt[0] = en & req[0] & ~req[1];
		end
		else
		begin
			gnt[0] = en & req[0];
			gnt[1] = en & req[1] & ~req[0];
		end
	end

	assign req_up = gnt[1] | gnt[0];

endmodule

module rps4(clock, reset, req, en, gnt, count);
	input			clock;
	input			reset;
	input	[3:0]	req;
	input			en;
	output	[3:0]	gnt;
	output	[1:0]	count;
	
	wire	[1:0]	in_req_up;
	wire	[1:0]	in_en;
	wire	[3:0]	tmp;
	wire	[1:0]	sel;
	wire	[1:0]	next_count;

	reg		[3:0]	gnt;
	reg		[1:0]	count;

	`define	ENABLE	1'b1

	assign sel = count;
	assign next_count = count + 2'b1;

	rps2 left(req[3:2], `ENABLE, sel[0], tmp[3:2], in_req_up[1]);
	rps2 right(req[1:0], `ENABLE, sel[0], tmp[1:0], in_req_up[0]);
	rps2 top(in_req_up, en, sel[1], in_en, req_up);

	always @ (posedge clock)
	begin
		if (reset)
			count <= #1 2'b0;
		else
			count <= #1 next_count;
	end

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
