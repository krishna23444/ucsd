SCALE = 1

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'idle'

x_label 'idle time (seconds)'

run do |argument|
  sleep argument
end
