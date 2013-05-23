
module shortfifo
  #(parameter WIDTH=32)
    (input clk, input rst,
     input [WIDTH-1:0] datain,
     output [WIDTH-1:0] dataout,
     input read,
     input write,
     input clear,
     output reg full,
     output reg empty,
     output reg [4:0] space,
     output reg [4:0] occupied);
   
   reg [3:0] 	  a;
   genvar 	  i;
   
   generate
      for (i=0;i<WIDTH;i=i+1)
	begin : gen_srl16
	   SRL16E
	     srl16e(.Q(dataout[i]),
		    .A0(a[0]),.A1(a[1]),.A2(a[2]),.A3(a[3]),
		    .CE(write),.CLK(clk),.D(datain[i]));
	end
   endgenerate
   
   always @(posedge clk)
     if(rst)
       begin
	  a <= 0;
	  empty <= 1;
	  full <= 0;
       end
     else if(clear)
       begin
	  a <= 0;
	  empty <= 1;
	  full<= 0;
       end
     else if(read & ~write)
       begin
	  full <= 0;
	  if(a==0)
	    empty <= 1;
	  else
	    a <= a - 4'd1;
       end
     else if(write & ~read)
       begin
	  empty <= 0;
	  if(~empty)
	    a <= a + 4'd1;
	  if(a == 14)
	    full <= 1;
       end

   // NOTE will fail if you write into a full fifo or read from an empty one

   //////////////////////////////////////////////////////////////
   // space and occupied are used for diagnostics, not 
   // guaranteed correct
   
   //assign space = full ? 0 : empty ? 16 : 15-a;
   //assign occupied = empty ? 0 : full ? 16 : a+1;

   always @(posedge clk)
     if(rst)
       space <= 5'd16;
     else if(clear)
       space <= 5'd16;
     else if(read & ~write)
       space <= space + 5'd1;
     else if(write & ~read)
       space <= space - 5'd1;
   
   always @(posedge clk)
     if(rst)
       occupied <= 5'd0;
     else if(clear)
       occupied <= 5'd0;
     else if(read & ~write)
       occupied <= occupied - 5'd1;
     else if(write & ~read)
       occupied <= occupied + 5'd1;
      
endmodule // shortfifo
