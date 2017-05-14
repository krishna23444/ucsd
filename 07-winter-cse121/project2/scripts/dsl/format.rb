SCALE = 1000

arguments SCALE, SCALE * 2, SCALE * 3, SCALE * 4

x_label 'format size (blocks)'

run do |argument|
  format argument
end
