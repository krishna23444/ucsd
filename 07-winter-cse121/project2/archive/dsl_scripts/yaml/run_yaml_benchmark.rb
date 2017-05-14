#!/usr/bin/ruby -w

require 'pp'
require 'yaml'

exit unless __FILE__ == $0

if ARGV.length < 2
  puts "USAGE: #{__FILE__} config,yaml benchmark1.yaml [benchmark2.yaml ...]"
  exit
end

config_yaml = YAML::load_file(ARGV.shift)

pp config_yaml
puts
ARGV.each do |benchmark_yaml_file_path|
  benchmark_yaml = YAML::load_file(benchmark_yaml_file_path)
  pp benchmark_yaml
end
