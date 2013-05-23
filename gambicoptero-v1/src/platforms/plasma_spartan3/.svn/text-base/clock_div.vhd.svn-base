library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clock_div is
    Port(
        clk_in   : in std_logic;
        clk_div2 : out std_logic;
        clk_div4 : out std_logic);
end clock_div;

architecture Behavioral of clock_div is
    signal clk_div_reg : std_logic_vector(31 downto 0);
begin

    clock_div_incr: process(clk_in)
    begin
        if rising_edge(clk_in) then
            clk_div_reg <= std_logic_vector(unsigned(clk_div_reg) + 1);
        end if;
    end process;

    clk_div2 <= clk_div_reg(0);
    clk_div4 <= clk_div_reg(1);

end Behavioral; 
