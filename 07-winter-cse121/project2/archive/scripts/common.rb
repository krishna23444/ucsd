require 'fileutils'
require 'singleton'


ROOT_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), '..', '..'))
BENCHMARK_DIRECTORY = File.join(ROOT_DIRECTORY, 'benchmark')
OUTPUT_DIRECTORY = File.join(BENCHMARK_DIRECTORY, 'output')
GRAPHS_DIRECTORY = File.join(BENCHMARK_DIRECTORY, 'graphs')


class FileSystem
  include Singleton

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


class MyFileSystem < FileSystem
  def fs_executable_name
    'cse121fs'
  end
  
  def mkfs_executable_name
    'mkcse121fs'
  end
end


class MyOriginalFileSystem < MyFileSystem
  def name
    'mine'
  end

  def directory_name
    'my_fs'
  end
end


class MyCachedFileSystem < MyFileSystem
  def name
    'mine with cache'
  end

  def directory_name
    'cached_my_fs'
  end
end


class FatFileSystem < FileSystem
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


class InodeFileSystem < FileSystem
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

  attr_writer :debug

  def initialize
    @debug = false
  end

  def run(cmd)
    cmd += ' 2>&1'
    puts cmd if @debug
    output = `#{cmd}`
    puts output if @debug
  end

  def touch(*args)
    args.each do |arg|
      puts "touch #{arg}" if @debug
      FileUtils.touch(arg)
    end
  end

  def mv(from, to)
    puts "mv #{from} #{to}" if @debug
    FileUtils.move(from, to)
  end

  def mkdir(directory)
    puts "mkdir #{directory}" if @debug
    FileUtils.mkdir(directory)
  end

  def rm(*files)
    puts "rm #{files.join(' ')}" if @debug
    FileUtils.rm(files)
  end

  def format(fs, size)
    run(fs.mkfs_executable + ' ' + size)
  end

  def mount(fs)
  end

  def unmount(fs)
  end
end


def debug
  Benchmark.instance.debug = true
end

def run(cmd)
  Benchmark.instance.run(cmd)
end

def touch(*args)
  Benchmark.touch(*args)
end

def mv(from, to)
  Benchmark.instance.mv(from, to)
end

def mkdir(directory)
  Benchmark.instance.rm(directory)
end

def rm(*files)
  Benchmark.instance.rm(file)
end


ALL_FILE_SYSTEMS = [MyOriginalFileSystem.instance,
                    FatFileSystem.instance,
                    InodeFileSystem.instance]
