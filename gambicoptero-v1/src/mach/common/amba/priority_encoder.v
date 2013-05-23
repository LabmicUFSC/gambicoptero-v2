module priority_encoder (
    in, out
);

    parameter INPUT_WIDTH=8;
    parameter OUTPUT_WIDTH=3;

    input  [INPUT_WIDTH-1:0]  in;
    output reg [OUTPUT_WIDTH-1:0] out;
 
    integer ii;
 
    always @(*) begin
        out = {OUTPUT_WIDTH{1'bx}};
        for(ii=0;ii<INPUT_WIDTH;ii=ii+1) if (in[ii]) out = ii;
    end
 
endmodule  