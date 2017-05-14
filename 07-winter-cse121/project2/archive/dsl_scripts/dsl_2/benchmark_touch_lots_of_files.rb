x_label 'number of files'

format_disk_size_in_blocks 100000
arguments 10, 100, 200, 500, 1000

run do |b, arg|
  1.upto(arg) do |i|
    b.sh 'touch ' + i.to_s
  end
end
