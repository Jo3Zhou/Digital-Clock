library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

--alarm is entity for user input min and hour
--when the timer entity input is equal to user input and en_alarm is '1'
--alarm will generate a interrupt to PS

entity alarm is
    Port ( en_alarm : in STD_LOGIC;
           min_in : in STD_LOGIC_VECTOR (31 downto 0);
           hor_in : in STD_LOGIC_VECTOR (31 downto 0);
           min_set : in STD_LOGIC_VECTOR (31 downto 0);
           hor_set : in STD_LOGIC_VECTOR (31 downto 0);
           min_out : out STD_LOGIC_VECTOR (31 downto 0);
           hor_out : out STD_LOGIC_VECTOR (31 downto 0);
           interrupt : out std_logic);
end alarm;

architecture Behavioral of alarm is

signal min_int : unsigned (31 downto 0);
signal hor_int : unsigned (31 downto 0);

begin

min_out <= min_set;
hor_out <= hor_set;

interrupt <= '1' when en_alarm = '1' and min_in = min_set and hor_in = hor_set else '0';

end Behavioral;
