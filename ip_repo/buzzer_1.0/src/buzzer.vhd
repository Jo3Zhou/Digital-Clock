library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

--buzzer entity contains a counter
--when en signal is '1' the buzz output signal will generate 2048hz signal

entity buzzer is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           en : in STD_LOGIC;
           buzz : out STD_LOGIC);
end buzzer;

architecture Behavioral of buzzer is
signal counter_value : unsigned(17 downto 0);
begin

counter: process(clk)
begin
if rising_edge(clk) then
    if rst ='0' then
        counter_value <= (others => '0');
    else
        if counter_value = 48827 then
            counter_value <= (others => '0');
        else
            counter_value <= counter_value + 1;
        end if;
    end if;
end if;
end process;

buzz <= '0' when counter_value <= 24413 and en = '1' else
        '1' when counter_value > 24413 and en = '1' else
        '1';

end Behavioral;
