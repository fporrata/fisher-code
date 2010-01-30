module testbench();

reg [63:0] a,b;
reg quit, clock, start, reset;

wire [63:0] result;
wire done;

wire [63:0] cres = a*b;

wire correct = (cres==result)|~done;


mult m0(clock, reset, a, b, start, result, done);

always @(posedge clock)
  #2 if(!correct) begin 
    $display("Incorrect at time %4.0f",$time);
    $display("cres = %h result = %h",cres,result);
    $finish;
  end

always
begin
  #5;
  clock=~clock;
end

initial begin

  //$vcdpluson;
  $monitor("Time:%4.0f done:%b a:%h b:%h product:%h result:%h",$time,done,a,b,cres,result);
  a=2;
  b=3;
  reset=1;
  clock=0;
  start=1;

@(negedge clock);
reset=0;
@(negedge clock);
start=0;
@(posedge done);
@(negedge clock);
start=1;
a=-1;
@(negedge clock);
start=0;
@(posedge done);
@(negedge clock);
@(negedge clock);
start=1;
a=-20;
b=5;
@(negedge clock);
start=0;
@(posedge done);
@(negedge clock);
quit = 0;
quit <= #10000 1;
while(~quit)
begin
  start=1;
  a={$random,$random};
  b={$random,$random};
  @(negedge clock);
  start=0;
  @(posedge done);
  @(negedge clock);
end
$finish;
end

endmodule



  
  
