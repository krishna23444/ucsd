#!/usr/bin/ruby -w

exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} number_of_nodes"
  exit
end

`rm -f client*`

1.upto(ARGV.first.to_i) do |i|
  i -= 1
  `./TritonShare_gui param.zeus#{i} > client#{i}.out 2> client#{i}.err &`
end
