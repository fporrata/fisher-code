module testbench;
	reg [0:3] req;
	reg  en;
	wire [0:3] gnt;
	reg [0:3] tb_gnt;
	wire correct;
	wire [0:1] count;
	reg reset;
	reg clock;

	rps4 rps4_i(clock,reset,req,en,gnt,count);

	assign correct=(gnt==tb_gnt);

	always @(correct)
	begin
		#2
		if(!correct)
		begin
			$display("@@@ Incorrect at time %4.0f", $time);
			$display("@@@ gnt=%b, en=%b, req=%b",gnt,en,req);
			$display("@@@ expected result=%b", tb_gnt);
			$finish;
		end
	end
	always
	  #5 clock=~clock;


	initial 
	begin
		$monitor("Time:%4.0f req:%b en:%b gnt:%b, cnt:%b", $time, req, en, gnt,count);

		// CNT=????, need to reset.
		clock=0;
		reset=1;
#6
		// CNT=0
		reset=0;
		req=4'b0001;
		en=1;
		tb_gnt=4'b0001;
#10
		// CNT=1
		req=4'b0010;
		en=1;
		tb_gnt=4'b0010;
#10
		// CNT=2
		req=4'b0101;
		tb_gnt=4'b0100;
#10
		// CNT=3
		req=4'b0011;
		tb_gnt=4'b0010;
#10
		// CNT=0
		req=4'b1111;
		tb_gnt=4'b0001;
#10
		// CNT=1
		req=4'b1111;
		tb_gnt=4'b0010;
#10
		// CNT=2
		req=4'b1111;
		tb_gnt=4'b0100;
#10
		// CNT=3
		req=4'b1111;
		tb_gnt=4'b1000;
#10
		// CNT=0
		req=4'b1111;
                en=0;
		tb_gnt=4'b0000;
#10
		// CNT=1
		req=4'b1111;
		tb_gnt=4'b0000;
#10
		$finish;
 	end // initial
endmodule
