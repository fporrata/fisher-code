module testbench;
	reg [7:0] req;
	reg  en;
	wire [7:0] gnt;
	wire [7:0] tb_gnt;
	wire correct;
	wire rep_up;

    ps8 pe8(req, en, gnt, rep_up);

	assign tb_gnt[7] = en & req[7];
	assign tb_gnt[6] = en & req[6] & ~req[7];
	assign tb_gnt[5] = en & req[5] & ~req[7] & ~req[6];
	assign tb_gnt[4] = en & req[4] & ~req[7] & ~req[6] & ~req[5];
	assign tb_gnt[3] = en & req[3] & ~req[7] & ~req[6] & ~req[5] & ~req[4];
	assign tb_gnt[2] = en & req[2] & ~req[7] & ~req[6] & ~req[5] & ~req[4] 
						& ~req[3];
	assign tb_gnt[1] = en & req[1] & ~req[7] & ~req[6] & ~req[5] & ~req[4]
						& ~req[3] & ~req[2];
	assign tb_gnt[0] = en & req[0] & ~req[7] & ~req[6] & ~req[5] & ~req[4]
						& ~req[3] & ~req[2] & ~req[1];
	assign correct=(tb_gnt==gnt);

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

	initial 
	begin
		$monitor("Time:%4.0f req:%b en:%b gnt:%b", $time, req, en, gnt);
		req=8'b00000000;
		en=1;
		#5	
		req = 8'b10000000;
		#5
		req = 8'b01000000;
		#5
		req = 8'b00100000;
#5
		req = 8'b00010000;
#5
		req = 8'b00001000;
		#5
		req = 8'b00000100;
		#5
		req = 8'b00000010;
		#5
		req = 8'b00000001;
		#5
		req = 8'b00000101;
		#5
		req = 8'b00000110;
		#5
		req = 8'b00001110;
		#5
		req = 8'b00001111;
#5
		req = 8'b11111111;
#5
		req = 8'b10101010;
                #5
		req = 8'b01010101;
#5
		req = 8'b11001100;
#5
		req = 8'b00110011;
#5
		req = 8'b00010001;
#5
		req = 8'b01100000;
#5
		req = 8'b11100000;
#5
		req = 8'b11100001;
#5
                en=0;
                #5
		req=8'b00000110;
                #5
		req = 8'b11111111;
#5
		req = 8'b10100000;
#5
		req = 8'b00000000;
		$finish;
 	end // initial
endmodule
