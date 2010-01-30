module testbench;
	reg [0:3] in;
	wire out;
	wire tb_out;

    	AND4 test(in,out);

	assign tb_out=in[0]&in[1]&in[2]&in[3];
	assign correct=out==tb_out;

	always @(correct)
	begin
		#2
		if(!correct)
		begin
			$display("@@@ Incorrect at time %4.0f", $time);
			$display("@@@ in:%b out:%b", in, out);
			$display("@@@ expected result=%b", tb_out);
			$finish;
		end
	end

	initial 
	begin
		$monitor("Time:%4.0f in:%b out:%b", $time, in, out);
		in=4'b0000;
		#5	
		in=4'b1100;
		#5
		in=4'b0110;
		#5
		in=4'b1111;
		#5
		in=4'b0111;
		#5
		$finish;
 	end // initial
endmodule
