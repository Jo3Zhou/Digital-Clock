library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.DigEng.ALL;

entity timer is
    Generic (LIMIT : integer := 100000000);
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           change : in STD_LOGIC;
           sec_in : in STD_LOGIC_VECTOR (31 downto 0);
           min_in : in STD_LOGIC_VECTOR (31 downto 0);
           hor_in : in STD_LOGIC_VECTOR (31 downto 0);
           sec_out : out STD_LOGIC_VECTOR (31 downto 0);
           min_out : out STD_LOGIC_VECTOR (31 downto 0);
           hor_out : out STD_LOGIC_VECTOR (31 downto 0));
end timer;

architecture Behavioral of timer is

---internal signals
signal ten_nano_second : unsigned (log2(LIMIT)-1 downto 0);

signal sec_int : unsigned (31 downto 0);
signal min_int : unsigned (31 downto 0);
signal hor_int : unsigned (31 downto 0);

signal one_sec, one_min, one_hor : std_logic;

begin

ten_ns: process (clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            ten_nano_second <= (others => '0');
        else
            if ten_nano_second = LIMIT -1 then
                ten_nano_second <= (others => '0');
            else
                ten_nano_second <= ten_nano_second + 1;
            end if;
        end if;
    end if;
end process;

second : process(clk) is
begin
    if rising_edge (clk) then
        if rst = '0' then
            sec_int <= (others => '0');
        elsif change = '1' then
            sec_int <= unsigned(sec_in);
        elsif change = '0' then
            if one_sec = '1' then
                if sec_int = 59 then
                    sec_int <= (others => '0');
                else
                    sec_int <= sec_int + 1;
                end if;
            end if;
        end if;
    end if;
end process;

minute : process(clk) is
begin
    if rising_edge(clk) then
        if rst = '0' then
            min_int <= (others => '0');
        elsif change = '1' then
            min_int <= unsigned(min_in);
        elsif change = '0' then
            if one_min = '1' then
                if min_int = 59 then
                    min_int <= (others => '0');
                else
                    min_int <= min_int + 1;
                end if;
            end if;
        end if;
    end if;
end process;

hour : process(clk) is
begin
    if rising_edge(clk) then
        if rst = '0' then
            hor_int <= (others => '0');
        elsif change = '1' then
            hor_int <= unsigned(hor_in);
        elsif change = '0' then
            if one_hor = '1' then
                if hor_int = 23 then
                    hor_int <= (others => '0');
                else
                    hor_int <= hor_int + 1;
                end if;
            end if;
        end if;
    end if;
end process;

one_sec <= '1' when ten_nano_second = LIMIT - 1 else '0';
one_min <= '1' when sec_int = 59 and ten_nano_second = LIMIT - 1 else '0';
one_hor <= '1' when min_int = 59 and sec_int = 59 and ten_nano_second = LIMIT - 1 else '0';

----output----
sec_out <= std_logic_vector(sec_int);
min_out <= std_logic_vector(min_int);
hor_out <= std_logic_vector(hor_int);

end Behavioral;