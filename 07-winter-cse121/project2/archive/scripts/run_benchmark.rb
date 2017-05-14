#!/usr/bin/ruby -w

require 'fileutils'
require 'gruff'
require 'singleton'


ROOT_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), '..', '..'))
BENCHMARK_DIRECTORY = File.join(ROOT_DIRECTORY, 'benchmark')
OUTPUT_DIRECTORY = File.join(BENCHMARK_DIRECTORY, 'output')
GRAPHS_DIRECTORY = File.join(BENCHMARK_DIRECTORY, 'graphs')


class FileSystemBenchmark
  include Singleton

  attr_writer :arguments, :benchmark_name, :debug, :format_size
  attr_reader :seeks, :reads, :writes

  def initialize
    @arguments = [0]
    @debug = false
    @benchmark_name = nil
    @format_size = nil

    @seeks = []
    @reads = []
    @writes = []
  end

  def run(&block)
    unmount

    @arguments.each do |argument|
      begin
        puts "#{name}: run with #{argument}"

        if @format_size
          format(@format_size)
          mount
        end

        yield self, argument

        unmount

#        parse_output
        write_output(argument)

        puts if @debug
      rescue => e
        puts e, e.backtrace
      end
    end
  end

  def mount
    execute("#{fs_executable} #{mount_directory}")
    sleep 0.5

    Dir.chdir(mount_directory)
  end

  def unmount
    Dir.chdir(ROOT_DIRECTORY)

    execute("fusermount -u #{mount_directory}")
    sleep 1
  end

  def format(size_in_blocks)
    execute(mkfs_executable + ' ' + size_in_blocks.to_s)
  end

  def echo(file, string)
    execute("echo -n '#{string}' >> #{file}")
  end

  def touch(*files)
    files.each do |file|
      puts "#{name}.touch #{file}" if @debug
      FileUtils.touch(file.to_s)
    end
  end

  def mv(from, to)
    puts "#{name}: mv #{from} #{to}" if @debug
    FileUtils.move(from, to)
  end

  def mkdir(*directories)
    puts "#{name}: mkdir #{directories.join(' ')}" if @debug
    FileUtils.mkdir(directories)
  end

  def rm(*files)
    puts "#{name}: rm #{files.join(' ')}" if @debug
    FileUtils.rm(files)
  end

  def ls(*paths)
    execute("ls #{paths.join(' ')}")
  end

  def ls_long(*paths)
    ls('-l', *paths)
  end

  def cat(*files)
    execute("cat #{files.join(' ')}")
  end

  def execute(command)
    command += ' 2>&1'
    puts name + ': ' + command if @debug
    output = `#{command}`
    puts output if @debug
  end

  def parse_output
    unless File.exists?(benchmark_output_file)
      puts "#{name}.parse_output: no benchmark file" if @debug
      return
    end

    open(benchmark_output_file, 'r') do |file|
      @seeks << file.gets.to_i
      @reads << file.gets.to_i
      @writes << file.gets.to_i
    end
  end

  def write_output(argument)
    unless File.exists?(benchmark_output_file)
      puts "#{name}.write_output: no benchmark file" if @debug
      return
    end

    output_file_name = [@benchmark_name, name, argument.to_s].join('.')
    output_file_path = File.join(OUTPUT_DIRECTORY, output_file_name)

    puts "cp #{benchmark_output_file} #{output_file_path}" if @debug
    FileUtils.copy(benchmark_output_file, output_file_path)
  end

  def directory
    File.expand_path(File.join(ROOT_DIRECTORY, directory_name))
  end

  def disk_file
    File.expand_path(File.join(directory, 'disk.txt'))
  end

  def mount_directory
    File.expand_path(File.join(directory, 'mount_directory'))
  end

  def benchmark_output_file
    File.expand_path(File.join(directory, 'benchmark.txt'))
  end

  def fs_executable
    File.expand_path(File.join(directory, fs_executable_name))
  end

  def mkfs_executable
    File.expand_path(File.join(directory, mkfs_executable_name))
  end
end


class MyFileSystemBenchmark < FileSystemBenchmark
  def fs_executable_name
    'cse121fs'
  end
  
  def mkfs_executable_name
    'mkcse121fs'
  end
end


class MyOriginalFileSystemBenchmark < MyFileSystemBenchmark
  def name
    'mine'
  end

  def directory_name
    'my_fs'
  end
end


class MyCachedFileSystemBenchmark < MyFileSystemBenchmark
  def name
    'mine with cache'
  end

  def directory_name
    'cached_my_fs'
  end
end


class FatFileSystemBenchmark < FileSystemBenchmark
  def name
    'fat'
  end
  
  def directory_name
    'fat_fs'
  end
  
  def fs_executable_name
    'fatfs'
  end
  
  def mkfs_executable_name
    'mkfatfs'
  end
end


class InodeFileSystemBenchmark < FileSystemBenchmark
  def name
    'inode'
  end

  def directory_name
    'inode_fs'
  end
  
  def fs_executable_name
    'inodefs'
  end
  
  def mkfs_executable_name
    'mkinodefs'
  end
end


class AllBenchmarks
  include Singleton

  attr_writer :x_label, :y_label

  def initialize
    @all_file_systems = [MyOriginalFileSystemBenchmark.instance,
                         MyCachedFileSystemBenchmark.instance,
                         FatFileSystemBenchmark.instance,
                         InodeFileSystemBenchmark.instance]

    @x_label = nil

    @arguments = nil
    @name = nil
  end

  def initialized?
    @name and @arguments
  end

  def run(&block)
    raise 'not initialized' unless initialized?

    child_pids = []
    @all_file_systems.each do |fs|
      child_pids << fork do
        fs.run(&block)
      end
    end

    child_pids.each do |pid|
      Process.wait(pid)
    end

    write_graphs
  end

  def write_graphs
    graph_file_prefix = File.join(GRAPHS_DIRECTORY, @name)

    graph_descriptions = [
      { :data_method => 'reads',
        :title_append => ' disk reads',
        :path_append => '_reads.png',
        :y_label => 'dread count (#)',
      },
      { :data_method => 'writes',
        :title_append => ' disk writes',
        :path_append => '_writes.png',
        :y_label => 'dwrite count (#)',
      },
      { :data_method => 'seeks',
        :title_append => ' disk seeks',
        :path_append => '_seeks.png',
        :y_label => 'total disk seek (blocks)'
      },
    ]

    graph_descriptions.each do |graph_description|
      graph = Gruff::Line.new

      graph.title = @name + graph_description[:title_append]
      graph.x_axis_label = @x_label
      graph.y_axis_label = graph_description[:y_label]

      @all_file_systems.each do |fs|
        data = fs.send(graph_description[:data_method])
        graph.data(fs.name, data)
      end

      @arguments.each_with_index do |argument, index|
        graph.labels[index] = argument.to_s
      end

      graph_file_path = graph_file_prefix + graph_description[:path_append]
      graph.write(graph_file_path)
    end
  end

  def arguments=(arguments)
    @arguments = arguments
    @all_file_systems.each do |fs|
      fs.arguments = arguments
    end
  end

  def format_size=(format_size)
    @all_file_systems.each do |fs|
      fs.format_size = format_size
    end
  end

  def name=(name)
    @name = name.gsub(/.*benchmark_(.*)\.rb/, '\1')
    @all_file_systems.each do |fs|
      fs.benchmark_name = @name
    end
  end
end


def arguments(*args)
  AllBenchmarks.instance.arguments = args
end

def format_size(format_size)
  AllBenchmarks.instance.format_size = format_size
end

def run(&block)
  AllBenchmarks.instance.run(&block)
end

def x_label(x_label)
  AllBenchmarks.instance.x_label = x_label
end


exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} <path_to_benchmark_dsl_file>"
  exit
end

AllBenchmarks.instance.name = ARGV.first

load ARGV.first
