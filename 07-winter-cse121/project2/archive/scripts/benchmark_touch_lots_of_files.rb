arguments 100, 200, 500, 1000, 2000, 5000, 10000

x_label 'number of files'

format_size 1000000

run do |b, arg|
  1.upto(arg) do |i|
    b.touch(i)
  end
end
