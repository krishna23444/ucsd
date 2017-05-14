type :bar

x_label 'number of files'

mkfs_size 100000
benchmark_sizes 10, 100, 10

run do |b, size|
  1.upto(size) do |i|
    b.sh 'touch ' + i.to_s
  end
end
