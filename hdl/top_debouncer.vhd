library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.DigEng.ALL;
use IEEE.NUMERIC_STD.ALL;

entity top_debouncer is
    Port ( clk : in STD_LOGIC;
           sig : in STD_LOGIC_VECTOR (4 downto 0);
           deb_sig : out STD_LOGIC_VECTOR (9 downto 0));
end top_debouncer;

architecture Behavioral of top_debouncer is
signal deb_sig_half : std_logic_vector (4 downto 0);
signal deb_sig_thre : std_logic_vector (4 downto 0);

begin

debouncers_half: for i in 0 to 4 generate
	deb: entity work.debouncer 
	generic map (LIMIT => 10000000)
	PORT MAP(
		clk => clk,
		sig => sig(i),
		deb_sig => deb_sig_half(i)
	);
end generate;

debouncers_thre: for i in 0 to 4 generate
	deb: entity work.debouncer 
	generic map (LIMIT => 200000000)
	PORT MAP(
		clk => clk,
		sig => sig(i),
		deb_sig => deb_sig_thre(i)
	);
end generate;

deb_sig (4 downto 0) <= deb_sig_half;
deb_sig (9 downto 5) <= deb_sig_thre;

end Behavioral;
