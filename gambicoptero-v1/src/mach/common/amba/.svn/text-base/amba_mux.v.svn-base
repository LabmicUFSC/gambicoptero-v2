//Parameterizable mux
//Params in 2^Value
module amba_mux (
    sel, in, out
);

    parameter SIZE = 0;
    parameter N_INPUTS = 1;
    
    input wire [N_INPUTS-1:0] sel;
    input wire [((1<<SIZE)*(1<<N_INPUTS))-1:0] in;
    output wire [(1<<SIZE)-1:0] out;

    genvar i;
    generate
        for(i = 0; i < (1<<SIZE); i = i + 1)
        begin: gen_mux_out
            assign out[i] = in[sel*(1<<SIZE)+i];
        end 
    endgenerate    
    
endmodule