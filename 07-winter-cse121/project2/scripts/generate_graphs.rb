#!/usr/bin/ruby -w

require 'gruff'
require 'pp'
require 'yaml'
require File.join(File.dirname(__FILE__), 'common')


module Benchmark
  class GraphGenerator < Base
    def initialize(benchmark_name)
      @benchmark_name = benchmark_name
      
      @config_path = graph_yaml_config_path_for(benchmark_name)
      @config = {}
    end

    def parse_and_write
      parse_graph_description
      write_graphs(parse_output_files)
    end

    private
    def parse_graph_description
      return unless File.exists?(@config_path)
      @config = YAML::load_file(@config_path)
    end

    def parse_output_files
      files = Dir.glob(File.join(OUTPUT_DIRECTORY, @benchmark_name + '*')).sort
      files -= [@config_path]

      return [] if files.empty?

      items = []
      files.each do |file_path|
        name, fs_name, size = file_path.split('.')

        name.gsub!(/.*\//, '')
        size = size.to_i

        item = [name, fs_name, size]
        open(file_path) do |file|
          item << file.gets.to_i
          item << file.gets.to_i
          item << file.gets.to_i
        end
      
        items << item
      end
      
      items.sort! do |x,y|
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

      items
    end

    def write_graphs(items)
      data = {}
      items.each do |item|
        name, fs_name, size, seeks, reads, writes = *item

        data[name] = {
          'seeks' => {
            :y_label => 'disk seek (blocks)'
          },
          'reads' => {
            :y_label => 'disk reads (#)'
          },
          'writes' => {
            :y_label => 'disk writes (#)'
          },
        } unless data.has_key?(name)

        data[name]['sizes'] ||= []
        data[name]['sizes'] |= [size]

        data[name]['seeks'][fs_name] ||= []
        data[name]['seeks'][fs_name] << seeks

        data[name]['reads'][fs_name] ||= []
        data[name]['reads'][fs_name] << reads

        data[name]['writes'][fs_name] ||= []
        data[name]['writes'][fs_name] << writes
      end

      data.each do |name, item_data|
        labels = {}
        item_data['sizes'].each_with_index do |x,i|
          labels[i] = x.to_s
        end
        item_data.delete_if { |k,v| k == 'sizes' }

        item_data.each do |type,item_info|
          graph = Gruff::Line.new

          graph.title = ''
          if @config[:title]
            graph.title << @config[:title]
          else
            graph.title << name
          end
          graph.title << ' disk ' + type

          graph.labels = labels
          graph.x_axis_label = @config[:x_label]

          graph.y_axis_label = item_info[:y_label]
          item_info.delete_if { |k,v| k == :y_label }

          # TODO: WTF, WTF, WTF... why doesn't this work?!?!
          item_info.each_pair do |fs_name,values|
            graph.data(fs_name, values)
          end
          filename = [name,type].join('_')
          graph.write(File.join(GRAPHS_DIRECTORY, filename + '.png'))
        end
      end
    end
  end
end


exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} benchamrk_1 [benchmark_2 ...]" 
  exit
end

ARGV.each do |benchmark_name|
  Benchmark::GraphGenerator.new(benchmark_name).parse_and_write
end
