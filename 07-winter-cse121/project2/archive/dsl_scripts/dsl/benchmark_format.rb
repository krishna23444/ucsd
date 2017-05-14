type :bar

x_label 'disk size (blocks)'

benchmark_sizes 100, 2000, 10000, 30000, 200000

run do |b, size|
  b.mkfs(size)
end
