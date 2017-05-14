module fibonacci_calculator (input_s, reset_n, begin_fibo,
                             clk, done, fibo_out);
  input  [4:0] input_s;
  input  reset_n;
  input  begin_fibo;
  input  clk;
  output done;
  output [15:0] fibo_out;

  reg done;
  reg [15:0] fibo_out;

  reg [15:0] a;
  reg [15:0] b;
  reg [4:0] count;
  reg started;

  always@ (posedge clk) begin
    if (reset_n == 0) begin
      a <= 1;
      b <= 1;
      count <= 0;
      started <= 0;
      done <= 0;
    end
    else if (begin_fibo == 1 && started == 0) begin
      count <= (input_s == 0) ? 0 : input_s - 1;
      a <= 1;
      b <= 1;
      started <= 1;
      done <= 0;
    end
    else if (count > 0) begin
      a <= b;
      b <= a + b;
      count <= count - 1;
      done <= 0;
    end
    else begin
      if (done == 0)
        fibo_out <= b;
      done <= 1;
      started <= 0;
    end
  end
endmodule
