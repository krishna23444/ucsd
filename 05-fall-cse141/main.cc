/*#################################################################
### main.cc for lab2 of CSE141L
### 
### This program takes many options:
###   -a #     - advance the program by # instructions
###   -h       - output help
###   -i []    - instruction input file
###   -d []    - initial d-mem file
###   -o []    - output file
###   -e #     - end dumps at # instructions
###   -s       - if set, then dump instructions
###   -S #     - if set, then dump instructions starting from #
###   -t       - if set, then dump data
###   -T #     - if set, then dump data starting from #
###   -u       - if set, then dump all registers
###   -U #     - if set, then dump register number #
###   -help    - output help
###
### DEFAULT:
###   [exec file] <machine code file> <data memory file> <output file>
#################################################################*/

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>
#include "proc.h"
#include "main.h"
using namespace std;

int main(int argc, char* argv[]) {
   FILE *infile;                 // instruction input file
   FILE *indfile;                // data input file
   FILE *outfile;                // output file
   char option;                  // temp char for receiving options
   // Proc p;			            // Define the class
   char * str;                   // Temp str to create headers in files
   bool options = false;         // true if using options, else default
   bool dumpI = false;           // Dump instructions
   int dumpIstart = 0;
   bool dumpD = false;           // Dump data
   int dumpDstart = 0;
   bool dumpR = false;           // Dump register(s)
   int dumpReg = -1;
   bool outputset = false;       // output file?
   int end = 0;
   int advancement = 256;
   if ((argc == 2) && (strcmp("-help", argv[1])) ) {
      printHelp(argv);
      return 0;
   }


   while ((option = getopt(argc, argv, "a:ehi:d:o:str:u:p")) != EOF) {
      options = true;
      switch(option) {
         // Advance N instructions
         case 'a':
            advancement = atoi(optarg);
         break;

         // End the dump at N instructions
         case 'e':
            end = atoi(optarg);
         break;
         case 'h':
            printHelp(argv);
            return 0;
         break;
         case 'i':
            infile = fopen(optarg, "r");
         break;
         case 'd':
            indfile = fopen(optarg, "r");
         break;
         case 'o':
            outfile = fopen(optarg, "w");
            outputset = true;
         break;

         // Handle the DUMPING options
         case 's':
            dumpI = true;
         break;
         case 'S':
            dumpI = true;
            dumpIstart = atoi(optarg);
         break; 
         case 't':
            dumpD = true;
         break; 
         case 'T':
            dumpD = true;
            dumpDstart = atoi(optarg);
         break;
         case 'u':
            dumpR = true;
         break;
         case 'U':
            dumpR = true;
            dumpReg = atoi(optarg);
         break;
      }
   }
   // Command line options
   // No options, so manually define as the argv1-3
   if (!options) {
      if (argc < 4) {
         printf("Invalid input\n");
         printHelp(argv);
         return 0;
      }
      // Define the i-mem file
      infile = fopen (argv[1], "r");
      // Define the d-mem file in
      indfile = fopen (argv[2], "r");
      // Define the d-mem file out
      outfile = fopen (argv[3], "w");

      outputset = true;
   }

   // If there is no explicit end for dumps, end at the N advancements
   if ((end == 0) && (end > advancement))
      end = advancement;

   if (indfile == NULL) {
      printf("Data input file does not exist\n"); 
      return 0;
   }

   if (infile == NULL) {
      printf("Instruction input file does not exist\n");
      return 0;
   }

   // If there are no options set, revert to default mode
   // Which means you need a valid output file
   if ((outfile == NULL) && (!options)) {
      printf("Data output file does not exist\n"); 
      return 0;
   }

/*
   // Load instruction and data mem into the ISS
   p.reset();

   p.loadIMem(infile);
   p.loadDMem(indfile);

   // Run the loop with the number of instructions (advancement)
   p.runLoop(advancement);


   //
   // Handle the output (DUMPING) of Instruction, Data, Registers
   //
   // Dump DMem to the output file if it is in default or stated
   if ((dumpD && outputset) || (!options)) {
      printHeaderInFile("D-Mem Dump", outfile);
      p.dumpDMem(outfile, start, end);
   } else if (dumpD) {
      printf("\n########## D-Mem Dump ##########\n", str);
      dumpDMem(NULL, start, end);
   }

   // Dump IMem
   if ((dumpI) && outputset) || (!options)) {
      printHeaderInFile("I-Mem Dump", outfile);
      p.dumpIMem(outfile, start, end);
   } else if (dumpD) {
      printf("\n########## I-Mem Dump ##########\n", str);
      p.dumpIMem(NULL, start, end);
   }

   // Dump Registers, -1 for ALL (default) and # for which register
   if ((dumpR) && outputset) || (!options)) {
      printHeaderInFile("R-Mem Dump", outfile);
      p.dumpReg(dumpReg);
   } else if (dumpR) {
      printf("\n########## R-Mem Dump ##########\n", str);
      p.dumpReg(dumpReg);
   }

   if (outputset) {
      printHeaderInFile("Instruction Count", outfile);
      p.PrintInstCount(outfile);
   }

   // Output instruction count to user
   p.PrintInstCount();
  
   

*/
   fclose(infile);
   fclose(indfile);
   fclose(outfile);

   return 0;
}

void printHeaderInFile(char * str, FILE * file) {
   //fprintf(file, "\n########## %s ##########\n", str);
}

void printHelp(char * argv[]) {
      printf( "\t%s: <machine code file> ", argv[0]);
      printf( "<data memory file> <output file>\n");
      printf( "\t-a #     - advance the program by N instructions\n");
      printf( "\t-h       - output help\n");
      printf( "\t-i []    - instruction input file\n");
      printf( "\t-d []    - initial d-mem file\n");
      printf( "\t-o []    - output file\n");
      printf( "\t-s       - if set, then dump instructions\n");
      printf( "\t-S []    - if set, then dump instructions\n");
      printf( "\t-t       - if set, then dump data\n");
      printf( "\t-T []    - if set, then dump data\n");
      printf( "\t-u       - if set, then dump all registers\n");
      printf( "\t-U #     - if set, then dump register #\n");
      printf( "\t-help    - output help\n");
}
