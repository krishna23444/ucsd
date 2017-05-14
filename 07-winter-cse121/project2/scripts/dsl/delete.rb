SCALE = 10000

format_size_in_blocks SCALE * 10

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'delete a file'

x_label 'file size (bytes)'

run do |argument|
  echo 'a' * argument, '1'

  unmount
  mount

  rm '1'
end
