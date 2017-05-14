library verilog;
use verilog.vl_types.all;
entity fibonacci_calculator is
    port(
        input_s         : in     vl_logic_vector(4 downto 0);
        reset_n         : in     vl_logic;
        begin_fibo      : in     vl_logic;
        clk             : in     vl_logic;
        done            : out    vl_logic;
        fibo_out        : out    vl_logic_vector(15 downto 0)
    );
end fibonacci_calculator;
