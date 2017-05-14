/*#############################################################
### main.h for lab2 of CSE141L
##############################################################*/

// Print the headers to the output file
void printHeaderInFile(char * str, FILE * file);

// Output the help
void printHelp(char**);

// Interactive
void interactive(FILE * infile, FILE * indfile, FILE * outfile, char**, Proc p);

// Print interactive help
void printIHelp();
