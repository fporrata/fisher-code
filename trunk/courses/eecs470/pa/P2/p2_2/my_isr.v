module ISR(reset, value, clock, result, done);
input 			reset;
input	[63:0]	value;
input 			clock;

output	[31:0]	result;
output			done;

wire	[63:0]	sq_result;

reg		[31:0]	result;
reg		[31:0]	cur_result;
reg				done;

wire			sq_done;
reg				sq_start;
reg				sq_reset;
reg		[63:0]	dest_value;

reg		[4:0]	counter;
reg		[4:0]	next_counter;

reg		[0:2]	state;
reg		[0:2]	next_state;

`define	INIT		3'b000
`define	SQ_START	3'b001
`define	SQ_RUN		3'b010
`define	SQ_DONE		3'b011
`define	DONE		3'b100

mult square
	(.clock(clock),
	 .reset(sq_reset),
	 .mplier({32'b0, result}),
	 .mcand({32'b0, result}),
	 .start(sq_start),
	 .product(sq_result),
	 .done(sq_done)
	);

always @*
begin
	next_counter = counter;
	cur_result = result;
	case (state)
		`INIT: begin
			done = 1'b0;
			sq_reset = 1'b1;
			sq_start = 1'b0;

			next_state = `SQ_START;
		end
		`SQ_START: begin
			sq_start = 1'b1;
			sq_reset = 1'b0;
			cur_result[counter] = 1'b1;

			next_state = `SQ_RUN;
		end
		`SQ_RUN: begin
			sq_start = 1'b0;
			
			if (sq_done)
				next_state = `SQ_DONE;
			else
				next_state = `SQ_RUN;
		end
		`SQ_DONE: begin
			sq_reset = 1;
			next_counter = counter - 1;
			//$display("sq_result: %d value: %d", sq_result, dest_value);
			if (sq_result > dest_value)
				cur_result[counter] = 1'b0;
			if (counter == 0)
				next_state = `DONE;
			else
				next_state = `SQ_START;
		end
		`DONE: begin
			done = 1'b1;
		end
	endcase
end

always @(posedge clock)
begin
	if (reset) begin
		state <= #1 `INIT;
		counter <= #1 5'h1f;
		dest_value <= #1 value;
		result <= #1 32'b0;
	end else begin
		state <= #1 next_state;
		counter <= #1 next_counter;
		result <= #1 cur_result;
	end
end

endmodule
