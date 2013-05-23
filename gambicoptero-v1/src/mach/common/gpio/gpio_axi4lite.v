module gpio_axi4lite(

    clk_i, 
    
    axi_rst_i,
    
    axi_awaddr_i, axi_awvalid_i, axi_awready_o, axi_wdata_i,
    axi_wstrb_i, axi_wvalid_i, axi_wready_o, axi_bresp_o,
    axi_bvalid_o, axi_bready_i, axi_araddr_i, axi_arvalid_i,
    axi_arready_o, axi_rdata_o, axi_rresp_o, axi_rvalid_o, axi_rready_i,
    
    gpio_i, gpio_o

);

    //Parameters
    localparam ADDR_WIDTH = 32;
    localparam DATA_WIDTH = 32;
    
        
    // Clk
    input clk_i;    
    
    // AXI signals
    input axi_rst_i;
    input [ADDR_WIDTH-1:0] axi_awaddr_i;
    input axi_awvalid_i;
    output axi_awready_o;
    input [DATA_WIDTH-1:0] axi_wdata_i;
    input [(DATA_WIDTH/8)-1:0] axi_wstrb_i;
    input axi_wvalid_i;
    output axi_wready_o;
    output [1:0] axi_bresp_o;
    output axi_bvalid_o;
    input axi_bready_i;
    input [ADDR_WIDTH-1:0] axi_araddr_i;
    input axi_arvalid_i;
    output axi_arready_o;
    output [DATA_WIDTH-1:0] axi_rdata_o;
    output [1:0] axi_rresp_o;
    output axi_rvalid_o;
    input axi_rready_i;
    
    input [31:0] gpio_i;
    output [31:0] gpio_o;
    
    // /////////////////////////////////////////////////////
    
    wire wb_rst;
    wire [DATA_WIDTH-1:0] wb_dat_i;
    wire [DATA_WIDTH-1:0] wb_dat_o;
    wire [ADDR_WIDTH-1:0] wb_adr;
    wire [(DATA_WIDTH/8)-1:0] wb_sel;
    wire wb_we;
    wire wb_cyc;
    wire wb_stb;
    wire wb_ack;
    wire wb_err;
    wire wb_rty;

    axi4lite_to_wishbone #(
    	.ADDR_WIDTH(ADDR_WIDTH),
    	.DATA_WIDTH(DATA_WIDTH)
    )
    axi4lite_to_wishbone(
    	.clk_i(clk_i),
    	.axi_rst_i(axi_rst_i),
    	.axi_awaddr_i(axi_awaddr_i),
    	.axi_awvalid_i(axi_awvalid_i),
    	.axi_awready_o(axi_awready_o),
    	.axi_wdata_i(axi_wdata_i),
    	.axi_wstrb_i(axi_wstrb_i),
    	.axi_wvalid_i(axi_wvalid_i),
    	.axi_wready_o(axi_wready_o),
    	.axi_bresp_o(axi_bresp_o),
    	.axi_bvalid_o(axi_bvalid_o),
    	.axi_bready_i(axi_bready_i),
    	.axi_araddr_i(axi_araddr_i),
    	.axi_arvalid_i(axi_arvalid_i),
    	.axi_arready_o(axi_arready_o),
    	.axi_rdata_o(axi_rdata_o),
    	.axi_rresp_o(axi_rresp_o),
    	.axi_rvalid_o(axi_rvalid_o),
    	.axi_rready_i(axi_rready_i),
    	.wb_rst_o(wb_rst),
    	.wb_dat_i(wb_dat_i),
    	.wb_dat_o(wb_dat_o),
    	.wb_adr_o(wb_adr),
    	.wb_sel_o(wb_sel),
    	.wb_we_o(wb_we),
    	.wb_cyc_o(wb_cyc),
    	.wb_stb_o(wb_stb),
    	.wb_ack_i(wb_ack),
    	.wb_err_i(wb_err),
    	.wb_rty_i(wb_rty)
    );
    
    very_simple_gpio very_simple_gpio(
    	.clk_i(clk_i),
    	.rst_i(wb_rst),
    	.cyc_i(wb_cyc),
    	.stb_i(wb_stb),
    	.adr_i(wb_adr[2:0]),
    	.we_i(wb_we),
    	.dat_i(wb_dat_o),
    	.dat_o(wb_dat_i),
    	.ack_o(wb_ack),
    	.gpio_i(gpio_i),
    	.gpio_o(gpio_o)
    );


endmodule
