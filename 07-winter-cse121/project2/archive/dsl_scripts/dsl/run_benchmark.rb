#!/usr/bin/ruby -w

require 'fileutils'
require 'gruff'
require 'singleton'


ROOT_DIRECTORY = File.join(File.dirname(__FILE__), '..')


class FileSystem
  include Singleton

  attr_accessor :debug, :pretend
  attr_reader :reads, :writes, :average_offsets

  def initialize
    @mounted = false

    @debug = false
    @pretend = false

    @reads = []
    @writes = []
    @average_offsets = []
  end

  def mkfs(size, measure)
    unmount

    if File.exists?(disk_file)
      puts "rm #{disk_file}" if @debug
      File.delete(disk_file)
    end

    execute("#{mkfs_executable} #{mkfs_arguments} #{size}")
    parse_last_run(measure) unless @pretend
  end

  def sh(command)
    mount
    execute(command)
  end

  def mount
    return if @mounted

    unless File.exists?(disk_file) or @pretend
      raise "#{name} mount w/o disk file" 
    end

    @cwd_before_mount = Dir.pwd

    unless File.exists?(mount_directory)
      puts "mkdir #{mount_directory}" if @debug
      Dir.mkdir(mount_directory) unless @pretend
    end

    execute("#{fs_executable} #{mount_directory}")
    @mounted = true

    puts "cd #{mount_directory}" if @debug
    Dir.chdir(mount_directory) unless @pretend
  end

  def unmount
    return unless @mounted

    puts "cd #{@cwd_before_mount}" if @debug
    Dir.chdir(@cwd_before_mount) unless @pretend

    execute("fusermount -u #{mount_directory}")
    @mounted = false

    sleep 1
    parse_last_run unless @pretend
  end

  def header
    '>> %s <<' % name
  end
 
  protected
  def execute(command)
    command += ' 2>&1'
    puts command if @debug

    return if @pretend

    output = `#{command}`
    puts output if @debug
  end

  def disk_file
    File.join(directory, 'disk.txt')
  end

  def mount_directory
    File.join(directory, 'mount_directory')
  end

  def benchmark_output_file
    File.join(directory, 'benchmark.txt')
  end

  def mkfs_arguments
    disk_file
  end
  
  def parse_last_run(measure=true)
    if measure
      unless File.exists?(benchmark_output_file)
        error = -999999999
        @reads << error
        @writes << error
        @average_offsets << error
        return
      end
      open(benchmark_output_file) do |file|
        offsets = file.gets.split(',').map { |s| s.to_i }
        offset_total = offsets.inject(0) { |sum,n| sum + n }
        @reads << file.gets.to_i
        puts "reads: #{@reads.last}" if @debug

        @writes << file.gets.to_i
        puts "writes: #{@writes.last}" if @debug

        @average_offsets << offset_total.to_f / (@reads.last + @writes.last)
        puts "average_offsets: #{@average_offsets.last}" if @debug
      end
    end
    File.delete(benchmark_output_file) if File.exists?(benchmark_output_file)
  end
end


class MyFileSystem < FileSystem
  def name
    'mine'
  end

  protected
  def directory
    File.join(ROOT_DIRECTORY, 'my_fs')
  end
  
  def fs_executable
    File.join(directory, 'cse121fs')
  end
  
  def mkfs_executable
    File.join(directory, 'mkcse121fs')
  end
    
  def mkfs_arguments
  end
end


class FatFileSystem < FileSystem
  def name
    'fat'
  end

  protected
  def directory
    File.join(ROOT_DIRECTORY, 'fat_fs')
  end
  
  def fs_executable
    File.join(directory, 'fatfs')
  end
  
  def mkfs_executable
    File.join(directory, 'mkfatfs')
  end
end


class InodeFileSystem < FileSystem
  def name
    'inode'
  end

  protected
  def directory
    File.join(ROOT_DIRECTORY, 'inode_fs')
  end
  
  def fs_executable
    File.join(directory, 'inodefs')
  end
  
  def mkfs_executable
    File.join(directory, 'mkinodefs')
  end
end


class Benchmark
  include Singleton

  attr_accessor :mkfs_size, :benchmark_sizes
  attr_reader :name

  def initialize
    @debug = false
    @pretend = false

    @filesystems = [MyFileSystem.instance,
                    FatFileSystem.instance,
                    InodeFileSystem.instance]

    @mkfs_size = nil
    @benchmark_sizes = [0]

    @name = nil
    @type = nil
  end

  def initialized?
    @name and @type
  end

  def run(&block)
    raise "unitialized benchmark" unless initialized?

    @filesystems.each do |fs|
      benchmark_sizes.each do |size|
        @current_fs = fs
        puts fs.header if @debug

        fs.mkfs(mkfs_size, false) if mkfs_size
        begin
          yield self, size
        rescue => e
          puts "ERROR: " + e.backtrace
        end
        fs.unmount
      end
    end

    @filesystems.each do |fs|
      @reads_graph.data(fs.name, fs.reads)
      @writes_graph.data(fs.name, fs.writes)
      @avg_offsets_graph.data(fs.name, fs.average_offsets)
    end

    @reads_graph.title = graph_title + ' read count'
    @writes_graph.title = graph_title + ' write count'
    @avg_offsets_graph.title = graph_title + ' average seek'

    benchmark_sizes.each_with_index do |size, index|
      @reads_graph.labels[index] = size.to_s
      @writes_graph.labels[index] = size.to_s
      @avg_offsets_graph.labels[index] = size.to_s
    end

    puts "write #{graph_prefix + '_reads.png'}" if @debug
    @reads_graph.write(graph_prefix + '_reads.png') unless @pretend

    puts "write #{graph_prefix + '_writes.png'}" if @debug
    @writes_graph.write(graph_prefix + '_writes.png') unless @pretend

    puts "write #{graph_prefix + '_avg_offsets.png'}" if @debug
    @avg_offsets_graph.write(graph_prefix + '_avg_offsets.png') unless @pretend
  end

  def type=(benchmark_type)
    case benchmark_type
    when :bar
      @reads_graph = Gruff::Bar.new
      @writes_graph = Gruff::Bar.new
      @avg_offsets_graph = Gruff::Bar.new
    when :line
      @reads_graph = Gruff::Line.new
      @writes_graph = Gruff::Line.new
      @avg_offsets_graph = Gruff::Line.new
    end
    @type = true
  end

  def name=(benchmark_name)
    @name = benchmark_name.gsub(/^.*benchmark_/, '').gsub(/\.rb$/, '')
  end

  def x_label=(name)
    raise 'type not specified' unless @type
    @reads_graph.x_axis_label = name
    @writes_graph.x_axis_label = name
    @avg_offsets_graph.x_axis_label = name
  end

  def y_label=(name)
    raise 'type not specified' unless @type
    @reads_graph.y_axis_label = name
    @writes_graph.y_axis_label = name
    @avg_offsets_graph.y_axis_label = name
  end

  def debug=(status)
    @debug = status
    @filesystems.each do |fs|
      fs.debug = status
    end
  end

  def pretend=(status)
    @pretend = status
    @filesystems.each do |fs|
      fs.pretend = status
    end
  end

  def mkfs(size)
    @current_fs.mkfs(size, true)
  end

  def sh(command)
    @current_fs.sh(command)
  end

  private
  def graph_prefix
    File.join(ROOT_DIRECTORY, 'graphs', name)
  end

  def graph_title
    name.gsub('_', ' ')
  end
end


def type(benchmark_type)
  Benchmark.instance.type = benchmark_type
end

def mkfs_size(size)
  Benchmark.instance.mkfs_size = size
end

def benchmark_sizes(*sizes)
  Benchmark.instance.benchmark_sizes = sizes
end

def run(&block)
  Benchmark.instance.run(&block)
end

def debug(&block)
  Benchmark.instance.debug = true
  run(&block)
end

def pretend(&block)
  Benchmark.instance.pretend = true
  debug(&block)
end

def x_label(name)
  Benchmark.instance.x_label = name
end

def y_label(name)
  Benchmark.instance.y_label = name
end


exit unless __FILE__ == $0

if ARGV.empty?
  puts "USAGE: #{__FILE__} <benchmark_name>"
  exit
end

ARGV.each do |arg|
  Benchmark.instance.name = arg

  puts "--- #{Benchmark.instance.name} ---"

  load File.join(Dir.pwd, arg)
end
