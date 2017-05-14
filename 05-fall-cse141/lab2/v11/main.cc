#include <iostream>
#include <fstream>
#include "proc.h"
using namespace std;

//
int main(int argc, char *argv[]) {
  Proc cpu;

  if (argc >= 3) {
    FILE *i_in = fopen(argv[1], "r");
    FILE *d_in = fopen(argv[2], "r");
    FILE *out = fopen(argv[3], "w");

    if (!i_in || !d_in || !out)
      fprintf(stderr, "FILES failed\n");

    cpu.loadIMem(i_in);
    cpu.loadDMem(d_in);

    cpu.reset();

  // --- FOR TESTING ---
    fclose(out);
    out = stderr;
  // --- FOR TESTING ---

    fprintf(out, "--- BEFORE ---\n");
    cpu.dumpDMem(out);

    cpu.runLoop();

    fprintf(out, "\n--- AFTER ---\n");
    cpu.dumpDMem(out);

    cpu.PrintInstCount(out);

    fclose(i_in);
    fclose(d_in);
    fclose(out);
  }

  return 0;
}
