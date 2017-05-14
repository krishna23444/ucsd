require 'gruff'

g = Gruff::Line.new
g.data('a', [63, 218, 473, 828])
g.data('b', [107, 262, 517, 872])
g.data('c', [4455, 17116, 39742, 74084])
g.write

`eog graph.png`
