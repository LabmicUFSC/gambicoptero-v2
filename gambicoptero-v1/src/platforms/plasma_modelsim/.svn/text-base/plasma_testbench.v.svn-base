`timescale 1ns / 1ps

module plasma_testbench;
    // Clocks
    reg         clk_100MHz;
    wire        clk_50MHz;

    reg rst = 1;
      
    initial clk_100MHz = 0;
    always #5 clk_100MHz = ~clk_100MHz;

    wire        div_clk;
    reg [7:0]   div_ctr = 0;

    assign      clk_50MHz = div_clk;

    always @(posedge clk_100MHz)
     div_ctr <= div_ctr + 1;
    assign      div_clk = div_ctr[0];

    reg [63:0] clk_c;
    initial clk_c = 0;
    always@(posedge clk_50MHz)
       clk_c <= clk_c + 64'd1;
           
    // dump
    //initial begin
    //    $dumpfile("trace.lxt");
    //    $dumpvars(0,plasma_testbench);
    //end

    plasma #(.memory_type("TRI_PORT_X"),
        .log_file("uart_trace.log"),
        .ethernet(0),
        .use_cache(1))
    plasma (
        .clk(clk_50MHz),
        .reset(rst),
        .uart_write(),
        .uart_read(1'b0),

        .address(),
        .byte_we(),
        .data_write(),
        .data_read(32'b0),
        .mem_pause_in(1'b0),
        .no_ddr_start(),
        .no_ddr_stop(),

        .gpio0_out(),
        .gpioA_in(32'b0));

    initial begin
        #100 rst = 0;
        #250000000 $finish; 
    end

    //prints simulation time
    always #1000000 $monitor("Time in ns ",$time);

endmodule
