/**
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 2 - File System Evaluation
 */
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#include "benchmark.h"

static FILE *benchmark_file = NULL;

static uint32_t disk_size_in_blocks = 0;
static uint32_t last_address = 0;

static uint32_t read_count = 0;
static uint32_t write_count = 0;
static uint64_t seek_total = 0;

static void benchmark_block_access(unsigned int block_number) {
  unsigned int offset = abs(block_number - last_address);
  if (disk_size_in_blocks > 0)
    if (offset > disk_size_in_blocks / 2)
      offset = disk_size_in_blocks - offset;

  last_address = block_number + 1;
  if (disk_size_in_blocks > 0)
    last_address %= disk_size_in_blocks;

  seek_total += offset;
}

void benchmark_start(const char *output_file_path) {
  benchmark_file = fopen(output_file_path, "w");
}

void benchmark_set_disk_size_in_blocks(unsigned int size_in_blocks) {
  disk_size_in_blocks = size_in_blocks;
}

void benchmark_read(unsigned int block_number) {
  benchmark_block_access(block_number);
  read_count++;
}

void benchmark_write(unsigned int block_number) {
  benchmark_block_access(block_number);
  write_count++;
}

void benchmark_end() {
  if (benchmark_file == NULL)
    return;

  fprintf(benchmark_file, "%" PRIu64 "\n", seek_total);
  fprintf(benchmark_file, "%u\n", read_count);
  fprintf(benchmark_file, "%u\n", write_count);

  fclose(benchmark_file);
}
