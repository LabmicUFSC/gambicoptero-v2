library ieee;
use ieee.std_logic_1164.all;
library std;
use std.env.all;

entity plasma_axi4lite_testbench is
end plasma_axi4lite_testbench;

architecture Behavioral of plasma_axi4lite_testbench is
    component plasma_axi4lite_master is
        generic(
            memory_type     : string  := "XILINX_16X";
            mult_type       : string  := "DEFAULT";
            shifter_type    : string  := "DEFAULT";
            alu_type        : string  := "DEFAULT";
            pipeline_stages : natural := 2); --2 or 3
        port(
            aclk       : in std_logic;
            areset     : in std_logic;
            -- write address channel
            awvalid    : out std_logic;
            awready    : in std_logic;
            awaddr     : out std_logic_vector(31 downto 0);
            awprot     : out std_logic_vector(2 downto 0);
            -- write data channel
            wvalid     : out std_logic;
            wready     : in std_logic;
            wdata      : out std_logic_vector(31 downto 0);
            wstrb      : out std_logic_vector(3 downto 0);
            -- write response channel
            bvalid     : in std_logic;
            bready     : out std_logic;
            bresp      : in std_logic_vector(1 downto 0);
            -- read address channel
            arvalid    : out std_logic;
            arready    : in std_logic;
            araddr     : out std_logic_vector(31 downto 0);
            arprot     : out std_logic_vector(2 downto 0);
            -- read data channel
            rvalid     : in std_logic;
            rready     : out std_logic;
            rdata      : in std_logic_vector(31 downto 0);
            rresp      : in std_logic_vector(1 downto 0);

            -- plasma cpu interrupt, externalized
            intr       : in std_logic);
    end component;

    component ram_amba_128k is
        port(
            s_aclk        : in std_logic;
            s_aresetn     : in std_logic;
            s_axi_awaddr  : in std_logic_vector(31 downto 0);
            s_axi_awvalid : in std_logic;
            s_axi_awready : out std_logic;
            s_axi_wdata   : in std_logic_vector(31 downto 0);
            s_axi_wstrb   : in std_logic_vector(3 downto 0);
            s_axi_wvalid  : in std_logic;
            s_axi_wready  : out std_logic;
            s_axi_bresp   : out std_logic_vector(1 downto 0);
            s_axi_bvalid  : out std_logic;
            s_axi_bready  : in std_logic;
            s_axi_araddr  : in std_logic_vector(31 downto 0);
            s_axi_arvalid : in std_logic;
            s_axi_arready : out std_logic;
            s_axi_rdata   : out std_logic_vector(31 downto 0);
            s_axi_rresp   : out std_logic_vector(1 downto 0);
            s_axi_rvalid  : out std_logic;
            s_axi_rready  : in std_logic);
    end component;


    signal clk_50MHz   : std_logic;
    signal sig_reset   : std_logic;
    signal sig_intr    : std_logic;

    signal sig_awvalid : std_logic;
    signal sig_awready : std_logic;
    signal sig_awaddr  : std_logic_vector(31 downto 0);
    signal sig_awprot  : std_logic_vector(2 downto 0);

    signal sig_wvalid  : std_logic;
    signal sig_wready  : std_logic;
    signal sig_wdata   : std_logic_vector(31 downto 0);
    signal sig_wstrb   : std_logic_vector(3 downto 0);

    signal sig_bvalid  : std_logic;
    signal sig_bready  : std_logic;
    signal sig_bresp   : std_logic_vector(1 downto 0);

    signal sig_arvalid : std_logic;
    signal sig_arready : std_logic;
    signal sig_araddr  : std_logic_vector(31 downto 0);
    signal sig_arprot  : std_logic_vector(2 downto 0);

    signal sig_rvalid  : std_logic;
    signal sig_rready  : std_logic;
    signal sig_rdata   : std_logic_vector(31 downto 0);
    signal sig_rresp   : std_logic_vector(1 downto 0);

begin

    plasma_amba: plasma_axi4lite_master
        generic map(
            memory_type     => "XILINX_16X",
            mult_type       => "DEFAULT",
            shifter_type    => "DEFAULT",
            alu_type        => "DEFAULT",
            pipeline_stages => 2)
        port map(
            aclk       => clk_50MHz,
            areset     => sig_reset,
            -- write address channel
            awvalid    => sig_awvalid,
            awready    => sig_awready,
            awaddr     => sig_awaddr,
            awprot     => sig_awprot,
            -- write data channel
            wvalid     => sig_wvalid,
            wready     => sig_wready,
            wdata      => sig_wdata,
            wstrb      => sig_wstrb,
            -- write response channel
            bvalid     => sig_bvalid,
            bready     => sig_bready,
            bresp      => sig_bresp,
            -- read address channel
            arvalid    => sig_arvalid,
            arready    => sig_arready,
            araddr     => sig_araddr,
            arprot     => sig_arprot,
            -- read data channel
            rvalid     => sig_rvalid,
            rready     => sig_rready,
            rdata      => sig_rdata,
            rresp      => sig_rresp,
        
            intr       => sig_intr);

    ram_amba: ram_amba_128k
        port map(
            s_aclk        => clk_50MHz,
            s_aresetn     => sig_reset,

            s_axi_awvalid => sig_awvalid,
            s_axi_awready => sig_awready,
            s_axi_awaddr  => sig_awaddr, 

            s_axi_wvalid  => sig_wvalid,
            s_axi_wready  => sig_wready,
            s_axi_wdata   => sig_wdata,
            s_axi_wstrb   => sig_wstrb,

            s_axi_bvalid  => sig_bvalid,
            s_axi_bready  => sig_bready,
            s_axi_bresp   => sig_bresp,

            s_axi_arvalid => sig_arvalid,
            s_axi_arready => sig_arready,
            s_axi_araddr  => sig_araddr,

            s_axi_rvalid  => sig_rvalid,
            s_axi_rready  => sig_rready,
            s_axi_rdata   => sig_rdata,
            s_axi_rresp   => sig_rresp);

    clk_process: process
    begin
        clk_50MHz <= '1';
        wait for 10 ns;
        clk_50MHz <= '0';
        wait for 10 ns;
    end process;


    -- do not interrupt the CPU
    sig_intr <= '0';

    tb : process
    begin
        sig_reset <= '0';
        wait for 60 ns;
        sig_reset <= '1';

        wait for 100000 ns;

        finish(0);
    end process;

end Behavioral;
