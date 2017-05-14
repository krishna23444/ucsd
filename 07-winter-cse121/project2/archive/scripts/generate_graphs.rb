#!/usr/bin/ruby -w

exit unless __FILE__ == $0

require File.join(File.dirname(__FILE__), 'common')
require 'gruff'


files = []
Dir.glob(File.join(OUTPUT_DIRECTORY, '*')).sort.each do |file_path|
  name, fs_name, size = file_path.split('.')

  name.gsub!(/.*\//, '')
  size = size.to_i

  item = [name, fs_name, size]
  open(file_path) do |file|
    item << file.gets.to_i
    item << file.gets.to_i
    item << file.gets.to_i
  end
  
  files << item
end

files.sort! do |x,y|
  if x[0] == y[0]
    if x[1] == y[1]
      x[2] <=> y[2]
    else
      x[1] <=> y[1]
    end
  else
    x[0] <=> y[0]
  end
end


data = {}
files.each do |item|
  name, fs_name, size, seeks, reads, writes = *item

  data[name] = {
    'seeks' => {},
    'reads' => {},
    'writes' => {},
  } unless data.has_key?(name)

  data[name]['sizes'] = [] unless data[name].has_key?('sizes')
  data[name]['sizes'] |= [size]

  data[name]['seeks'][fs_name]=[] unless data[name]['seeks'].has_key?(fs_name)
  data[name]['seeks'][fs_name] << seeks

  data[name]['reads'][fs_name]=[] unless data[name]['reads'].has_key?(fs_name)
  data[name]['reads'][fs_name] << reads

  data[name]['writes'][fs_name]=[] unless data[name]['writes'].has_key?(fs_name)
  data[name]['writes'][fs_name] << writes
end

data.each do |name,item_data|
  labels = {}
  item_data['sizes'].each_with_index do |x,i|
    labels[i] = x.to_s
  end

  item_data.delete_if { |k,v| k == 'sizes' }

  item_data.each do |type,item|
    filename = [name,type].join('_')
    graph = Gruff::Line.new
    graph.title = filename.gsub('_', ' ')
    graph.labels = labels
    item.each do |fs_name,values|
      graph.data(fs_name, values)
    end
    graph.write(File.join(GRAPHS_DIRECTORY, filename + '.png'))
  end
end
