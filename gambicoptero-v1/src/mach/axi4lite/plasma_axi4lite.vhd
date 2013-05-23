library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.log2;
use ieee.math_real.ceil;

-- Memory Map:
--   0x00000000 - 0x0fffffff   Internal RAM (4KB - 256MB)
--   0x10000000 - 0x1fffffff   External RAM (1MB - 256MB)
--   All peripherals have a 1 KB address space
--   0x80000000  Uart
--   0x80000400  GPIO
--   0x80000800  Timer
--   0x80000C00  PIC

entity plasma_axi4lite is
   generic(
        CLK_FREQ : integer := 50_000_000);

   port(clk_i       : in std_logic;
        reset_i     : in std_logic;

        uart_tx_o   : out std_logic;
        uart_rx_i   : in std_logic;
        uart_baud_o : out std_logic;
        
        gpio_i  : in  std_logic_vector(31 downto 0);
        gpio_o  : out  std_logic_vector(31 downto 0));

end plasma_axi4lite;

architecture RTL of plasma_axi4lite is
    constant N_SLAVES       : integer := 6;
    constant N_SLAVES_LOG   : integer := integer(ceil(log2(real(N_SLAVES))));

    constant SLAVE0_ADDR_W  : integer := 4;
    constant SLAVE0_ADDR    : std_logic_vector(SLAVE0_ADDR_W-1 downto 0) := b"0000";

    constant SLAVE1_ADDR_W  : integer := 4;
    constant SLAVE1_ADDR    : std_logic_vector(SLAVE1_ADDR_W-1 downto 0) := b"0001";

    constant SLAVE2_ADDR_W  : integer := 22;
    constant SLAVE2_ADDR    : std_logic_vector(SLAVE2_ADDR_W-1 downto 0) 
        := b"1000" & b"0000" & b"0000" & b"0000" & b"0000" & b"00";
        
    constant SLAVE3_ADDR_W  : integer := 22;
    constant SLAVE3_ADDR    : std_logic_vector(SLAVE3_ADDR_W-1 downto 0) 
        := b"1000" & b"0000" & b"0000" & b"0000" & b"0000" & b"01";
        
    constant SLAVE4_ADDR_W  : integer := 22;
    constant SLAVE4_ADDR    : std_logic_vector(SLAVE4_ADDR_W-1 downto 0) 
        := b"1000" & b"0000" & b"0000" & b"0000" & b"0000" & b"10";
        
    constant SLAVE5_ADDR_W  : integer := 22;
    constant SLAVE5_ADDR    : std_logic_vector(SLAVE5_ADDR_W-1 downto 0) 
        := b"1000" & b"0000" & b"0000" & b"0000" & b"0000" & b"11";

    component axi4lite_decoder is
        generic(
            sw          : integer;
            sw_log2     : integer;
            s0_addr_w   : integer; 
            s0_addr     : std_logic_vector(SLAVE0_ADDR_W-1 downto 0);
            s1_addr_w   : integer; 
            s1_addr     : std_logic_vector(SLAVE1_ADDR_W-1 downto 0);
            s2_addr_w   : integer; 
            s2_addr     : std_logic_vector(SLAVE2_ADDR_W-1 downto 0);
            s3_addr_w   : integer; 
            s3_addr     : std_logic_vector(SLAVE3_ADDR_W-1 downto 0);
            s4_addr_w   : integer; 
            s4_addr     : std_logic_vector(SLAVE4_ADDR_W-1 downto 0);
            s5_addr_w   : integer; 
            s5_addr     : std_logic_vector(SLAVE5_ADDR_W-1 downto 0));
        port(
            -- master write address channel
            m_awvalid_i    : in std_logic;
            m_awready_o    : out std_logic;
            m_awaddr_i     : in std_logic_vector(31 downto 0);
            m_awprot_i     : in std_logic_vector(2 downto 0);
            -- master write data channel
            m_wvalid_i     : in std_logic;
            m_wready_o     : out std_logic;
            m_wdata_i      : in std_logic_vector(31 downto 0);
            m_wstrb_i      : in std_logic_vector(3 downto 0);
            -- master write response channel
            m_bvalid_o     : out std_logic;
            m_bready_i     : in std_logic;
            m_bresp_o      : out std_logic_vector(1 downto 0);
            -- master read address channel
            m_arvalid_i    : in std_logic;
            m_arready_o    : out std_logic;
            m_araddr_i     : in std_logic_vector(31 downto 0);
            m_arprot_i     : in std_logic_vector(2 downto 0);
            -- master read data channel
            m_rvalid_o     : out std_logic;
            m_rready_i     : in std_logic;
            m_rdata_o      : out std_logic_vector(31 downto 0);
            m_rresp_o      : out std_logic_vector(1 downto 0);
            -- slaves write address channel
            s_awvalid_o    : out std_logic_vector(N_SLAVES-1 downto 0);
            s_awready_i    : in std_logic_vector(N_SLAVES-1 downto 0);
            s_awaddr_o     : out std_logic_vector((N_SLAVES*32)-1 downto 0);
            s_awprot_o     : out std_logic_vector((N_SLAVES*3)-1 downto 0);
            -- slaves write data channel
            s_wvalid_o     : out std_logic_vector(N_SLAVES-1 downto 0);
            s_wready_i     : in std_logic_vector(N_SLAVES-1 downto 0);
            s_wdata_o      : out std_logic_vector((N_SLAVES*32)-1 downto 0);
            s_wstrb_o      : out std_logic_vector((N_SLAVES*4)-1 downto 0);
            -- slaves write response channel
            s_bvalid_i     : in std_logic_vector(N_SLAVES-1 downto 0);
            s_bready_o     : out std_logic_vector(N_SLAVES-1 downto 0);
            s_bresp_i      : in std_logic_vector((N_SLAVES*2)-1 downto 0);
            -- slaves read address channel
            s_arvalid_o    : out std_logic_vector(N_SLAVES-1 downto 0);
            s_arready_i    : in std_logic_vector(N_SLAVES-1 downto 0);
            s_araddr_o     : out std_logic_vector((N_SLAVES*32)-1 downto 0);
            s_arprot_o     : out std_logic_vector((N_SLAVES*3)-1 downto 0);
            -- slaves read data channel
            s_rvalid_i     : in std_logic_vector(N_SLAVES-1 downto 0);
            s_rready_o     : out std_logic_vector(N_SLAVES-1 downto 0);
            s_rdata_i      : in std_logic_vector((N_SLAVES*32)-1 downto 0);
            s_rresp_i      : in std_logic_vector((N_SLAVES*2)-1 downto 0));
    end component axi4lite_decoder;

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
    
    component ram_amba_1024k is
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

    component simple_uart_axi4lite is
        generic(
            TXDEPTH : integer;
            RXDEPTH : integer);
        port(
            -- System signals
            clk_i       : in  std_logic;
            axi_rst_i   : in  std_logic;
            rx_int_o    : out std_logic;
            tx_int_o    : out std_logic;
            -- AXI signals
            axi_awaddr_i          : in  std_logic_vector(31 downto 0);
            axi_awvalid_i         : in  std_logic;
            axi_awready_o         : out std_logic;
            axi_wdata_i           : in  std_logic_vector(31 downto 0);
            axi_wstrb_i           : in  std_logic_vector(3 downto 0);
            axi_wvalid_i          : in  std_logic;
            axi_wready_o          : out std_logic;
            axi_bresp_o           : out std_logic_vector(1 downto 0);
            axi_bvalid_o          : out std_logic;
            axi_bready_i          : in  std_logic;
            axi_araddr_i          : in  std_logic_vector(31 downto 0);
            axi_arvalid_i         : in  std_logic;
            axi_arready_o         : out std_logic;
            axi_rdata_o           : out std_logic_vector(31 downto 0);
            axi_rresp_o           : out std_logic_vector(1 downto 0);
            axi_rvalid_o          : out std_logic;
            axi_rready_i          : in  std_logic;
            -- UARTLite Interface Signals
            rx_i    : in  std_logic;
            tx_o    : out std_logic;
            baud_o  : out std_logic);
    end component;
    
    component gpio_axi4lite is
        port(
            -- AXI signals
            clk_i       : in  std_logic;
            axi_rst_i   : in  std_logic;
            axi_awaddr_i          : in  std_logic_vector(31 downto 0);
            axi_awvalid_i         : in  std_logic;
            axi_awready_o         : out std_logic;
            axi_wdata_i           : in  std_logic_vector(31 downto 0);
            axi_wstrb_i           : in  std_logic_vector(3 downto 0);
            axi_wvalid_i          : in  std_logic;
            axi_wready_o          : out std_logic;
            axi_bresp_o           : out std_logic_vector(1 downto 0);
            axi_bvalid_o          : out std_logic;
            axi_bready_i          : in  std_logic;
            axi_araddr_i          : in  std_logic_vector(31 downto 0);
            axi_arvalid_i         : in  std_logic;
            axi_arready_o         : out std_logic;
            axi_rdata_o           : out std_logic_vector(31 downto 0);
            axi_rresp_o           : out std_logic_vector(1 downto 0);
            axi_rvalid_o          : out std_logic;
            axi_rready_i          : in  std_logic;
            -- GPIO
            gpio_i  : in  std_logic_vector(31 downto 0);
            gpio_o  : out  std_logic_vector(31 downto 0));
    end component;

    component timer_axi4lite is
        port(
            -- AXI signals
            clk_i       : in  std_logic;
            axi_rst_i   : in  std_logic;
            axi_awaddr_i          : in  std_logic_vector(31 downto 0);
            axi_awvalid_i         : in  std_logic;
            axi_awready_o         : out std_logic;
            axi_wdata_i           : in  std_logic_vector(31 downto 0);
            axi_wstrb_i           : in  std_logic_vector(3 downto 0);
            axi_wvalid_i          : in  std_logic;
            axi_wready_o          : out std_logic;
            axi_bresp_o           : out std_logic_vector(1 downto 0);
            axi_bvalid_o          : out std_logic;
            axi_bready_i          : in  std_logic;
            axi_araddr_i          : in  std_logic_vector(31 downto 0);
            axi_arvalid_i         : in  std_logic;
            axi_arready_o         : out std_logic;
            axi_rdata_o           : out std_logic_vector(31 downto 0);
            axi_rresp_o           : out std_logic_vector(1 downto 0);
            axi_rvalid_o          : out std_logic;
            axi_rready_i          : in  std_logic;
            -- Int
            int_o  : out  std_logic);
    end component;
    
    component pic_axi4lite is
        port(
            -- AXI signals
            clk_i       : in  std_logic;
            axi_rst_i   : in  std_logic;
            axi_awaddr_i          : in  std_logic_vector(31 downto 0);
            axi_awvalid_i         : in  std_logic;
            axi_awready_o         : out std_logic;
            axi_wdata_i           : in  std_logic_vector(31 downto 0);
            axi_wstrb_i           : in  std_logic_vector(3 downto 0);
            axi_wvalid_i          : in  std_logic;
            axi_wready_o          : out std_logic;
            axi_bresp_o           : out std_logic_vector(1 downto 0);
            axi_bvalid_o          : out std_logic;
            axi_bready_i          : in  std_logic;
            axi_araddr_i          : in  std_logic_vector(31 downto 0);
            axi_arvalid_i         : in  std_logic;
            axi_arready_o         : out std_logic;
            axi_rdata_o           : out std_logic_vector(31 downto 0);
            axi_rresp_o           : out std_logic_vector(1 downto 0);
            axi_rvalid_o          : out std_logic;
            axi_rready_i          : in  std_logic;
            -- Int
            irq_i  : in  std_logic_vector(31 downto 0);
            int_o  : out  std_logic);
    end component;


    signal sig_intr    : std_logic;


    signal sig_m0_awvalid : std_logic;
    signal sig_m0_awready : std_logic;
    signal sig_m0_awaddr  : std_logic_vector(31 downto 0);
    signal sig_m0_awprot  : std_logic_vector(2 downto 0);

    signal sig_m0_wvalid  : std_logic;
    signal sig_m0_wready  : std_logic;
    signal sig_m0_wdata   : std_logic_vector(31 downto 0);
    signal sig_m0_wstrb   : std_logic_vector(3 downto 0);

    signal sig_m0_bvalid  : std_logic;
    signal sig_m0_bready  : std_logic;
    signal sig_m0_bresp   : std_logic_vector(1 downto 0);

    signal sig_m0_arvalid : std_logic;
    signal sig_m0_arready : std_logic;
    signal sig_m0_araddr  : std_logic_vector(31 downto 0);
    signal sig_m0_arprot  : std_logic_vector(2 downto 0);

    signal sig_m0_rvalid  : std_logic;
    signal sig_m0_rready  : std_logic;
    signal sig_m0_rdata   : std_logic_vector(31 downto 0);
    signal sig_m0_rresp   : std_logic_vector(1 downto 0);


    signal sig_awvalid  : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_awready  : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_awaddr   : std_logic_vector((N_SLAVES*32)-1 downto 0);
    signal sig_awprot   : std_logic_vector((N_SLAVES*3)-1 downto 0);

    signal sig_wvalid   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_wready   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_wdata    : std_logic_vector((N_SLAVES*32)-1 downto 0);
    signal sig_wstrb    : std_logic_vector((N_SLAVES*4)-1 downto 0);

    signal sig_bvalid   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_bready   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_bresp    : std_logic_vector((N_SLAVES*2)-1 downto 0);

    signal sig_arvalid  : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_arready  : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_araddr   : std_logic_vector((N_SLAVES*32)-1 downto 0);
    signal sig_arprot   : std_logic_vector((N_SLAVES*3)-1 downto 0);

    signal sig_rvalid   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_rready   : std_logic_vector(N_SLAVES-1 downto 0);
    signal sig_rdata    : std_logic_vector((N_SLAVES*32)-1 downto 0);
    signal sig_rresp    : std_logic_vector((N_SLAVES*2)-1 downto 0);


    type array_of_stdlogic is array(0 to N_SLAVES-1) of std_logic;
    type array_of_stdvec2 is array(0 to N_SLAVES-1) of std_logic_vector(1 downto 0);
    type array_of_stdvec3 is array(0 to N_SLAVES-1) of std_logic_vector(2 downto 0);
    type array_of_stdvec4 is array(0 to N_SLAVES-1) of std_logic_vector(3 downto 0);
    type array_of_stdvec32 is array(0 to N_SLAVES-1) of std_logic_vector(31 downto 0);

    signal sig_slaves_awvalid : array_of_stdlogic;
    signal sig_slaves_awready : array_of_stdlogic;
    signal sig_slaves_awaddr  : array_of_stdvec32;
    signal sig_slaves_awprot  : array_of_stdvec3;

    signal sig_slaves_wvalid  : array_of_stdlogic;
    signal sig_slaves_wready  : array_of_stdlogic;
    signal sig_slaves_wdata   : array_of_stdvec32;
    signal sig_slaves_wstrb   : array_of_stdvec4;

    signal sig_slaves_bready  : array_of_stdlogic;
    signal sig_slaves_bvalid  : array_of_stdlogic;
    signal sig_slaves_bresp   : array_of_stdvec2;

    signal sig_slaves_arvalid : array_of_stdlogic;
    signal sig_slaves_arready : array_of_stdlogic;
    signal sig_slaves_araddr  : array_of_stdvec32;
    signal sig_slaves_arprot  : array_of_stdvec3;

    signal sig_slaves_rready  : array_of_stdlogic;
    signal sig_slaves_rdata   : array_of_stdvec32;
    signal sig_slaves_rresp   : array_of_stdvec2;
    signal sig_slaves_rvalid  : array_of_stdlogic;
    
    
    signal sig_irqs         : std_logic_vector(31 downto 0);
    signal sig_int_timer    : std_logic;
    signal sig_uart_rx_int  : std_logic;
    signal sig_uart_tx_int  : std_logic;

begin

    -- -----------------------------------------------------
    -- AMBA interconnect
    -- -----------------------------------------------------
    
    axi4lite_interconnect: axi4lite_decoder
        generic map(
            sw          => N_SLAVES,
            sw_log2     => N_SLAVES_LOG,
            s0_addr_w   => SLAVE0_ADDR_W, 
            s0_addr     => SLAVE0_ADDR,
            s1_addr_w   => SLAVE1_ADDR_W, 
            s1_addr     => SLAVE1_ADDR,
            s2_addr_w   => SLAVE2_ADDR_W, 
            s2_addr     => SLAVE2_ADDR,
            s3_addr_w   => SLAVE3_ADDR_W, 
            s3_addr     => SLAVE3_ADDR,
            s4_addr_w   => SLAVE4_ADDR_W, 
            s4_addr     => SLAVE4_ADDR,
            s5_addr_w   => SLAVE5_ADDR_W, 
            s5_addr     => SLAVE5_ADDR)
        port map(
            -- master write address channel
            m_awvalid_i => sig_m0_awvalid,
            m_awready_o => sig_m0_awready,
            m_awaddr_i  => sig_m0_awaddr,
            m_awprot_i  => sig_m0_awprot,
            -- master write data channel
            m_wvalid_i  => sig_m0_wvalid,
            m_wready_o  => sig_m0_wready,
            m_wdata_i   => sig_m0_wdata,
            m_wstrb_i   => sig_m0_wstrb,
            -- master write response channel
            m_bvalid_o  => sig_m0_bvalid,
            m_bready_i  => sig_m0_bready,
            m_bresp_o   => sig_m0_bresp,
            -- master read address channel
            m_arvalid_i => sig_m0_arvalid,
            m_arready_o => sig_m0_arready,
            m_araddr_i  => sig_m0_araddr,
            m_arprot_i  => sig_m0_arprot,
            -- master read data channel
            m_rvalid_o  => sig_m0_rvalid,
            m_rready_i  => sig_m0_rready,
            m_rdata_o   => sig_m0_rdata,
            m_rresp_o   => sig_m0_rresp,
            -- slaves write address channel
            s_awvalid_o =>  sig_awvalid,
            s_awready_i =>  sig_awready,
            s_awaddr_o  =>  sig_awaddr,
            s_awprot_o  =>  sig_awprot,
            -- slaves write data channel
            s_wvalid_o  =>  sig_wvalid,
            s_wready_i  =>  sig_wready,
            s_wdata_o   =>  sig_wdata,
            s_wstrb_o   =>  sig_wstrb,
            -- slaves write response channel
            s_bvalid_i  =>  sig_bvalid,
            s_bready_o  =>  sig_bready,
            s_bresp_i   =>  sig_bresp,
            -- slaves read address channel
            s_arvalid_o =>  sig_arvalid,
            s_arready_i =>  sig_arready,
            s_araddr_o  =>  sig_araddr,
            s_arprot_o  =>  sig_arprot,
            -- slaves read data channel
            s_rvalid_i  =>  sig_rvalid,
            s_rready_o  =>  sig_rready,
            s_rdata_i   =>  sig_rdata,
            s_rresp_i   =>  sig_rresp);


    slaves_outputs_to_interconnect: for s in 0 to (N_SLAVES-1) generate
        sig_awready(s) <= sig_slaves_awready(s);
        sig_wready(s)  <= sig_slaves_wready(s);
        sig_bvalid(s)  <= sig_slaves_bvalid(s);
        sig_arready(s) <= sig_slaves_arready(s);
        sig_rvalid(s)  <= sig_slaves_rvalid(s);
        sig_bresp( ((s*2)+1) downto (s*2) )    <= sig_slaves_bresp(s);
        sig_rdata( ((s*32)+31) downto (s*32) ) <= sig_slaves_rdata(s);
        sig_rresp( ((s*2)+1) downto (s*2) )    <= sig_slaves_rresp(s);
    end generate slaves_outputs_to_interconnect;

    slaves_inputs_from_interconnect: for s in 0 to (N_SLAVES-1) generate
        sig_slaves_awvalid(s) <= sig_awvalid(s);
        sig_slaves_awaddr(s)  <= sig_awaddr( ((s*32)+31) downto (s*32) );
        sig_slaves_awprot(s)  <= sig_awprot( ((s*3)+2) downto (s*3) );
        sig_slaves_wvalid(s)  <= sig_wvalid(s);
        sig_slaves_wdata(s)   <= sig_wdata( ((s*32)+31) downto (s*32) );
        sig_slaves_wstrb(s)   <= sig_wstrb( ((s*4)+3) downto (s*4) );
        sig_slaves_bready(s)  <= sig_bready(s);
        sig_slaves_arvalid(s) <= sig_arvalid(s);
        sig_slaves_araddr(s)  <= sig_araddr( ((s*32)+31) downto (s*32) );
        sig_slaves_arprot(s)  <= sig_arprot( ((s*3)+2) downto (s*3) );
        sig_slaves_rready(s)  <= sig_rready(s);
    end generate slaves_inputs_from_interconnect;

    -- -----------------------------------------------------
    -- Master - Plasma
    -- -----------------------------------------------------

    plasma_amba: plasma_axi4lite_master
        generic map(
            memory_type     => "XILINX_16X",
            mult_type       => "DEFAULT",
            shifter_type    => "DEFAULT",
            alu_type        => "DEFAULT",
            pipeline_stages => 2)
        port map(
            aclk       => clk_i,
            areset     => reset_i,
            -- write address channel
            awvalid    => sig_m0_awvalid,
            awready    => sig_m0_awready,
            awaddr     => sig_m0_awaddr,
            awprot     => sig_m0_awprot,
            -- write data channel
            wvalid     => sig_m0_wvalid,
            wready     => sig_m0_wready,
            wdata      => sig_m0_wdata,
            wstrb      => sig_m0_wstrb,
            -- write response channel
            bvalid     => sig_m0_bvalid,
            bready     => sig_m0_bready,
            bresp      => sig_m0_bresp,
            -- read address channel
            arvalid    => sig_m0_arvalid,
            arready    => sig_m0_arready,
            araddr     => sig_m0_araddr,
            arprot     => sig_m0_arprot,
            -- read data channel
            rvalid     => sig_m0_rvalid,
            rready     => sig_m0_rready,
            rdata      => sig_m0_rdata,
            rresp      => sig_m0_rresp,

            intr       => sig_intr);
            
    -- -----------------------------------------------------
    -- Slave0 - Internal RAM
    -- -----------------------------------------------------

    ram_amba0: ram_amba_128k
        port map(
            s_aclk        => clk_i,
            s_aresetn     => reset_i,

            s_axi_awvalid => sig_slaves_awvalid(0),
            s_axi_awready => sig_slaves_awready(0),
            s_axi_awaddr  => sig_slaves_awaddr(0),

            s_axi_wvalid  => sig_slaves_wvalid(0),
            s_axi_wready  => sig_slaves_wready(0),
            s_axi_wdata   => sig_slaves_wdata(0),
            s_axi_wstrb   => sig_slaves_wstrb(0), 

            s_axi_bvalid  => sig_slaves_bvalid(0),
            s_axi_bready  => sig_slaves_bready(0),
            s_axi_bresp   => sig_slaves_bresp(0),

            s_axi_arvalid => sig_slaves_arvalid(0),
            s_axi_arready => sig_slaves_arready(0),
            s_axi_araddr  => sig_slaves_araddr(0),

            s_axi_rvalid  => sig_slaves_rvalid(0),
            s_axi_rready  => sig_slaves_rready(0),
            s_axi_rdata   => sig_slaves_rdata(0),
            s_axi_rresp   => sig_slaves_rresp(0));

    -- -----------------------------------------------------
    -- Slave1 - External RAM (DDR3 controller)
    -- -----------------------------------------------------

    ram_amba1: ram_amba_1024k
        port map(
            s_aclk        => clk_i,
            s_aresetn     => reset_i,

            s_axi_awvalid => sig_slaves_awvalid(1),
            s_axi_awready => sig_slaves_awready(1),
            s_axi_awaddr  => sig_slaves_awaddr(1),

            s_axi_wvalid  => sig_slaves_wvalid(1),
            s_axi_wready  => sig_slaves_wready(1),
            s_axi_wdata   => sig_slaves_wdata(1),
            s_axi_wstrb   => sig_slaves_wstrb(1), 

            s_axi_bvalid  => sig_slaves_bvalid(1),
            s_axi_bready  => sig_slaves_bready(1),
            s_axi_bresp   => sig_slaves_bresp(1),

            s_axi_arvalid => sig_slaves_arvalid(1),
            s_axi_arready => sig_slaves_arready(1),
            s_axi_araddr  => sig_slaves_araddr(1),

            s_axi_rvalid  => sig_slaves_rvalid(1),
            s_axi_rready  => sig_slaves_rready(1),
            s_axi_rdata   => sig_slaves_rdata(1),
            s_axi_rresp   => sig_slaves_rresp(1));

    -- -----------------------------------------------------
    -- Slave2 - UART
    -- -----------------------------------------------------

    uart: simple_uart_axi4lite 
        generic map(
            TXDEPTH => 3,
            RXDEPTH => 3)
        port map(
            -- System signals
            clk_i       => clk_i,
            axi_rst_i   => reset_i,
            rx_int_o    => sig_uart_rx_int,
            tx_int_o    => sig_uart_tx_int,
            -- AXI signals
            axi_awaddr_i    => sig_slaves_awaddr(2),
            axi_awvalid_i   => sig_slaves_awvalid(2),
            axi_awready_o   => sig_slaves_awready(2),
            axi_wdata_i     => sig_slaves_wdata(2),
            axi_wstrb_i     => sig_slaves_wstrb(2),
            axi_wvalid_i    => sig_slaves_wvalid(2),
            axi_wready_o    => sig_slaves_wready(2),
            axi_bresp_o     => sig_slaves_bresp(2),
            axi_bvalid_o    => sig_slaves_bvalid(2),
            axi_bready_i    => sig_slaves_bready(2),
            axi_araddr_i    => sig_slaves_araddr(2),
            axi_arvalid_i   => sig_slaves_arvalid(2),
            axi_arready_o   => sig_slaves_arready(2),
            axi_rdata_o     => sig_slaves_rdata(2),
            axi_rresp_o     => sig_slaves_rresp(2),
            axi_rvalid_o    => sig_slaves_rvalid(2),
            axi_rready_i    => sig_slaves_rready(2),
            -- UARTLite Interface Signals
            rx_i    => uart_rx_i,
            tx_o    => uart_tx_o,
            baud_o  => uart_baud_o);


    -- -----------------------------------------------------
    -- Slave3 - GPIO
    -- -----------------------------------------------------
            
    gpio: gpio_axi4lite 
        port map(
            -- System signals
            clk_i       => clk_i,
            axi_rst_i   => reset_i,
            -- AXI signals
            axi_awaddr_i    => sig_slaves_awaddr(3),
            axi_awvalid_i   => sig_slaves_awvalid(3),
            axi_awready_o   => sig_slaves_awready(3),
            axi_wdata_i     => sig_slaves_wdata(3),
            axi_wstrb_i     => sig_slaves_wstrb(3),
            axi_wvalid_i    => sig_slaves_wvalid(3),
            axi_wready_o    => sig_slaves_wready(3),
            axi_bresp_o     => sig_slaves_bresp(3),
            axi_bvalid_o    => sig_slaves_bvalid(3),
            axi_bready_i    => sig_slaves_bready(3),
            axi_araddr_i    => sig_slaves_araddr(3),
            axi_arvalid_i   => sig_slaves_arvalid(3),
            axi_arready_o   => sig_slaves_arready(3),
            axi_rdata_o     => sig_slaves_rdata(3),
            axi_rresp_o     => sig_slaves_rresp(3),
            axi_rvalid_o    => sig_slaves_rvalid(3),
            axi_rready_i    => sig_slaves_rready(3),
            -- GPIO
            gpio_o  => gpio_o,
            gpio_i  => gpio_i);

    -- -----------------------------------------------------
    -- Slave4 - Timer
    -- -----------------------------------------------------
            
    timer: timer_axi4lite 
        port map(
            -- System signals
            clk_i       => clk_i,
            axi_rst_i   => reset_i,
            -- AXI signals
            axi_awaddr_i    => sig_slaves_awaddr(4),
            axi_awvalid_i   => sig_slaves_awvalid(4),
            axi_awready_o   => sig_slaves_awready(4),
            axi_wdata_i     => sig_slaves_wdata(4),
            axi_wstrb_i     => sig_slaves_wstrb(4),
            axi_wvalid_i    => sig_slaves_wvalid(4),
            axi_wready_o    => sig_slaves_wready(4),
            axi_bresp_o     => sig_slaves_bresp(4),
            axi_bvalid_o    => sig_slaves_bvalid(4),
            axi_bready_i    => sig_slaves_bready(4),
            axi_araddr_i    => sig_slaves_araddr(4),
            axi_arvalid_i   => sig_slaves_arvalid(4),
            axi_arready_o   => sig_slaves_arready(4),
            axi_rdata_o     => sig_slaves_rdata(4),
            axi_rresp_o     => sig_slaves_rresp(4),
            axi_rvalid_o    => sig_slaves_rvalid(4),
            axi_rready_i    => sig_slaves_rready(4),
            -- Int
            int_o  => sig_int_timer); 
            
    -- -----------------------------------------------------
    -- Slave5 - PIC
    -- -----------------------------------------------------
        
    sig_irqs(0) <= sig_int_timer;
    sig_irqs(1) <= sig_uart_rx_int;
    sig_irqs(2) <= sig_uart_tx_int;
    sig_irqs(31 downto 3) <= (others => '0');        
            
    pic: pic_axi4lite 
        port map(
            -- System signals
            clk_i       => clk_i,
            axi_rst_i   => reset_i,
            -- AXI signals
            axi_awaddr_i    => sig_slaves_awaddr(5),
            axi_awvalid_i   => sig_slaves_awvalid(5),
            axi_awready_o   => sig_slaves_awready(5),
            axi_wdata_i     => sig_slaves_wdata(5),
            axi_wstrb_i     => sig_slaves_wstrb(5),
            axi_wvalid_i    => sig_slaves_wvalid(5),
            axi_wready_o    => sig_slaves_wready(5),
            axi_bresp_o     => sig_slaves_bresp(5),
            axi_bvalid_o    => sig_slaves_bvalid(5),
            axi_bready_i    => sig_slaves_bready(5),
            axi_araddr_i    => sig_slaves_araddr(5),
            axi_arvalid_i   => sig_slaves_arvalid(5),
            axi_arready_o   => sig_slaves_arready(5),
            axi_rdata_o     => sig_slaves_rdata(5),
            axi_rresp_o     => sig_slaves_rresp(5),
            axi_rvalid_o    => sig_slaves_rvalid(5),
            axi_rready_i    => sig_slaves_rready(5),
            -- Int
            irq_i  => sig_irqs,
            int_o  => sig_intr);

end RTL;
