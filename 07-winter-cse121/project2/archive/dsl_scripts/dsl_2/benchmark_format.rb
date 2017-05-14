x_label 'disk size (blocks)'

arguments 100, 2000, 10000, 30000, 200000

run do |b, arg|
  b.mkfs(arg)
end
