`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

module plasma_ml605
    (
        // Misc, debug
        output [7:0] gpio_leds, //GPIO leds
        input  [7:0] gpio_sws, //GPIO switches
        output [4:0] dir_leds, //C,E,N,S,W leds
        input  [4:0] dir_btns, //C,E,N,S,W buttons

        // UART
        output uart_tx_o,
        input uart_rx_i,

        // Clocks
        input clk_fpga_p,  // Diff - 200 MHz
        input clk_fpga_n
    );

    wire 	clk_100MHz, clk_50MHz;

    // Main clock manager
    // Input: diff 200 MHz
    // Out1: 100 MHz
    // Out2: 50 MHz
    clk_xlnx_100M_diff clks_main
    (
        // Clock in ports
        .CLK_IN1_P          (clk_fpga_p),   // IN
        .CLK_IN1_N          (clk_fpga_n),   // IN
        // Clock out ports
        .CLK_OUT1           (clk_100MHz),   // OUT
        .CLK_OUT2           (clk_50MHz)     // OUT 
    );
     

    // Plasma ////////////////////////
    wire reset = dir_btns[4];

    wire [18:0] gpio_others;

    wire [31:2] extram_address;
    wire [3:0] extram_byte_we;
    wire [31:0] extram_data_write;
    wire [31:0] extram_data_read;
    wire extram_mem_pause;
    wire extram_no_ddr_start;
    wire extram_no_ddr_stop; 

    plasma #(
        .memory_type("XILINX_16X"),
        .log_file("UNUSED"),
        .ethernet(0),
        .use_cache(0))
    plasma (
        .clk(clk_50MHz),
        .reset(reset),
        .uart_write(uart_tx_o),
        .uart_read(uart_rx_i),

        .address(extram_address),
        .byte_we(extram_byte_we),
        .data_write(extram_data_write),
        .data_read(extram_data_read),
        .mem_pause_in(extram_mem_pause),
        .no_ddr_start(extram_no_ddr_start),
        .no_ddr_stop(extram_no_ddr_stop),

        /*
        .address(),
        .byte_we(),
        .data_write(),
        .data_read(32'b0),
        .mem_pause_in(1'b0),
        .no_ddr_start(),
        .no_ddr_stop(),
        */

        .gpio0_out({gpio_others, dir_leds, gpio_leds}),
        .gpioA_in({19'b0, dir_btns, gpio_sws})
     );
     
    fake_external_ram
    fake_external_ram (
        .clk_i(clk_100MHz),
        .address(extram_address),
        .byte_we(extram_byte_we),
        .data_write(extram_data_write),
        .data_read(extram_data_read),
        .mem_pause_out(extram_mem_pause),
        .no_ddr_start(extram_no_ddr_start),
        .no_ddr_stop(extram_no_ddr_stop)
    );
     
    // ///////////////////////////////////////////


endmodule
