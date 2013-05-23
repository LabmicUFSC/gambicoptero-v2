//
// Very basic 32bit GPIO core - No data direction registers
//
//
// Registers:
//
// 0x00: Output register
//
// 0x04: Input register
//

module very_simple_gpio(
  clk_i, rst_i, cyc_i, stb_i, adr_i, we_i, dat_i, dat_o, ack_o,
  gpio_i, gpio_o
);

  // 32bit WISHBONE bus slave interface
  input         clk_i;         // clock
  input         rst_i;         // reset (asynchronous active low)
  input         cyc_i;         // cycle
  input         stb_i;         // strobe
  input  [2:0]  adr_i;         // address adr_i[3]
  input         we_i;          // write enable
  input  [31:0] dat_i;         // data output
  output reg [31:0] dat_o;         // data input
  output reg       ack_o;         // normal bus termination

  // GPIO pins
  input  [31:0] gpio_i;
  output  [31:0] gpio_o;

  
  reg [31:0] out_reg, in_reg;                  // Regs
  
  always@(posedge clk_i)
  if(rst_i)
  begin
    out_reg <= 32'b0;
    dat_o <= 32'b0;
    ack_o <= 0;
  end
  else if (cyc_i & stb_i)
  begin
    case (adr_i[2])
        0: begin
            if (we_i) out_reg <= dat_i;
            else dat_o <= out_reg;
            ack_o <= 1;
        end
        1: begin
            dat_o <= in_reg;
            ack_o <= 1;
        end
        default: begin
            dat_o <= 32'b0;
            ack_o <= 1;
        end
    endcase
  end
  else
    ack_o <= 0;
  
  always@(posedge clk_i)
  if(rst_i)
    in_reg <= 32'b0;
  else 
    in_reg <= gpio_i;
    
    
  assign gpio_o = out_reg; 


endmodule
