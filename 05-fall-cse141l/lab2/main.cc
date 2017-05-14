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
###   -f #     - start the dumps at # instructions in
###   -s       - if set, then dump instructions
###   -S #     - if set, then dump instructions starting from #
###   -t       - if set, then dump data
###   -T #     - if set, then dump data starting from #
###   -u       - if set, then dump all registers
###   -U #     - if set, then dump register number #
###   -N       - run interactive mode with instr/data input and output optional
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
//#include <getopt.h>
#include "proc.h"
#include "main.h"
using namespace std;

int main(int argc, char* argv[]) {
   FILE *infile = stdin;         // instruction input file
   FILE *indfile = stdin;        // data input file
   FILE *outfile = stdout;       // output file
   char option;                  // temp char for receiving options
   Proc p;			            // Define the class
   bool options = false;         // true if using options, else default
   bool dumpI = false;           // Dump instructions
   int dumpIstart = 0;
   bool dumpD = false;           // Dump data
   int dumpDstart = 0;
   bool dumpR = false;           // Dump register(s)
   int dumpReg = -1;
   bool interactiveEnable = false;
   int end = 0;
   int advancement = -1;
   if ((argc == 2) && (strcmp("-help", argv[1])) ) {
      printHelp(argv);
      return 0;
   }


   while ((option = getopt(argc, argv, "a:ehNi:d:o:stuSTU")) != EOF) {
      options = true;
      switch(option) {
         // Interactive
         case 'N':
            interactiveEnable = true;
         break;
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
   if ((!options) || (interactiveEnable)) {
      if (argc < 3) {
         printf("Invalid input\n");
         printHelp(argv);
         return 0;
      }

      if (!interactiveEnable) {
         // Define the i-mem file
         infile = fopen (argv[1], "r");
         // Define the d-mem file in
         indfile = fopen (argv[2], "r");

         if (argc == 4) {
            // Define the d-mem file out
            outfile = fopen (argv[3], "w");
         }
      } else {
         // Define the i-mem file
         infile = fopen (argv[2], "r");
         // Define the d-mem file in
         indfile = fopen (argv[3], "r");

         if (argc == 5) {
            // Define the d-mem file out
            outfile = fopen (argv[4], "w");
         }
      } 
   }
   // If there is no explicit end for dumps, end at the N advancements
   if ((end == 0) || (end > advancement))
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

   // Run interactive mode
   if (interactiveEnable) {
      interactive(infile, indfile, outfile, argv, p);
      return 0;
   }

   // Load instruction and data mem into the ISS
   p.reset();

   p.loadIMem(infile);
   p.loadDMem(indfile);

   // OUTPUT THE BEFORE d-mem
   printHeaderInFile("D-Mem Dump BEFORE EXECUTION", outfile);
   p.dumpDMem(outfile);

   // Run the loop with the number of instructions (advancement)
   p.runLoop(advancement);


   //
   // Handle the output (DUMPING) of Instruction, Data, Registers
   //
   // Dump DMem to the output file if it is in default or stated
   if (dumpD || !options) {
      printHeaderInFile("D-Mem Dump AFTER EXECUTION", outfile);
      p.dumpDMem(outfile, dumpDstart, end);
   }

   // Dump IMem
   if (dumpI || !options) {
      printHeaderInFile("I-Mem Dump", outfile);
      p.dumpIMem(outfile, dumpIstart, end);
   }

   // Dump Registers, -1 for ALL (default) and # for which register
   if (dumpR || !options) {
      printHeaderInFile("R-Mem Dump", outfile);
      p.dumpRegister(outfile, dumpReg);
   } 

   printHeaderInFile("Instruction Count", outfile);
   p.PrintInstCount(outfile);

   // Output instruction count to user
   p.PrintInstCount();

   fclose(infile);
   fclose(indfile);
   fclose(outfile);

   return 0;
}

void printHeaderInFile(char * str, FILE * file) {
   fprintf(file, "\n########## %s ##########\n", str);
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
   printf( "\n\tinteractive mode:\n");
   printf( "\t%s -N <machine code> <data memory> [<output file>]\n", argv[0]);
}

void interactive(FILE * infile, 
                 FILE * indfile, 
                 FILE * outfile, 
                 char *argv[],
                 Proc p) {
   char input[80];
   int num;
   int num2;
   p.reset();
   char str[80];

   p.loadIMem(infile);
   p.loadDMem(indfile);
   
   
   printf( "> ");
   while (( fscanf(stdin, "%s", input) ) != EOF) {
      /*
       * Start comparing a lot of commands
       */
  
      // Advance N instructions
      if (!strcmp(input, "next")) {
         fscanf(stdin, "%d", &num);
         p.runLoop(num);
      } else if (!strcmp(input, "nextall")) {
         p.runLoop(-1);
      } else if (!strcmp(input, "dumpi")) {
         printHeaderInFile("I-Mem Dump All", outfile);

         p.dumpIMem(outfile);
      } else if (!strcmp(input, "dumpir")) {
         fscanf(stdin, "%d %d", &num, &num2);

         sprintf(str, "I-Mem Dump Range [%d-%d]", num, num2);
         printHeaderInFile(str, outfile);

         p.dumpIMem(outfile, num, num2);
      } else if (!strcmp(input, "dumpis")) {
         fscanf(stdin, "%d", &num);

         sprintf(str, "I-Mem Dump starting at %d", num);
         printHeaderInFile(str, outfile);

         p.dumpIMem(outfile, num);
      } else if (!strcmp(input, "dumpd")) {
         sprintf(str, "D-Mem Dump ALL");
         printHeaderInFile(str, outfile);

         p.dumpDMem(outfile);
      } else if (!strcmp(input, "dumpdr")) {
         fscanf(stdin, "%d %d", &num, &num2);

         sprintf(str, "D-Mem Dump Range [%d-%d]", num, num2);
         printHeaderInFile(str, outfile);

         p.dumpDMem(outfile,num, num2);
      } else if (!strcmp(input, "dumpds")) {
         fscanf(stdin, "%d", &num);

         sprintf(str, "D-Mem Dump starting at %d", num);
         printHeaderInFile(str, outfile);

         p.dumpDMem(outfile,num);
      } else if (!strcmp(input, "dumprall")) {
         printHeaderInFile("Register Dump", outfile);
         p.dumpRegister(outfile, -1);
      } else if (!strcmp(input, "dumpr")) {
         fscanf(stdin, "%d", &num);
         sprintf(str, "Register Dump for register %d", num);
         printHeaderInFile(str, outfile);
         p.dumpRegister(outfile, num);
      } else if (!strcmp(input, "reset")) {
         p.reset();
      } else if (!strcmp(input, "instcount")) {
         printHeaderInFile("Instruction Count", outfile);
         p.PrintInstCount(outfile);
      } else if (!strcmp(input, "exit")) {
         return;
      } else {
         printIHelp();
      }
      printf( "> ");
   }
}

void printIHelp() {
   printf( "Commands:\n" );
   printf( "\tnext #       - advance # steps\n");
   printf( "\tnextall      - advance to the end\n");
   printf( "\tdumpi        - dump instructions\n");
   printf( "\tdumpir # #   - dump instructions from # to #\n");
   printf( "\tdumpis #     - dump instructions from # to current\n");
   printf( "\tdumpd        - dump data memory\n");
   printf( "\tdumpdr # #   - dump data memory from # to #\n");
   printf( "\tdumpds #     - dump data memory from # to current\n");
   printf( "\tdumprall     - dump all of the registers\n");
   printf( "\tdumpr #      - dump register #\n");
   printf( "\treset        - reset the iss\n");
   printf( "\tinstcount    - print the dynamic instruction count\n");
   printf( "\texit         - exit the program\n");

}

