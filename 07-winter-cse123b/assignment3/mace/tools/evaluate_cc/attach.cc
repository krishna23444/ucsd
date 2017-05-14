//  #define  USE_NS_ROUTING_TABLE 0

//  #include "evaluate.cc"
//  #include "enumerate.cc"
#include <math.h>
#include <stdio.h>
//  int visited[600];
int total = 120;

char*network_file = "network";
char*nodes_file = "nodes";
char*clients_file = "clients";

int client_index = 600;
int main(int argc, char** argv)
{ 
  int index = 0;

  if (argc > 1)
    {
  	  network_file =argv[1]; 
    }
  if (argc > 2)
    {
      nodes_file =argv[2]; 
    }
  if (argc > 3)
    {
     clients_file =argv[3]; 
    }

#if 1
  FILE *myfile;
  FILE *clients;
  int  from;
  int i=0;

  myfile = (FILE *)fopen(nodes_file, "r");
  clients = (FILE *)fopen(clients_file, "w");
  i=0;
  int route = 0;
  while (fscanf(myfile, "%d", &from)!=EOF)
    {
  fprintf(clients," edge [\n"
"    lat %d\n    source %d\n    target %d\n"
"    parameter \"<private data>\"\n"
"    bw 10000000\n"
"    plr 0.00000\n"
"    type \"\"\n"
" ]\n",
0,from,client_index ++);
    }
  //caused a mesh
//             LEDA_add_overlay_link( route, from);
//    LEDA_remove_overlay_link( previous, from);
  fclose (clients);
  fclose (myfile);

#endif

}
