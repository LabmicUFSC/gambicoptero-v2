module address_decoder(
   addr_i, ssel_o
);

    parameter s0_addr_w = 4;          
    parameter s0_addr = 32'h0;
    parameter s1_addr_w = 4;          
    parameter s1_addr = 32'h0;
    parameter s2_addr_w = 4;          
    parameter s2_addr = 32'h0;
    parameter s3_addr_w = 4;          
    parameter s3_addr = 32'h0;
    parameter s4_addr_w = 4;          
    parameter s4_addr = 32'h0;
    parameter s5_addr_w = 4;          
    parameter s5_addr = 32'h0;
    parameter s6_addr_w = 4;          
    parameter s6_addr = 32'h0;
    parameter s7_addr_w = 4;          
    parameter s7_addr = 32'h0;
    parameter s8_addr_w = 4;          
    parameter s8_addr = 32'h0;
    parameter s9_addr_w = 4;          
    parameter s9_addr = 32'h0;
    parameter s10_addr_w = 4;          
    parameter s10_addr = 32'h0;
    parameter s11_addr_w = 4;          
    parameter s11_addr = 32'h0;
    parameter s12_addr_w = 4;          
    parameter s12_addr = 32'h0;
    parameter s13_addr_w = 4;          
    parameter s13_addr = 32'h0;
    parameter s14_addr_w = 4;          
    parameter s14_addr = 32'h0;
    parameter s15_addr_w = 4;          
    parameter s15_addr = 32'h0;
    
    parameter aw = 32;
    parameter sw = 16;
    
   input [aw-1:0] addr_i;
   output [sw-1:0] ssel_o;
       
   // Address decode logic
   // WARNING -- must make sure these are mutually exclusive!
   generate if(sw >= 1) begin:ssel0 assign ssel_o[0] = (addr_i[aw -1 : aw - s0_addr_w ] == s0_addr); end endgenerate
   generate if(sw >= 2) begin:ssel1 assign ssel_o[1] = (addr_i[aw -1 : aw - s1_addr_w ] == s1_addr); end endgenerate
   generate if(sw >= 3) begin:ssel2 assign ssel_o[2] = (addr_i[aw -1 : aw - s2_addr_w ] == s2_addr); end endgenerate
   generate if(sw >= 4) begin:ssel3 assign ssel_o[3] = (addr_i[aw -1 : aw - s3_addr_w ] == s3_addr); end endgenerate
   generate if(sw >= 5) begin:ssel4 assign ssel_o[4] = (addr_i[aw -1 : aw - s4_addr_w ] == s4_addr); end endgenerate
   generate if(sw >= 6) begin:ssel5 assign ssel_o[5] = (addr_i[aw -1 : aw - s5_addr_w ] == s5_addr); end endgenerate
   generate if(sw >= 7) begin:ssel6 assign ssel_o[6] = (addr_i[aw -1 : aw - s6_addr_w ] == s6_addr); end endgenerate
   generate if(sw >= 8) begin:ssel7 assign ssel_o[7] = (addr_i[aw -1 : aw - s7_addr_w ] == s7_addr); end endgenerate
   generate if(sw >= 9) begin:ssel8 assign ssel_o[8] = (addr_i[aw -1 : aw - s8_addr_w ] == s8_addr); end endgenerate
   generate if(sw >= 10) begin:ssel9 assign ssel_o[9] = (addr_i[aw -1 : aw - s9_addr_w ] == s9_addr); end endgenerate
   generate if(sw >= 11) begin:ssel10 assign ssel_o[10] = (addr_i[aw -1 : aw - s10_addr_w ] == s10_addr); end endgenerate
   generate if(sw >= 12) begin:ssel11 assign ssel_o[11] = (addr_i[aw -1 : aw - s11_addr_w ] == s11_addr); end endgenerate
   generate if(sw >= 13) begin:ssel12 assign ssel_o[12] = (addr_i[aw -1 : aw - s12_addr_w ] == s12_addr); end endgenerate
   generate if(sw >= 14) begin:ssel13 assign ssel_o[13] = (addr_i[aw -1 : aw - s13_addr_w ] == s13_addr); end endgenerate
   generate if(sw >= 15) begin:ssel14 assign ssel_o[14] = (addr_i[aw -1 : aw - s14_addr_w ] == s14_addr); end endgenerate
   generate if(sw >= 16) begin:ssel15 assign ssel_o[15] = (addr_i[aw -1 : aw - s15_addr_w ] == s15_addr); end endgenerate
   

endmodule
