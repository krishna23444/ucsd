#!/usr/bin/ruby -w

require 'singleton'
require 'yaml'
require File.join(File.dirname(__FILE__), 'common')


module Benchmark
  class BenchmarkBase < Base
    BASH_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), 'bash'))

    BASH = '/bin/bash'

    private
    def create_bash_file(bash_file_path)
      bash_file = File.new(bash_file_path, 'w', 0755)
      bash_file << '#!' + BASH + "\n\n"
    end
  end


  class SingleBenchmark < BenchmarkBase
    FAT_DIRECTORY = File.join(ROOT_DIRECTORY, 'fat_fs')
    INODE_DIRECTORY = File.join(ROOT_DIRECTORY, 'inode_fs')
    MY_DIRECTORY = File.join(ROOT_DIRECTORY, 'my_fs')
    CACHED_MY_DIRECTORY = File.join(ROOT_DIRECTORY, 'cached_my_fs')

    CONFIG = {
      :fat => {
        :name => 'fat',
        :directory => File.join(ROOT_DIRECTORY, 'fat_fs'),
        :fs => File.join(FAT_DIRECTORY, 'fatfs'),
        :mkfs => File.join(FAT_DIRECTORY, 'mkfatfs'),
        :benchmark => File.join(FAT_DIRECTORY, 'benchmark.txt'),
        :mount_point => File.join(FAT_DIRECTORY, 'mount_directory'),
      },
      :inode => {
        :name => 'inode',
        :directory => File.join(ROOT_DIRECTORY, 'inode_fs'),
        :fs => File.join(INODE_DIRECTORY, 'inodefs'),
        :mkfs => File.join(INODE_DIRECTORY, 'mkinodefs'),
        :benchmark => File.join(INODE_DIRECTORY, 'benchmark.txt'),
        :mount_point => File.join(INODE_DIRECTORY, 'mount_directory'),
      },
      :mine => {
        :name => 'sffs',
        :directory => File.join(ROOT_DIRECTORY, 'my_fs'),
        :fs => File.join(MY_DIRECTORY, 'cse121fs'),
        :mkfs => File.join(MY_DIRECTORY, 'mkcse121fs'),
        :benchmark => File.join(MY_DIRECTORY, 'benchmark.txt'),
        :mount_point => File.join(MY_DIRECTORY, 'mount_directory'),
      },
#      :cached_mine => {
#        :name => 'sffs+cache',
#        :directory => File.join(ROOT_DIRECTORY, 'cached_my_fs'),
#        :fs => File.join(CACHED_MY_DIRECTORY, 'cse121fs'),
#        :mkfs => File.join(CACHED_MY_DIRECTORY, 'mkcse121fs'),
#        :benchmark => File.join(CACHED_MY_DIRECTORY, 'benchmark.txt'),
#        :mount_point => File.join(CACHED_MY_DIRECTORY, 'mount_directory'),
#      },
    }

    SCRIPTS_DIRECTORY = File.expand_path(File.join(ROOT_DIRECTORY, 'scripts'))
    GRAPH_SCRIPT_PATH = File.join(SCRIPTS_DIRECTORY, 'generate_graphs.rb')

    attr_reader :bash_file_path, :graph_config, :name
    attr_writer :arguments, :format_size

    def initialize(file_path)
      @name = File.basename(file_path).gsub(/\.rb$/, '')

      @bash_file_path = File.join(BASH_DIRECTORY, @name + '.sh')
      
      @graph_config = {
        :title => @name
      }

      @format_size = nil
      @arguments = [0]
    end

    def run(&block)
      CONFIG.each_value do |fs|
        fs[:bash_path] = @bash_file_path.gsub(/sh$/, fs[:name].gsub(' ', '_') + '.sh')
        fs[:bash] = create_bash_file(fs[:bash_path])

        fs[:bash] << "rm -rf #{fs[:mount_point]}\n"
        fs[:bash] << "mkdir #{fs[:mount_point]}\n\n"

        @fs = fs
        unmount

        fs[:bash] << "\n"

        @arguments.each do |argument|
          fs[:bash] << 'echo -- ' + output_name_for(argument) << "\n"
          if @format_size
            format(@format_size)
            mount
          end

          yield argument

          unmount if @format_size

          mv(fs[:benchmark], output_path_for(argument))

          fs[:bash] << "\n"
        end
      end

      write_bash_script
    end

    def cat(files)
      @fs[:bash] << '/usr/bin/cat ' + files.join(' ') + " &> /dev/null\n"
    end

    def echo(data, file)
      @fs[:bash] << 'echo ' + data + ' >> ' + file + "\n"
    end

    def touch(files)
      @fs[:bash] << '/usr/bin/touch ' + files.join(' ') + "\n"
    end

    def mv(from, to)
      @fs[:bash] << '/usr/bin/mv ' + from + ' ' + to + "\n"
    end

    def cd(directory)
      @fs[:bash] << 'cd ' + directory + "\n"
    end

    def rm(files)
      @fs[:bash] << '/usr/bin/rm ' + files.join(' ') + "\n"
    end

    def mkdir(directories)
      @fs[:bash] << '/usr/bin/mkdir -p ' + directories.join(' ') + "\n"
    end

    def ls(files)
      @fs[:bash] << '/usr/bin/ls ' + files.join(' ') + "\n"
    end

    def ls_long(files)
      ls(['-l'] + files)
    end

    def sleep(time)
      @fs[:bash] << '/usr/bin/sleep ' + time.to_s << "\n"
    end

    def format(format_size)
      @fs[:bash] << @fs[:mkfs] + ' ' + format_size.to_s + "\n"
    end

    def mount
      @fs[:bash] << @fs[:fs] + ' ' + @fs[:mount_point] + "\n"
      cd @fs[:mount_point]
    end

    def unmount
      cd @fs[:directory]
      sleep 0.3
      @fs[:bash] << '/usr/bin/fusermount -u' + ' ' + @fs[:mount_point] + "\n"
      sleep 0.3
    end

    private
    def output_prefix
      File.join(OUTPUT_DIRECTORY, @name)
    end

    def output_name_for(argument)
      [@name, @fs[:name], argument.to_s].join('.')
    end

    def output_path_for(argument)
      output_prefix + '.' + @fs[:name] + '.' + argument.to_s
    end

    def graph_prefix
      File.join(GRAPHS_DIRECTORY, @name)
    end
    
    def write_bash_script
      bash_file = create_bash_file(@bash_file_path)

      bash_file << "rm -f #{output_prefix}*\n"
      bash_file << "rm -f #{graph_prefix}*\n\n"

      CONFIG.each_value do |fs|
        bash_file << BASH + ' ' + fs[:bash_path] + "\n"
      end
      bash_file << "\n" + 'wait' + "\n\n"

      graph_yaml_config = YAML::dump(@graph_config)
      graph_yaml_config_path = graph_yaml_config_path_for(@name)
      bash_file << "echo '#{graph_yaml_config}' > #{graph_yaml_config_path}\n"

      bash_file << GRAPH_SCRIPT_PATH + ' ' + @name + "\n"
    end
  end


  class MultipleBenchmarks < BenchmarkBase
    include Singleton

    attr_reader :current

    def initialize
      @benchmarks = []
      @current = nil
    end

    def add(file_path)
      @benchmarks << SingleBenchmark.new(file_path)
      @current = @benchmarks.last
    end

    def write_bash_script
      bash_file_path = File.join(BASH_DIRECTORY, 'all_benchmarks.sh')
      bash_file = create_bash_file(bash_file_path)
      @benchmarks.each do |benchmark|
        bash_file << 'echo ++ ' + benchmark.name + "\n"
        bash_file << BASH + ' ' + benchmark.bash_file_path + " &> /dev/null\n"
      end
      bash_file << "\n" + 'wait' + "\n"
    end
  end
end


def all_benchmarks
  Benchmark::MultipleBenchmarks.instance
end

def current_benchmark
  all_benchmarks.current
end

def format_size_in_blocks(size_in_blocks)
  current_benchmark.format_size = size_in_blocks
end

def arguments(*arguments)
  current_benchmark.arguments = arguments
end

def x_label(x_label)
  current_benchmark.graph_config[:x_label] = x_label
end

def title(title)
  current_benchmark.graph_config[:title] = title
end

def run(&block)
  current_benchmark.run(&block)
end

def format(size_in_blocks)
  current_benchmark.format(size_in_blocks)
end

def touch(*files)
  current_benchmark.touch(files)
end

def rm(*files)
  current_benchmark.rm(files)
end

def mv(from, to)
  current_benchmark.mv(from, to)
end

def cat(*files)
  current_benchmark.cat(files)
end

def echo(data, file)
  current_benchmark.echo(data, file)
end

def mkdir(*directories)
  current_benchmark.mkdir(directories)
end

def ls(*files)
  current_benchmark.ls(files)
end

def ls_long(*files)
  current_benchmark.ls_long(files)
end

def sleep(time)
  current_benchmark.sleep(time)
end

def mount
  current_benchmark.mount
end

def unmount
  current_benchmark.unmount
end


exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} path_to_ruby_file_1 [path_to_ruby_file_2 ...]"
  exit
end

ARGV.each do |argument|
  all_benchmarks.add(argument)
  load argument
end

all_benchmarks.write_bash_script if ARGV.size > 1
