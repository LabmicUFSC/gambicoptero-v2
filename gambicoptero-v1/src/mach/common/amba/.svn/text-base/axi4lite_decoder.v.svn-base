module axi4lite_decoder(
 //master write address channel
    m_awvalid_i,
    m_awready_o,
    m_awaddr_i,
    m_awprot_i,
    //master write data channel
    m_wvalid_i,
    m_wready_o,
    m_wdata_i,
    m_wstrb_i,
    //master write response channel
    m_bvalid_o,
    m_bready_i,
    m_bresp_o,
    //master read address channel
    m_arvalid_i,
    m_arready_o,
    m_araddr_i,
    m_arprot_i,
    //master read data channel
    m_rvalid_o,
    m_rready_i,
    m_rdata_o,
    m_rresp_o,
    
    //slave write address channel
    s_awvalid_o,
    s_awready_i,
    s_awaddr_o,
    s_awprot_o,
    //slave write data channel
    s_wvalid_o,
    s_wready_i,
    s_wdata_o,
    s_wstrb_o,
    //slave write response channel
    s_bvalid_i,
    s_bready_o,
    s_bresp_i,
    //slave read address channel
    s_arvalid_o,
    s_arready_i,
    s_araddr_o,
    s_arprot_o,
    //slave read data channel
    s_rvalid_i,
    s_rready_o,
    s_rdata_i,
    s_rresp_i

);

    // WARNING -- must make sure these are mutually exclusive!
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
    
    parameter sw = 16;
    parameter sw_log2 = 4; //must be equal to round_up(log2(sw))
    
    localparam aw = 32;
    
    
    //master write address channel
    input m_awvalid_i;
    output m_awready_o;
    input [aw-1:0] m_awaddr_i;
    input [2:0] m_awprot_i;
    //master write data channel
    input m_wvalid_i;
    output m_wready_o;
    input [aw-1:0] m_wdata_i;
    input [3:0] m_wstrb_i;
    //master write response channel
    output m_bvalid_o;
    input m_bready_i;
    output [1:0] m_bresp_o;
    //master read address channel
    input m_arvalid_i;
    output m_arready_o;
    input [aw-1:0] m_araddr_i;
    input [2:0] m_arprot_i;
    //master read data channel
    output m_rvalid_o;
    input m_rready_i;
    output [aw-1:0] m_rdata_o;
    output [1:0] m_rresp_o;
    
    //slave write address channel
    output [sw-1:0] s_awvalid_o;
    input [sw-1:0] s_awready_i;
    output [(sw*aw)-1:0] s_awaddr_o;
    output [(3*sw)-1:0] s_awprot_o;
    //slave write data channel
    output [sw-1:0] s_wvalid_o;
    input [sw-1:0] s_wready_i;
    output [(sw*aw)-1:0] s_wdata_o;
    output [(sw*4)-1:0] s_wstrb_o;
    //slave write response channel
    input [sw-1:0] s_bvalid_i;
    output [sw-1:0] s_bready_o;
    input [(sw*2)-1:0] s_bresp_i;
    //slave read address channel
    output [sw-1:0] s_arvalid_o;
    input [sw-1:0] s_arready_i;
    output [(aw*sw)-1:0] s_araddr_o;
    output [(sw*3)-1:0] s_arprot_o;
    //slave read data channel
    input [sw-1:0] s_rvalid_i;
    output [sw-1:0] s_rready_o;
    input [(aw*sw)-1:0] s_rdata_i;
    input [(sw*2)-1:0] s_rresp_i;
    
    // /////////////////////////////////////////////////////////////////q
    
    //Write side
    
    wire [sw-1:0] ssel_write;
    
    address_decoder #(
    	.s0_addr_w(s0_addr_w),
    	.s0_addr(s0_addr),
    	.s1_addr_w(s1_addr_w),
        .s1_addr(s1_addr),
    	.s2_addr_w(s2_addr_w),
        .s2_addr(s2_addr),
    	.s3_addr_w(s3_addr_w),
        .s3_addr(s3_addr),
    	.s4_addr_w(s4_addr_w),
        .s4_addr(s4_addr),
    	.s5_addr_w(s5_addr_w),
        .s5_addr(s5_addr),
    	.s6_addr_w(s6_addr_w),
        .s6_addr(s6_addr),
    	.s7_addr_w(s7_addr_w),
        .s7_addr(s7_addr),
    	.s8_addr_w(s8_addr_w),
        .s8_addr(s8_addr),
    	.s9_addr_w(s9_addr_w),
        .s9_addr(s9_addr),
    	.s10_addr_w(s10_addr_w),
        .s10_addr(s10_addr),
    	.s11_addr_w(s11_addr_w),
        .s11_addr(s11_addr),
    	.s12_addr_w(s12_addr_w),
        .s12_addr(s12_addr),
    	.s13_addr_w(s13_addr_w),
        .s13_addr(s13_addr),
    	.s14_addr_w(s14_addr_w),
        .s14_addr(s14_addr),
    	.s15_addr_w(s15_addr_w),
        .s15_addr(s15_addr),
    	.aw(aw),
    	.sw(sw)
    )
    write_decoder(
    	.addr_i(m_awaddr_i),
    	.ssel_o(ssel_write)
    );
    
    wire [sw_log2-1:0] ssel_write_idx;
    priority_encoder #(
    	.INPUT_WIDTH(sw),
    	.OUTPUT_WIDTH(sw_log2)
    )
    encoder_write(
    	.in(ssel_write),
    	.out(ssel_write_idx)
    );
    
    assign m_awready_o = s_awready_i[ssel_write_idx];
    assign m_wready_o = s_wready_i[ssel_write_idx];
    assign m_bvalid_o = s_bvalid_i[ssel_write_idx];
    //assign m_bresp_o = s_bresp_i[((ssel_write_idx+1)*2)-1:ssel_write_idx*2];
    generate if (sw < (1<<sw_log2)) begin
    amba_mux #(.SIZE(1), .N_INPUTS(sw_log2))
    bresp_mux(
    	.sel(ssel_write_idx),
    	.in({{(((1<<sw_log2)*2)-(sw*2)){1'b0}},s_bresp_i}),
    	.out(m_bresp_o)
    );
    end else begin
    amba_mux #(.SIZE(1), .N_INPUTS(sw_log2))
    bresp_mux(
        .sel(ssel_write_idx),
        .in(s_bresp_i),
        .out(m_bresp_o)
    );
    end endgenerate
    
    assign s_awaddr_o = {sw{m_awaddr_i}};
    assign s_awprot_o = {sw{m_awprot_i}};
    assign s_wdata_o = {sw{m_wdata_i}};
    assign s_wstrb_o = {sw{m_wstrb_i}};
    
    genvar i;
    generate for (i = 0; i < sw; i = i + 1) begin: m_to_s_write
        assign s_awvalid_o[i] = ssel_write[i] ? m_awvalid_i : 1'b0;   
        assign s_wvalid_o[i] = ssel_write[i] ? m_wvalid_i : 1'b0;
        assign s_bready_o[i] = ssel_write[i] ? m_bready_i : 1'b0;
    end endgenerate
    
        
    
    //Read side
    
    wire [sw-1:0] ssel_read;
    
    address_decoder #(
        .s0_addr_w(s0_addr_w),
        .s0_addr(s0_addr),
        .s1_addr_w(s1_addr_w),
        .s1_addr(s1_addr),
        .s2_addr_w(s2_addr_w),
        .s2_addr(s2_addr),
        .s3_addr_w(s3_addr_w),
        .s3_addr(s3_addr),
        .s4_addr_w(s4_addr_w),
        .s4_addr(s4_addr),
        .s5_addr_w(s5_addr_w),
        .s5_addr(s5_addr),
        .s6_addr_w(s6_addr_w),
        .s6_addr(s6_addr),
        .s7_addr_w(s7_addr_w),
        .s7_addr(s7_addr),
        .s8_addr_w(s8_addr_w),
        .s8_addr(s8_addr),
        .s9_addr_w(s9_addr_w),
        .s9_addr(s9_addr),
        .s10_addr_w(s10_addr_w),
        .s10_addr(s10_addr),
        .s11_addr_w(s11_addr_w),
        .s11_addr(s11_addr),
        .s12_addr_w(s12_addr_w),
        .s12_addr(s12_addr),
        .s13_addr_w(s13_addr_w),
        .s13_addr(s13_addr),
        .s14_addr_w(s14_addr_w),
        .s14_addr(s14_addr),
        .s15_addr_w(s15_addr_w),
        .s15_addr(s15_addr),
        .aw(aw),
        .sw(sw)
    )
    read_decoder(
        .addr_i(m_araddr_i),
        .ssel_o(ssel_read)
    );
    
    wire [sw_log2-1:0] ssel_read_idx;
    priority_encoder #(
        .INPUT_WIDTH(sw),
        .OUTPUT_WIDTH(sw_log2)
    )
    encoder_read(
        .in(ssel_read),
        .out(ssel_read_idx)
    );
    
    
    assign m_arready_o = s_arready_i[ssel_read_idx];
    assign m_rvalid_o = s_rvalid_i[ssel_read_idx];
    //assign m_rdata_o = s_rdata_i[((ssel_read_idx+1)*aw)-1:ssel_read_idx*aw];
    generate if (sw < (1<<sw_log2)) begin
    amba_mux #(.SIZE(5), .N_INPUTS(sw_log2))
    rdata_mux(
        .sel(ssel_read_idx),
        .in({{(((1<<sw_log2)*aw)-(sw*aw)){1'b0}},s_rdata_i}),
        .out(m_rdata_o)
    );
    end else begin
    amba_mux #(.SIZE(5), .N_INPUTS(sw_log2))
    rdata_mux(
        .sel(ssel_read_idx),
        .in(s_rdata_i),
        .out(m_rdata_o)
    );
    end endgenerate
    //assign m_rresp_o = s_rresp_i[((ssel_read_idx+1)*2)-1:ssel_read_idx*2];
    generate if (sw < (1<<sw_log2)) begin
    amba_mux #(.SIZE(1), .N_INPUTS(sw_log2))
    rresp_mux(
        .sel(ssel_read_idx),
        .in({{(((1<<sw_log2)*2)-(sw*2)){1'b0}},s_rresp_i}),
        .out(m_rresp_o)
    );
    end else begin
    amba_mux #(.SIZE(1), .N_INPUTS(sw_log2))
    rresp_mux(
        .sel(ssel_read_idx),
        .in(s_rresp_i),
        .out(m_rresp_o)
    );
    end endgenerate
    
    assign s_araddr_o = {sw{m_araddr_i}};
    assign s_arprot_o = {sw{m_arprot_i}};
    
    genvar j;
    generate for (j = 0; j < sw; j = j + 1) begin: m_to_s_read
        assign s_arvalid_o[j] = ssel_read[j] ? m_arvalid_i : 1'b0;   
        assign s_rready_o[j] = ssel_read[j] ? m_rready_i : 1'b0;
    end endgenerate
    
        

endmodule
