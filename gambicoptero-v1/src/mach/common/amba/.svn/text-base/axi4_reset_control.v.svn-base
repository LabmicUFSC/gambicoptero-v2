module axi4_reset_control(
    input clk_i,
    input ext_reset_i,
    output axi_reset_o
);

    reg [8:0] ctrl = 9'b0;
    
    reg reset = 1'b0;
    
    assign axi_reset_o = reset;
    
    always@(posedge clk_i)
        if(ctrl[8]) begin
            reset <= ext_reset_i;
            ctrl <= 9'b1_1111_1111;
        end
        else begin
            reset <= 1'b0;
            ctrl <= ctrl + 9'b1;
        end  

endmodule
