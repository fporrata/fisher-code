module testbench();

reg 			reset, clock;

`define NUM_TESTS	7

reg		[`NUM_TESTS*64-1:0]	test_values;
wire	[31:0]				result;

reg		[`NUM_TESTS*32-1:0]	cres;
reg		[63:0]				value;
//reg		[31:0]				cur_cres;
reg		[9:0]	counte;
reg		[9:0]	next_counte;

wire			done;
wire			correct;
wire			checked;
reg				finish;


//assign done = 0;

//assign result[31:0] = 32'hfffff;

wire	[64:0]		result_ext = {33'b0, result};
assign correct = (result_ext * result_ext <= value && (result_ext+1) * (result+1) > value)? 1:0;

ISR isr0(.reset(reset),
		 .value(value),
		 .clock(clock),
		 .result(result),
		 .done(done));

// Check the result
always @(posedge clock)
begin
#2
	if (done && ~correct && ~reset)
	begin
		$display("@@@Failed");
		$finish;
	end
	if (finish)
		$display("@@@Passed");
end

// Increase the counter
always @*
begin
	next_counte = counte + 10'b1;
end


// Clock
always
begin
#5
	clock = ~clock;
end

initial
begin
	$monitor("Time:%4.0f reset:%b done:%b value:%h result:%h correct:%b", $time, reset, 
			done, value,
			result, correct);

	test_values[1*64-1:0] = 64'hFFFFFFFFFFFFFFFF;
	test_values[2*64-1:64] = 64'hFFFFFFFFFF;
	test_values[3*64-1:2*64] = 64'h3;
	test_values[4*64-1:3*64] = 64'h0;
	test_values[5*64-1:4*64] = 64'b10101010101010101;
	test_values[6*64-1:5*64] = 64'h1000000000000000;
	test_values[7*64-1:6*64] = 64'h10000000000;

	reset = 1;
	clock = 0;
	finish = 0;
	counte = 0;

	// ISR should record the last value before negedge reset
	value = test_values[63:0];
	@(posedge clock)
	@(posedge clock)
	@(negedge clock)
	value = test_values[127:64];
	@(negedge clock);
	reset = 0;
	@(posedge done);
	@(posedge clock);

	// ISR should not change value when reset is not asserted
	@(negedge clock);
	reset = 1;
	value = test_values[63:0];
	@(posedge clock)
	@(posedge clock)
	@(negedge clock)
	value = test_values[127:64];
	@(negedge clock);
	reset = 0;
	value = test_values[63:0];
	@(posedge done);
	value = test_values[127:64];
	@(posedge clock);

	@(negedge clock);
	reset = 1;


	while (counte < `NUM_TESTS)
	begin
		$display("Loop %b", counte);
		value = test_values[(counte+1)*64-1 -:64]; //(counte-1)*64];
//		cur_cres = cres[(counte+1)*32-1 -:32]; //(counte-1)*32];
		//cur_value[(counte*64-1) -:64] = 64'b0;

		@(negedge clock);
		reset = 0;
		@(posedge done);
		@(posedge clock);
		@(negedge clock);
		reset = 1;
		counte = next_counte;
	end

	while (counte < 1000)
	begin
		$display("Loop %b", counte);
		value = {$random, $random};
		@(negedge clock);
		reset = 0;
		@(posedge done);
		@(posedge clock);
		@(negedge clock);
		reset = 1;
		counte = next_counte;
	end

	finish = 1;
	@(negedge clock);
	$finish;
end

endmodule
