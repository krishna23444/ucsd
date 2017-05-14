SCALE = 10

format_size_in_blocks SCALE * 100

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

title 'deep ls'

x_label 'depth of directory structure'

run do |argument|
  path_array = ['a'] * argument
  path = path_array.join('/')

  mkdir path

  unmount
  mount

  ls path
end
