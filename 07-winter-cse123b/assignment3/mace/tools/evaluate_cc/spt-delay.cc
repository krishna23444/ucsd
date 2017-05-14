#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1
#define  USE_MODELNET_DO_SP 1

#include "evaluate.cc"
//#include "enumerate.cc"
#include <math.h>
int visited[600];
int total = 120;

char*network_file = NULL;
char*nodes_file = "nodes";

int random_integer( int maximum)
{
  int retval;
  double value = drand48();
  value*= maximum;
  retval = (int) floor(value);
  if (retval == maximum)
    retval--;
  return retval;
}


int main(int argc, char** argv)
{ 

#if !USE_MODELNET
  int index = 0;
  for ( index = 0; index < 600 ;index++ )
    {
      visited [index]= 0;
    }

  if (argc > 1)
    {
      total =atoi(argv[1]);
      //  	  network_name =argv[1]; 
    }
  if (argc > 2)
    {
      network_file =argv[2]; 
    }
  if (argc > 3)
    {
      nodes_file =argv[3]; 
    }
     read_network( network_file);
     readnetwork = true;
  int previous;


#if 1
  FILE *myfile;
  int  from;
  int i=0;

  myfile = (FILE *)fopen(nodes_file, "r");
  i=0;
  int route = 0;
  while (fscanf(myfile, "%d", &from)!=EOF)
    {
      node s =get_node_with_number (from);
      i++;
      if (i==1)
	{
	  route = from;
	LEDA_init(from, 1);
	}
      else
	{
	LEDA_init(from);
//         LEDA_add_overlay_link(previous, from);
//         LEDA_add_overlay_link(previous, from);
	}
      LEDA_tree_status(from, 1);
      if (i>=total)
	{
	  break;
	}
      previous = from;
    }
  //caused a mesh
//             LEDA_add_overlay_link( route, from);
//    LEDA_remove_overlay_link( previous, from);
  fclose (myfile);
#else
 LEDA_init(0, 1);
  visited [0] = 1;
  int previous = 0;
  for (index = 0; index < total ;index++ )
    {
      int candidate = random_integer( 600);
      if (!  visited [candidate ])
	{
	  LEDA_init(candidate);
	}
      LEDA_add_overlay_link(previous, candidate);
      previous = candidate;
    }
#endif
#else
  read_modelnet_network( );
#endif
  edge e ;
  is_physical = edge_array<int>(G,0);
  forall(e, physical_edges) 
    { 
      is_physical [e] = 1;
    }
  double network_diameter =calculate_physical_network_diameter();

  prepare_MST();
  list < edge > result2 = DIJKSTRA1(G);
  add_reverse_edges(result2);
  restore_normal();
  prepare_list(result2);
  int degree = 0;
  node_map2<double> ddistance (G);
  edge_array<double > dcost (G, infinity);
  double cost = compute_cost(dcost, result2)/2;
  int connected;
  int satisfied = 0;
  list<double >  distances;
  double diameter = calculate_diameter(dcost,ddistance, connected, satisfied, distances);
  double average = 0;
      cout
	   << "DIJKSTRA cost "  << cost
	   << " max root delay "  << diameter/1000.0
	   << " network diameter "  << network_diameter/1000.0
	   << endl << flush;
//        for (int i = 0; i < 100 ;i++ )
//  	{
//  	cout << LEDA_get_join_target()<< endl << flush;
//  	}

  
  //    LEDA_add_overlay_link( 0,2);
  //    LEDA_add_overlay_link( 0, 3);
  //    LEDA_add_overlay_link( 1, 3);
//    LEDA_dump_stats(time(NULL));
//    LEDA_dump_tree(time(NULL),"test");
  //    LEDA_remove_overlay_link( 0,2);
  //    LEDA_remove_overlay_link( 0,3);
  //    LEDA_remove_overlay_link( 1,3);
  //    LEDA_dump_stats(time(NULL));
//      find_best();
}
