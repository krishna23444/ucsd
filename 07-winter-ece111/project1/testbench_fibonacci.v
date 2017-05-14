module testbench_fibonacci;
  reg [4:0] input_s;
  reg reset_n;
  reg begin_fibo;
  reg clk;
  wire done;
  wire [15:0] fibo_out;
  
  fibonacci_calculator uut(.input_s(input_s),
                           .reset_n(reset_n),
                           .begin_fibo(begin_fibo),
                           .clk(clk),
                           .done(done),
                           .fibo_out(fibo_out));
                         
  initial begin
    clk = 0;
    forever #1
    clk = ~clk;
  end

  initial begin
    begin_fibo = 1;

    reset_n = 0;
    input_s = 0;
    #2
    reset_n = 1;
    #5

    reset_n = 0;
    input_s = 1;
    #2
    reset_n = 1;
    #5

    reset_n = 0;
    input_s = 3;
    #2
    reset_n = 1;
    #20

    reset_n = 0;
    input_s = 7;
    #2
    reset_n = 1;
    #20

    reset_n = 0;
    input_s = 15;
    #2
    reset_n = 1;
    #35

    reset_n = 0;
    input_s = 20;
    #2
    reset_n = 1;
    #50

    reset_n = 0;
    input_s = 23;
    #2
    reset_n = 1;
    #70

    $stop;
  end
endmodule
