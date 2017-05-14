SCRIPTS_DIR = scripts
BENCHMARKS = $(wildcard $(SCRIPTS_DIR)/benchmark_*.rb)
BENCHMARK_SCRIPT = $(SCRIPTS_DIR)/run_benchmark.rb

OUTPUT_DIR = output

GRAPHS_DIR = graphs
READS_GRAPHS = $(BENCHMARKS:$(SCRIPTS_DIR)/benchmark_%.rb=$(GRAPHS_DIR)/%_reads.png)

bench: $(READS_GRAPHS)

all:

clean:
	rm -f $(GRAPHS_DIR)/*


$(GRAPHS_DIR)/%_reads.png: $(SCRIPTS_DIR)/benchmark_%.rb
	@echo
	@echo --- $< ---
	@./$(BENCHMARK_SCRIPT) $<
