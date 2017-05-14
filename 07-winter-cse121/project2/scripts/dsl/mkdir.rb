SCALE = 300

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'mkdir'

x_label 'number of directories (#)'

run do |argument|
  1.upto(argument) do |i|
    mkdir i
  end
end
