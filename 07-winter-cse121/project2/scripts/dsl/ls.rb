SCALE = 10

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'ls'

x_label 'number of items (#)'

run do |argument|
  1.upto(argument) do |i|
    if i % 2 == 0
      touch i
    else
      mkdir i
    end
  end

  unmount
  mount

  ls
end
