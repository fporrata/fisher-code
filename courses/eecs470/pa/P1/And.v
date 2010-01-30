module AND2(a,out);
  input [1:0] a;
  output out;

  assign out=a[0] & a[1];
endmodule

module AND4(a,out);
  input [3:0] a;
  output out;
  
  wire [1:0]tmp;
  AND2 left(a[1:0],tmp[0]);
  AND2 right(a[3:2],tmp[1]);
  AND2 top(tmp,out);
endmodule 






