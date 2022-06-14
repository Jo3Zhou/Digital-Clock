library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.DigEng.ALL;
use IEEE.NUMERIC_STD.ALL;

entity debouncer is
    Generic (LIMIT : NATURAL := 50000000);
    Port (
    clk : in std_logic;
    sig : in std_logic;
    deb_sig : out std_logic
    );
end debouncer;

architecture Behavioral of debouncer is
    signal rst : STD_LOGIC;
    signal en : STD_LOGIC;
    signal cnt_int : UNSIGNED (log2(LIMIT)-1 downto 0);
    signal achieve : STD_LOGIC;
    signal deb_sig_int : STD_LOGIC;
    signal dff_out : std_logic;
begin

counter : process (clk)
begin
    if rising_edge (clk) then
        if (rst = '1') then
            cnt_int <= (others => '0');
        elsif (en = '1') then
            if (cnt_int = LIMIT - 1) then
                cnt_int <= (others => '0');
            else
                cnt_int <= cnt_int + 1;
            end if;
        end if;
    end if;
end process counter;

dff: process (clk)
begin
    if rising_edge (clk) then
        dff_out <= achieve;
    end if;
end process;

rst <= not Sig;
achieve <= '1' when (cnt_int = LIMIT - 1) else '0';
Deb_sig <= (not(dff_out) and achieve);
en <= (Sig and not(achieve));

end Behavioral;
