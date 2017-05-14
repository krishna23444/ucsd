MAX_WRITE_SIZE = 100000

arguments 100, 1000, 10000, 100000, MAX_WRITE_SIZE * 10, MAX_WRITE_SIZE * 100

x_label 'file size'

format_size 1000000

run do |b, arg|
  num_writes = arg / MAX_WRITE_SIZE
  if num_writes > 0
    num_writes.times do
      b.echo('1', 'a' * MAX_WRITE_SIZE)
    end
  else
    b.echo('1', 'a' * arg)
  end
end
