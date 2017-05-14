#!/usr/bin/ruby -w

COMMANDS = [
  'mkdir -p a/b/c/d/e',
  'mkdir -p 1/2/3/4/5',
  'ls -l 1/2',
  'mkdir qwertyuiopasdfghjklzxcvbnm11',
  'touch blahdifoo',
  'touch 1/2/3/aa',
  'echo "fl3k4s9dfusdank" >> a/b/c/0485',
  'cat 1/2/3/aa',
  'ls -l 1/2',
  'echo "sldj233-0cvupaksdfp3428" > 1/2/foo',
  'ls -l 1/2/3',
  'echo "23409sdflkj" > 1/2/3/bar',
  'cat 1/2/foo',
  'ls -l 1/2',
  'ls -l 1/2/3',
  'rm 1/2/foo',
  'rm 1/2/foo',
  'ls -l 1/2/3',
  'ls -l 1/2',
].freeze

def run_cmd(command)
  puts
  puts '-- ' +  command
  puts `#{command}`
end

Dir::chdir('mount_directory') do
  COMMANDS.each do |cmd|
    run_cmd(cmd)
  end
end
