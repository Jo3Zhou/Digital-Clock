library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.DigEng.ALL;

--this is entity provide stopwatch
--the clock input should be 100MHz
--the user can set en to '1' to start stopwatch
--or set en to '0' to pause stopwatch
--and use clr to clear stopwatch

entity stopwatch is
    Generic (LIMIT : integer := 1000000);
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
		   --user input control
           en : in STD_LOGIC;
           clr : in STD_LOGIC;
		   --output to show the stopwatch time
           msc_out : out STD_LOGIC_VECTOR(31 downto 0);
           sec_out : out STD_LOGIC_VECTOR(31 downto 0);
           min_out : out STD_LOGIC_VECTOR(31 downto 0));
           
end stopwatch;

architecture Behavioral of stopwatch is

--internal signals
--ten nano second, the first counter's output
signal ten_nano_second : unsigned (log2(LIMIT)-1 downto 0);
--msc_int, the second counter's output
--sec_int, the first counter's output
--min_int, the third counter's output
signal msc_int : unsigned (31 downto 0);
signal sec_int : unsigned (31 downto 0);
signal min_int : unsigned (31 downto 0);
--multiplexer input for generate enable second for counters
signal one_msc, one_sec, one_min : std_logic;

begin

--ten_ns for generate 10 nanosecond
ten_ns: process (clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            ten_nano_second <= (others => '0');
        elsif clr = '1' then
            ten_nano_second <= (others => '0');
        elsif en = '1' then
            if ten_nano_second = LIMIT -1 then
                ten_nano_second <= (others => '0');
            else
                ten_nano_second <= ten_nano_second + 1;
            end if;
        end if;
    end if;
end process;

--ms for generate 1 millisecond
ms: process (clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            msc_int <= (others => '0');
        elsif clr = '1' then
            msc_int <= (others => '0');
        elsif one_msc = '1' then
            if msc_int = 99 then
                msc_int <= (others => '0');
            else
                msc_int <= msc_int + 1;
            end if;
        end if;
    end if;
end process;

--second for generate 1 second
second: process (clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            sec_int <= (others => '0');
        elsif clr = '1' then
            sec_int <= (others => '0');
        elsif one_sec = '1' then
            if sec_int = 59 then
                sec_int <= (others => '0');
            else
                sec_int <= sec_int + 1;
            end if;
        end if;
    end if;
end process;

one_msc <= '1' when ten_nano_second = LIMIT - 1 else '0';
one_sec <= '1' when msc_int = 99 and ten_nano_second = LIMIT - 1 else '0';
one_min <= '1' when sec_int = 59 and msc_int = 99 and ten_nano_second = LIMIT - 1 else '0';

---output
msc_out <= std_logic_vector(msc_int);
sec_out <= std_logic_vector(sec_int);
min_out <= std_logic_vector(min_int);

end Behavioral;
