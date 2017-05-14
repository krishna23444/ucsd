/**
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 2 - File System Evaluation
 */
#ifndef BENCHMARK_H_
#define BENCHMARK_H_

void benchmark_start(const char *output_file_path);

void benchmark_set_disk_size_in_blocks(unsigned int size_in_blocks);

void benchmark_read(unsigned int block_number);
void benchmark_write(unsigned int block_number);

void benchmark_end();

#endif
