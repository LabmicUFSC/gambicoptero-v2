library ieee;
use ieee.std_logic_1164.all; 


entity plasma_axi4lite_master is
    generic(
        memory_type     : string  := "XILINX_16X";
        mult_type       : string  := "DEFAULT"; --AREA_OPTIMIZED
        shifter_type    : string  := "DEFAULT"; --AREA_OPTIMIZED
        alu_type        : string  := "DEFAULT"; --AREA_OPTIMIZED
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
end plasma_axi4lite_master;


architecture RTL of plasma_axi4lite_master is
    component mlite_cpu is
        generic(
            memory_type     : string  := "XILINX_16X"; --ALTERA_LPM, or DUAL_PORT_
            mult_type       : string  := "DEFAULT"; --AREA_OPTIMIZED
            shifter_type    : string  := "DEFAULT"; --AREA_OPTIMIZED
            alu_type        : string  := "DEFAULT"; --AREA_OPTIMIZED
            pipeline_stages : natural := 2); --2 or 3
        port(
            clk          : in std_logic;
            reset_in     : in std_logic;
            intr_in      : in std_logic;

            address_next : out std_logic_vector(31 downto 2);
            byte_we_next : out std_logic_vector(3 downto 0); 

            address      : out std_logic_vector(31 downto 2);
            byte_we      : out std_logic_vector(3 downto 0);
            data_w       : out std_logic_vector(31 downto 0);
            data_r       : in std_logic_vector(31 downto 0);
            mem_pause    : in std_logic);
    end component;

    signal plasma_reset         : std_logic;

    signal plasma_byte_we_next  : std_logic_vector(3 downto 0);
    signal plasma_address_next  : std_logic_vector(31 downto 2);

    signal plasma_address       : std_logic_vector(31 downto 2);
    signal plasma_byte_we       : std_logic_vector(3 downto 0);
    signal plasma_data_write    : std_logic_vector(31 downto 0);
    signal plasma_data_read     : std_logic_vector(31 downto 0);
    signal plasma_mem_pause_in  : std_logic;

    type STATE_TYPE is (RST1, RST2, IDLE, ADDR_READ, DATA_READ, 
                        ADDR_WRITE, DATA_WRITE, WRITE_RESPONSE);
    signal state : STATE_TYPE;

    constant ZERO_32BITS : std_logic_vector(31 downto 0) := "00000000000000000000000000000000";

begin

    plasma_cpu: mlite_cpu
        generic map(
            memory_type     => memory_type,
            mult_type       => mult_type,
            shifter_type    => shifter_type,
            alu_type        => alu_type,
            pipeline_stages => pipeline_stages)
        port map(
            clk          => aclk,
            reset_in     => plasma_reset,
            intr_in      => intr,

            address_next => plasma_address_next,
            byte_we_next => plasma_byte_we_next, 

            address      => plasma_address,
            byte_we      => plasma_byte_we,
            data_w       => plasma_data_write,
            data_r       => plasma_data_read,
            mem_pause    => plasma_mem_pause_in);


    -- leave write and read protections ALL LOW
    awprot <= "000";
    arprot <= "000";

    -- plasma reset is active HIGH while AXI is active LOW, damn!
    plasma_reset <= not areset;

    -- permanently tied signals
    araddr           <= plasma_address & "00";
    awaddr           <= plasma_address & "00";
    plasma_data_read <= rdata;
    wdata            <= plasma_data_write;
    wstrb            <= plasma_byte_we;
    rready           <= '1';
    bready           <= '1';


    -- Moore machine, the inputs are Plasma's external memory signals
    -- and the outputs are all AXI4Lite channels
    fsm_proc: process(aclk, areset)
    begin
        if areset = '0' then
            state <= RST1;

            -- must drive these LOW on reset (AXI Protocol Specification sec. 11.1.2)
            arvalid <= '0';
            awvalid <= '0';
            wvalid  <= '0';

        elsif rising_edge(aclk) then
            case state is
                when RST1 =>
                    state <= RST2;
                    arvalid <= '0'; awvalid <= '0'; wvalid  <= '0';
                    plasma_mem_pause_in <= '0';

                when RST2 =>
                    state <= ADDR_READ;
                    arvalid <= '0'; awvalid <= '0'; wvalid  <= '0';
                    plasma_mem_pause_in <= '0';

                when IDLE =>
                    if plasma_address_next /= plasma_address then
                        if plasma_byte_we_next = "0000" then
                            state   <= ADDR_READ;
                            arvalid <= '1';
                            awvalid <= '0';
                        else
                            state   <= ADDR_WRITE;
                            arvalid <= '0';
                            awvalid <= '1';
                        end if;
                        plasma_mem_pause_in <= '1';
                    else
                        state <= IDLE;
                        plasma_mem_pause_in <= '0';
                        arvalid             <= '0';
                        awvalid             <= '0';
                    end if;
                    wvalid <= '0';

                when ADDR_READ =>
                    if arready = '1' then
                        state <= DATA_READ;
                    else
                        state <= ADDR_READ;
                    end if;
                    plasma_mem_pause_in <= '1';
                    arvalid             <= '1';
                    awvalid             <= '0';
                    wvalid              <= '0';
                
                when DATA_READ =>
                    if rvalid = '1' then
                        state <= IDLE;
                        plasma_mem_pause_in <= '0';
                    else
                        state <= DATA_READ;
                        plasma_mem_pause_in <= '1';
                    end if;
                    
                    arvalid             <= '0';
                    awvalid             <= '0';
                    wvalid              <= '0';
                
                when ADDR_WRITE =>
                    if awready = '1' then
                        state <= DATA_WRITE;
                    else
                        state <= ADDR_WRITE;
                    end if;
                    plasma_mem_pause_in <= '1';
                    arvalid             <= '0';
                    awvalid             <= '1';
                    wvalid              <= '0';

                when DATA_WRITE =>
                    state <= WRITE_RESPONSE;

                    plasma_mem_pause_in <= '1';
                    arvalid             <= '0';
                    awvalid             <= '0';
                    wvalid              <= '1';

                when WRITE_RESPONSE =>
                    if bvalid = '1' then
                        state <= IDLE;
                    else
                        state <= WRITE_RESPONSE;
                    end if;
                    plasma_mem_pause_in <= '1';
                    arvalid             <= '0';
                    awvalid             <= '0';
                    wvalid              <= '0';
            end case;

        end if;
    end process;

end RTL;

