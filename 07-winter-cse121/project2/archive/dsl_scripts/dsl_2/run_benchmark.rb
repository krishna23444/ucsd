#!/usr/bin/ruby -w

require 'fileutils'
require 'gruff'
require 'singleton'


ROOT_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), '..'))


class FileSystemBenchmark
  include Singleton

  attr_reader :reads, :writes, :seeks
  attr_accessor :debug

  def initialize
    @reads = []
    @writes = []
    @seeks = []
    
    @cwd_before_mount = nil

    @debug = false

    unmount
  end

  def run(arguments, &block)
    arguments.each do |argument|
      begin
        yield self, argument
      rescue => e
        puts e, e.backtrace.join("\n")
      end

      unmount

      parse_results
    end
  end

  def parse_results
    raise 'no benchmark output' unless File.exists?(benchmark_output_file)
    open(benchmark_output_file) do |file|
      @seeks << file.gets.to_i
      puts "seeks: #{@seeks.last}" if @debug

      @reads << file.gets.to_i
      puts "reads: #{@reads.last}" if @debug

      @writes << file.gets.to_i
      puts "writes: #{@writes.last}" if @debug
    end
    File.delete(benchmark_output_file)
  end

  def mkfs(size)
    unmount

    if File.exists?(disk_file)
      puts "rm #{disk_file}" if @debug
      File.delete(disk_file)
    end

    arguments = [mkfs_arguments].flatten.join(' ')
    command = mkfs_executable + ' ' + arguments + ' ' + size.to_s

    execute(command)
  end

  def sh(command)
    mount
    execute(command)
  end

  def mounted?
    regex = /#{fs_executable_name} on #{mount_directory} type fuse/
    not `mount`.grep(regex).empty?
  end

  def mount
    return if mounted?

    raise "#{name} mount w/o disk file" unless File.exists?(disk_file)

    unless File.exists?(mount_directory)
      puts "mkdir #{mount_directory}" if @debug
      Dir.mkdir(mount_directory)
    end

    execute("#{fs_executable} #{mount_directory}")

    @cwd_before_mount = Dir.pwd

    puts "cd #{mount_directory}" if @debug
    Dir.chdir(mount_directory)
  end

  def unmount
    return unless mounted?

    if @cwd_before_mount
      puts "cd #{@cwd_before_mount}" if @debug
      Dir.chdir(@cwd_before_mount)
    end

    execute("fusermount -u #{mount_directory}")
    sleep 0.5
  end

  def header
    '>> %s <<' % name
  end
 
  def execute(command)
    command += ' 2>&1'
    puts command if @debug

    output = `#{command}`
    puts output if @debug
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

  def mkfs_arguments
    disk_file
  end
end


class MyFileSystemBenchmark < FileSystemBenchmark
  def name
    'mine'
  end
  
  def directory_name
    'my_fs'
  end
  
  def fs_executable_name
    'cse121fs'
  end
  
  def mkfs_executable_name
    'mkcse121fs'
  end
    
  def mkfs_arguments
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


class Benchmark
  include Singleton

  attr_reader :name
  attr_accessor :format_size, :arguments

  def initialize
    reset!
  end

  def reset!
    @name = nil

    @fs_benchmarks = [MyFileSystemBenchmark.instance,
                      FatFileSystemBenchmark.instance,
                      InodeFileSystemBenchmark.instance]

    @reads_graph = Gruff::Line.new
    @writes_graph = Gruff::Line.new
    @seeks_graph = Gruff::Line.new

    @debug = false

    @format_size = nil
    @arguments = [0]
  end

  def run(&block)
    @fs_benchmarks.each do |fs_benchmark|
      puts fs_benchmark.header if @debug
      fs_benchmark.mkfs(@format_size) if @format_size
      fs_benchmark.run(@arguments, &block)
    end

    @fs_benchmarks.each do |fs_benchmark|
      @reads_graph.data(fs_benchmark.name, fs_benchmark.reads)
      @writes_graph.data(fs_benchmark.name, fs_benchmark.writes)
      @seeks_graph.data(fs_benchmark.name, fs_benchmark.seeks)
    end

    @reads_graph.title = graph_title + ' read count'
    @writes_graph.title = graph_title + ' write count'
    @seeks_graph.title = graph_title + ' seek total'

    @arguments.each_with_index do |argument, index|
      @reads_graph.labels[index] = argument.to_s
      @writes_graph.labels[index] = argument.to_s
      @seeks_graph.labels[index] = argument.to_s
    end

    puts "write #{graph_prefix + '_reads.png'}" if @debug
    @reads_graph.write(graph_prefix + '_reads.png')

    puts "write #{graph_prefix + '_writes.png'}" if @debug
    @writes_graph.write(graph_prefix + '_writes.png')

    puts "write #{graph_prefix + '_seeks.png'}" if @debug
    @seeks_graph.write(graph_prefix + '_seeks.png')
  end

  def name=(name)
    @name = name.gsub(/^.*benchmark_/, '').gsub(/\.rb$/, '')
  end

  def debug=(debug)
    @debug = debug
    @fs_benchmarks.each do |fs_benchmark|
      fs_benchmark.debug = debug
    end
  end

  def x_label=(name)
    @reads_graph.x_axis_label = name
    @writes_graph.x_axis_label = name
    @seeks_graph.x_axis_label = name
  end

  def y_label=(name)
    @reads_graph.y_axis_label = name
    @writes_graph.y_axis_label = name
    @seeks_graph.y_axis_label = name
  end

  private
  def graph_prefix
    File.join(ROOT_DIRECTORY, 'graphs', name)
  end

  def graph_title
    name.gsub('_', ' ')
  end
end


def format_disk_size_in_blocks(size)
  Benchmark.instance.format_size = size
end

def arguments(*arguments)
  Benchmark.instance.arguments = arguments
end

def debug
  Benchmark.instance.debug = true
end

def run(&block)
  Benchmark.instance.run(&block)
end

def x_label(name)
  Benchmark.instance.x_label = name
end

def y_label(name)
  Benchmark.instance.y_label = name
end


exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} benchmark_1_name [benchmark_2_name ...]"
  exit
end

ARGV.each do |arg|
  Benchmark.instance.reset!
  Benchmark.instance.name = arg

  puts "--- #{Benchmark.instance.name} ---"

  load File.join(Dir.pwd, arg)
end
