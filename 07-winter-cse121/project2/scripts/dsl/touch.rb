SCALE = 10

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'touch a bunch of files'

x_label 'number of files'

run do |argument|
  1.upto(argument) do |i|
    touch i
  end
end
