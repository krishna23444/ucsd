require 'fileutils'
require 'singleton'


class BenchmarkTest
  include Singleton

  ROOT_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), '..'))

  OUTPUT_DIRECTORY = File.join(ROOT_DIRECTORY, 'output')

  FAT_DIRECTORY = File.join(ROOT_DIRECTORY, 'fat_fs')
  INODE_DIRECTORY = File.join(ROOT_DIRECTORY, 'inode_fs')
  MY_DIRECTORY = File.join(ROOT_DIRECTORY, 'my_fs')
  CACHED_MY_DIRECTORY = File.join(ROOT_DIRECTORY, 'cached_my_fs')

  CONFIG_DATA = {
    FAT_DIRECTORY => {
      :name => 'fat',
      :fs_executable => File.join(FAT_DIRECTORY, 'fatfs'),
      :mkfs_executable => File.join(FAT_DIRECTORY, 'mkfatfs'),
      :benchmark_file => File.join(FAT_DIRECTORY, 'benchmark.txt'),
      :mount_directory => File.join(FAT_DIRECTORY, 'mount_directory'),
    },
    INODE_DIRECTORY => {
      :name => 'inode',
      :fs_executable => File.join(INODE_DIRECTORY, 'inodefs'),
      :mkfs_executable => File.join(INODE_DIRECTORY, 'mkinodefs'),
      :benchmark_file => File.join(INODE_DIRECTORY, 'benchmark.txt'),
      :mount_directory => File.join(INODE_DIRECTORY, 'mount_directory'),
    },
    MY_DIRECTORY => {
      :name => 'mine',
      :fs_executable => File.join(MY_DIRECTORY, 'cse121fs'),
      :mkfs_executable => File.join(MY_DIRECTORY, 'mkcse121fs'),
      :benchmark_file => File.join(MY_DIRECTORY, 'benchmark.txt'),
      :mount_directory => File.join(MY_DIRECTORY, 'mount_directory'),
    },
    CACHED_MY_DIRECTORY => {
      :name => 'cached_mine',
      :fs_executable => File.join(CACHED_MY_DIRECTORY, 'cse121fs'),
      :mkfs_executable => File.join(CACHED_MY_DIRECTORY, 'mkcse121fs'),
      :benchmark_file => File.join(CACHED_MY_DIRECTORY, 'benchmark.txt'),
      :mount_directory => File.join(CACHED_MY_DIRECTORY, 'mount_directory'),
    },
  }

  FUSE_UNMOUNT = '/usr/bin/fusermount -u'

  RUNNING_SCRIPT = $0
  RUNNING_SCRIPT_NAME = File.basename(RUNNING_SCRIPT)

  attr_writer :format_size_in_blocks

  def initialize
    @format_size_in_blocks = nil
  end

  def format?
    not @format_size_in_blocks.nil?
  end

  def run(&block)
    if ARGV.empty?
      puts "USAGE: #{RUNNING_SCRIPT} argument_1 [argument_2 ...]"
      exit
    end

    arguments = []
    ARGV.each do |argument|
      begin
        arguments << Integer(argument)
      rescue ArgumentError => e
        puts "ERROR: argument '#{argument}' not an integer"
        exit
      end
    end

    config_data = CONFIG_DATA[Dir.pwd]
    if config_data.nil?
      puts "cwd #{Dir.pwd} not recognized"
      exit
    end

    unmount if mounted?(config_data)
    arguments.each do |argument|
      `#{config_data[:mkfs_executable]} #{@format_size_in_blocks}` if format?
      `#{config_data[:fs_executable]} #{config_data[:mount_directory]}`
      Dir.chdir(config_data[:mount_directory]) do
        puts "run with #{argument}"
        begin
          yield argument
        rescue => e
          puts e, e.backtrace
        end
      end
      unmount
      sleep 0.3
      write_benchmark(config_data, argument)
    end
  end

  private
  def mounted?(config_data)
    mount_dir = config_data[:mount_directory]
    regex = /#{config_data[:fs_executable_name]} on #{mount_dir} type fuse/
    not `mount`.grep(regex).empty?
  end

  def unmount
    `#{FUSE_UNMOUNT} #{config_data[:mount_directory]}`
  end

  def write_benchmark(config_data, argument)
    unless File.exists?(config_data[:benchmark_file])
      puts "ERROR: #{config_data[:benchmark_file]} not found"
      return
    end

    script_name = RUNNING_SCRIPT_NAME.gsub(/(.*)\.rb/, '\1')
    output_name = script_name + '.' + config_data[:name] + '.' + argument.to_s
    output_path = File.join(OUTPUT_DIRECTORY, output_name)
    FileUtils.copy(config_data[:benchmark_file], output_path)
  end
end


def format_size_in_blocks(size_in_blocks)
  BenchmarkTest.instance.format_size_in_blocks = size_in_blocks
end

def run(&block)
  BenchmarkTest.instance.run(&block)
end

def touch(*files)
  `touch #{files.join(' ')}`
end
