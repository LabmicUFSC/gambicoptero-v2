
//Instantiates the Xilinx's UART

module axi_uart_xilinx (
     S_AXI_ACLK, S_AXI_ARESETN, Interrupt,
     
     S_AXI_AWADDR, S_AXI_AWVALID, S_AXI_AWREADY, S_AXI_WDATA, S_AXI_WSTRB,
     S_AXI_WVALID, S_AXI_WREADY, S_AXI_BRESP, S_AXI_BVALID, S_AXI_BREADY,
     S_AXI_ARADDR, S_AXI_ARVALID, S_AXI_ARREADY, S_AXI_RDATA, S_AXI_RRESP,
     S_AXI_RVALID, S_AXI_RREADY,

     RX, TX
);

    parameter C_FAMILY = "virtex6";
    parameter C_S_AXI_ACLK_FREQ_HZ  = 100000000;
    // AXI Parameters
    parameter C_BASEADDR = 32'h00000000;
    parameter C_HIGHADDR = 32'hFFFFFFFF;
    localparam C_S_AXI_ADDR_WIDTH = 32;
    localparam C_S_AXI_DATA_WIDTH = 32;
    //  UARTLite Parameters
    parameter C_BAUDRATE = 9600;
    parameter C_DATA_BITS = 8;
    parameter C_USE_PARITY = 0;
    parameter C_ODD_PARITY = 0;
    
    // System signals
    input S_AXI_ACLK;
    input S_AXI_ARESETN;
    output Interrupt;

    // AXI signals
    input [C_S_AXI_ADDR_WIDTH-1:0] S_AXI_AWADDR;
    input S_AXI_AWVALID;
    output S_AXI_AWREADY;
    input [C_S_AXI_DATA_WIDTH-1:0] S_AXI_WDATA;
    input [(C_S_AXI_DATA_WIDTH/8)-1:0] S_AXI_WSTRB;
    input S_AXI_WVALID;
    output S_AXI_WREADY;
    output [1:0] S_AXI_BRESP;
    output S_AXI_BVALID;
    input S_AXI_BREADY;
    input [C_S_AXI_ADDR_WIDTH-1:0] S_AXI_ARADDR;
    input S_AXI_ARVALID;
    output S_AXI_ARREADY;
    output [C_S_AXI_DATA_WIDTH-1:0] S_AXI_RDATA;
    output [1:0] S_AXI_RRESP;
    output S_AXI_RVALID;
    input S_AXI_RREADY;

    // UARTLite Interface Signals
    input RX;
    output TX;
    
    axi_uartlite #(
    	.C_FAMILY(C_FAMILY),
    	.C_S_AXI_ACLK_FREQ_HZ(C_S_AXI_ACLK_FREQ_HZ),
    	.C_BASEADDR(C_BASEADDR),
    	.C_HIGHADDR(C_HIGHADDR),
    	.C_S_AXI_ADDR_WIDTH(C_S_AXI_ADDR_WIDTH),
    	.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
    	.C_BAUDRATE(C_BAUDRATE),
    	.C_DATA_BITS(C_DATA_BITS),
    	.C_USE_PARITY(C_USE_PARITY),
    	.C_ODD_PARITY(C_ODD_PARITY)
    )
    uart_xilinx(
    	.S_AXI_ACLK(S_AXI_ACLK),
    	.S_AXI_ARESETN(S_AXI_ARESETN),
    	.Interrupt(Interrupt),
    	.S_AXI_AWADDR(S_AXI_AWADDR),
    	.S_AXI_AWVALID(S_AXI_AWVALID),
    	.S_AXI_AWREADY(S_AXI_AWREADY),
    	.S_AXI_WDATA(S_AXI_WDATA),
    	.S_AXI_WSTRB(S_AXI_WSTRB),
    	.S_AXI_WVALID(S_AXI_WVALID),
    	.S_AXI_WREADY(S_AXI_WREADY),
    	.S_AXI_BRESP(S_AXI_BRESP),
    	.S_AXI_BVALID(S_AXI_BVALID),
    	.S_AXI_BREADY(S_AXI_BREADY),
    	.S_AXI_ARADDR(S_AXI_ARADDR),
    	.S_AXI_ARVALID(S_AXI_ARVALID),
    	.S_AXI_ARREADY(S_AXI_ARREADY),
    	.S_AXI_RDATA(S_AXI_RDATA),
    	.S_AXI_RRESP(S_AXI_RRESP),
    	.S_AXI_RVALID(S_AXI_RVALID),
    	.S_AXI_RREADY(S_AXI_RREADY),
    	.RX(RX),
    	.TX(TX)
    );
    
endmodule