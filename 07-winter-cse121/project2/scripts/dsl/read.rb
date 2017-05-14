SCALE = 30000

format_size_in_blocks SCALE * 10

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4, SCALE * 5

title 'read a file'

x_label 'file size (bytes)'

run do |argument|
  num = argument / SCALE
  num.times do
    echo 'a' * SCALE, '1'
  end

  unmount
  mount

  cat '1'
end
