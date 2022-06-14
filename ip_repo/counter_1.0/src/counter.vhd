library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.DigEng.ALL;

--counter entity contains three counters
--first is for generating ten nano second
--second generates 1 second
--third one generates 1 minute
--when output of second and minute counter down to 0
--interrupt will happen

entity counter is
    Generic (LIMIT : integer := 100000000);
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
		   --user input
           str : in STD_LOGIC;
           set : in STD_LOGIC;
           en : in STD_LOGIC;
		   --user input min
           min_in : in STD_LOGIC_VECTOR (31 downto 0);
		   --output 
           sec_out : out STD_LOGIC_VECTOR (31 downto 0);
           min_out : out STD_LOGIC_VECTOR (31 downto 0);
		   --interrput
           interrput : out STD_LOGIC);
end counter;

architecture Behavioral of counter is

signal ten_nano_second : unsigned (log2(LIMIT)-1 downto 0);
signal sec_int : unsigned (31 downto 0);
signal min_int : unsigned (31 downto 0);
signal one_sec, one_min : std_logic;

begin

ten_ns: process (clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            ten_nano_second <= (others => '0');
        elsif str = '1' then
            if ten_nano_second = LIMIT -1 then
                ten_nano_second <= (others => '0');
            else
                ten_nano_second <= ten_nano_second + 1;
            end if;
        end if;
    end if;
end process;

one_sec <= '1' when ten_nano_second = LIMIT - 1 else '0';

second : process(clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            sec_int <= (others => '0');
        elsif set = '1' then
            sec_int <= (others => '0');
        elsif one_sec = '1' then
            if sec_int = 0 then
                if min_int = 0 then
                    sec_int <= (others => '0');
                else
                    sec_int <= to_unsigned(59, 32);
                end if;
            else
                sec_int <= sec_int - 1;
            end if;
        end if;
    end if;
end process;

one_min <= '1' when sec_int = 0 and ten_nano_second = LIMIT - 1 else '0';

minute : process(clk) is
begin
    if rising_edge(clk) then
        if rst = '0' then
            min_int <= (others => '0');
        elsif set = '1' then
            min_int <= unsigned(min_in);
        elsif one_min = '1' then
            if min_int = 0 then
                min_int <= min_int;
            else
                min_int <= min_int - 1;
            end if;
        end if;
    end if;
end process;

sec_out <= std_logic_vector(sec_int);
min_out <= std_logic_vector(min_int);

interrput <= '1' when en ='1' and sec_int = 0 and min_int = 0 else '0';

end Behavioral;
