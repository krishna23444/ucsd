require 'gruff'

arg = ARGV.first
arg = 'pie'
size = 256

case arg
when 'pie'
  if size == 48
    a = 199040
    b = 3047160
    c = 879160
    d = 215200
  elsif size == 256
    a = 1039360
    b = 3047160
    c = 4689720
    d = 1055520
  end
  g = Gruff::Pie.new
  g.title = 'Breakdown of Computation Time'
  g.data('first crc', a)
  g.data('sbox setup', b)
  g.data('wep encryption', c)
  g.data('second crc', d)
  g.write
when 'bar'
  labels = {
    0 => '48',
    1 => '64',
    2 => '96',
    3 => '128',
    4 => '256',
  }

  a = [4348400, 4770800, 5615600,6460400, 9839600]
  b = [786600, 1003960, 1473800, 1943320, 3819880]

  g = Gruff::Bar.new
  g.data('sw', a)
  g.data('hw/sw', b)
  g.title = 'SW versus HW/SW implementation'
  g.x_axis_label = 'packet size'
  g.y_axis_label = 'MAC time'
  g.labels = labels
  g.write
end
`eog graph.png`
