arguments 100, 2000, 5000, 10000

x_label 'format size (blocks)'

run do |b, arg|
  b.format arg
end
