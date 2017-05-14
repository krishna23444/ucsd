module Benchmark
  class Base
    ROOT_DIRECTORY = File.expand_path(File.join(File.dirname(__FILE__), '..'))
    OUTPUT_DIRECTORY = File.expand_path(File.join(ROOT_DIRECTORY, 'output'))

    GRAPHS_DIRECTORY = File.join(ROOT_DIRECTORY, 'graphs')
    GRAPH_CONFIG_SUFFIX = 'graph.yaml'

    def graph_yaml_config_path_for(benchmark_name)
      File.join(OUTPUT_DIRECTORY, benchmark_name + '.' + GRAPH_CONFIG_SUFFIX)
    end
  end
end
