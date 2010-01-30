module ISR(reset, value, clock, result, done);
input 			reset;
input	[63:0]	value;
input 			clock;

output	[31:0]	result;
output			done;

wire	[63:0]	sq_result;

reg		[31:0]	result;
reg				done;

wire			sq_done;
reg				sq_start;
reg				sq_reset;

reg		[4:0]	counter;
reg		[4:0]	next_counter;

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
	next_counter = counter - 1;
/*	if (sq_done)
	begin
		next_counter = counter - 1;
	end*/
end

always @(posedge clock)
begin
	if (reset)
	begin
		counter <= #1 5'h1f;
		result <= #1 32'b0;
		done <= #1 0;

		sq_start <= #1 0;
		sq_reset <= #1 1;

//		sq_done = 0;
	end
	else
	begin
		if (~done)
		begin
			if (sq_reset) begin
				sq_reset <= #1 1'b0;
				sq_start <= #1 1'b1;
				result[counter] <= #1 1'b1;
			end
			else if (sq_start)
			begin
				sq_start <= #1 1'b0;
			end
			else if (sq_done) begin
				//$display("counter %d", counter);
				if (sq_result > value)
					result[counter] <= #1 1'b0;
				sq_start <= #1 1'b1;
				if (counter == 0)
					done <= #1 1'b1;
				else begin
					result[counter-1] <= #1 1'b1;
					counter <= #1 next_counter;
				end
			end 
		end
	end
end

endmodule
