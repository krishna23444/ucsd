exit

x_label 'depth of directory'

format_disk_size_in_blocks 100000
arguments 10, 20, 50, 100, 200, 500

run do |b, arg|
  directory = (1..arg).to_a.join('/')
  b.sh 'mkdir -p ' + directory
end
