
// Single port RAM with external DDR interface
//implemented using coregen generated primitives. Size is fixed to 1 MB

module fake_external_ram 
	
  (
   input clk_i,
   
   input [31:2] address,
   input [3:0] byte_we, 
   input [31:0] data_write,
   output [31:0] data_read,
   output mem_pause_out,
   input no_ddr_start,
   input no_ddr_stop   
);
   
   parameter AWIDTH=20;
   parameter RAM_SIZE=1024*1024;
   
   wire [AWIDTH-1:2] adr_i = address[AWIDTH-1:2];
   wire [31:0] dat_i = data_write;
   wire [31:0] dat_o;
   wire we_i = 1'b1;
   wire en_i = 1'b1;
   wire [3:0] sel_i = byte_we;
   
   assign mem_pause_out = 1'b0;
   assign data_read = dat_o;

   ram_xlnx_256k ram3 (
	.clka(clk_i),
	.ena(en_i),
	.wea(we_i & sel_i[3]), 
	.addra(adr_i[AWIDTH-1:2]),  
	.dina(dat_i[31:24]),  
	.douta(dat_o[31:24]));
   
   ram_xlnx_256k ram2 (
	.clka(clk_i),
	.ena(en_i),
	.wea(we_i & sel_i[2]), 
	.addra(adr_i[AWIDTH-1:2]),  
	.dina(dat_i[23:16]),  
	.douta(dat_o[23:16]));
   
   ram_xlnx_256k ram1 (
	.clka(clk_i),
	.ena(en_i),
	.wea(we_i & sel_i[1]), 
	.addra(adr_i[AWIDTH-1:2]),  
	.dina(dat_i[15:8]),  
	.douta(dat_o[15:8]));
   
   ram_xlnx_256k ram0 (
	.clka(clk_i),
	.ena(en_i),
	.wea(we_i & sel_i[0]), 
	.addra(adr_i[AWIDTH-1:2]),  
	.dina(dat_i[7:0]),  
	.douta(dat_o[7:0]));


//end
   
//endgenerate
   
endmodule // dpram32


