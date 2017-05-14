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

      File.move(benchmark_output_file, benchmark_output_file + '.' + argument)
    end
  end

  def format(size)
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
  def fs_executable_name
    'cse121fs'
  end
  
  def mkfs_executable_name
    'mkcse121fs'
  end
    
  def mkfs_arguments
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


class MyCachedFileSystemBenchmark < FileSystemBenchmark
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


class Benchmark
  include Singleton

  attr_reader :name
  attr_accessor :debug, :format_size, :arguments

  def initialize
    reset!
  end

  def reset!
    @name = nil


    @reads_graph = Gruff::Line.new
    @writes_graph = Gruff::Line.new
    @seeks_graph = Gruff::Line.new

    @debug = false

    @format_size = nil
    @arguments = [0]
  end

  def run(&block)
    commands = []
    @arguments.each do |argument|
      yield
    end
  end

  def format(size_in_blocks)
    @filesystems.each do |fs|
      commands << fs.mkfs_executable + ' ' + size_in_blocks
    end
  end

  def name=(name)
    @name = name.gsub(/^.*benchmark_/, '').gsub(/\.rb$/, '')
  end
end


def format_size_in_blocks(size)
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


ALL_FILE_SYSTEMS = [MyOriginalFileSystemBenchmark.instance,
                    FatFileSystemBenchmark.instance,
                    InodeFileSystemBenchmark.instance]
