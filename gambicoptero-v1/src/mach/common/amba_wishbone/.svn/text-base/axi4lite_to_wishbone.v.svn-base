module axi4lite_to_wishbone(
    clk_i, 
    
    axi_rst_i,
    
    axi_awaddr_i, axi_awvalid_i, axi_awready_o, axi_wdata_i,
    axi_wstrb_i, axi_wvalid_i, axi_wready_o, axi_bresp_o,
    axi_bvalid_o, axi_bready_i, axi_araddr_i, axi_arvalid_i,
    axi_arready_o, axi_rdata_o, axi_rresp_o, axi_rvalid_o, axi_rready_i,
    
    wb_rst_o,
    
    wb_dat_i, wb_dat_o, wb_adr_o, wb_sel_o, wb_we_o,
    wb_cyc_o, wb_stb_o, wb_ack_i, wb_err_i, wb_rty_i
);

    
    //Parameters
    parameter ADDR_WIDTH = 32;
    parameter DATA_WIDTH = 32;
        
        
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
    
    //WB signals
    output wb_rst_o;
    input [DATA_WIDTH-1:0] wb_dat_i;
    output [DATA_WIDTH-1:0] wb_dat_o;
    output [ADDR_WIDTH-1:0] wb_adr_o;
    output [(DATA_WIDTH/8)-1:0] wb_sel_o;
    output wb_we_o;
    output wb_cyc_o;
    output wb_stb_o;
    input wb_ack_i;
    input wb_err_i;
    input wb_rty_i;
    
    
    // ///////////////////////////////////////
    
    assign wb_rst_o = ~axi_rst_i;
    
    reg [2:0] state;
    localparam ADDR = 3'd0;
    localparam WRITE_DATA = 3'd1;
    localparam WRITE_ACK = 3'd2;
    localparam READ_DATA = 3'd3;
    localparam READ_ACK = 3'd4;
    localparam READ_VALID = 3'd5;
    
    reg [ADDR_WIDTH-1:0] address;
    assign wb_adr_o = address;
    
    reg addr_ready;
    assign axi_awready_o = addr_ready;
    assign axi_arready_o = addr_ready;
    
    reg write_ready;
    assign axi_wready_o = write_ready;
    
    reg stb;
    reg we;
    reg [(DATA_WIDTH/8)-1:0] sel;
    reg [DATA_WIDTH-1:0] data;
    assign wb_we_o = we;
    assign wb_stb_o = stb;
    assign wb_cyc_o = stb;
    assign wb_sel_o = sel;
    assign wb_dat_o = data;
    assign axi_rdata_o = data;
    
    assign axi_bvalid_o = 1'b1;
    assign axi_bresp_o = 2'b0; //OK
    assign axi_rresp_o = 2'b0; //OK
    
    reg read_valid;
    assign axi_rvalid_o = read_valid;
    
    always@(posedge clk_i)
    if(wb_rst_o) begin
       state <= ADDR; 
       addr_ready <= 1'b0;
       write_ready <= 1'b0;
       stb <= 1'b0;
       we <= 1'b0;
       sel <= 0;
       data <= 0;
       read_valid <= 1'b0;
    end
    else begin
        case (state)
        
        ADDR: begin
            if(axi_awvalid_i) begin
                address <= axi_awaddr_i;
                addr_ready <= 1'b1;
                state <= WRITE_DATA;
            end
            else if(axi_arvalid_i) begin
                address <= axi_araddr_i;
                addr_ready <= 1'b1;
                state <= READ_DATA;
            end     
        end
        
        WRITE_DATA: begin
            addr_ready <= 1'b0;
            if(axi_wvalid_i) begin
                stb <= 1'b1;
                we <= 1'b1;
                sel <= axi_wstrb_i;
                data <= axi_wdata_i;
                write_ready <= 1'b1;
                state <= WRITE_ACK;
            end
        end
        
        WRITE_ACK: begin
            write_ready <= 1'b0;
            if(wb_ack_i)begin
                stb <= 1'b0;
                we <= 1'b0;
                state <= ADDR;
            end
        end
        
        
        READ_DATA: begin
            addr_ready <= 1'b0;
            stb <= 1'b1;
            state <= READ_ACK;
        end
        
        READ_ACK: begin
            if(wb_ack_i) begin
                data <= wb_dat_i;
                read_valid <= 1'b1;
                stb <= 1'b0;
                state <= READ_VALID;
            end            
        end
        
        READ_VALID: begin
            if(axi_rready_i) begin
                read_valid <= 1'b0;
                state <= ADDR;
            end
        end
            
        default: begin
            state <= ADDR;
            addr_ready <= 1'b0;
            write_ready <= 1'b0;
            stb <= 1'b0;
            we <= 1'b0;
            sel <= 0;
            data <= 0;
            read_valid <= 1'b0;
        end
        
        endcase
    
    end
    
    

endmodule
