SCALE = 10

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'deep mkdir'

x_label 'depth of directory structure'

run do |argument|
  path_array = ['a'] * argument
  mkdir path_array.join('/')
end
