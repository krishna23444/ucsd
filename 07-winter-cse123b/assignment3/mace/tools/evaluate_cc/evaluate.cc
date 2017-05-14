//  #if 1
// Dejan Kostic

//  The invariant is that all physical links and nodes are visible,
//  and all overlay edges are hidden.

//  The current overlay topology includes overlay edges in only one 
//  direction, the code handles the reverse automatically when it
// calculates cost and diameter.

#define USE_OVERLAY_MAP  1 //this has to be one
#define IN_USE_MAP  1
#define USE_ALL   1
#define  USE_LIFT 1
#define  USE_NODE_NUMBER 1

#ifndef  USE_NS_ROUTING_TABLE 
#define  USE_NS_ROUTING_TABLE 1
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "evaluate.h"
#include <LEDA/graph_alg.h>
#include <LEDA/graphwin.h>
#include <LEDA/graph.h>
#include <LEDA/ugraph.h>
#include <LEDA/map.h>
#include <LEDA/set.h>
#include <LEDA/color.h> 
#include <LEDA/node_partition.h> 
#include <LEDA/gml_graph.h>
#include <LEDA/node_pq.h>
#include <LEDA/dictionary.h>
#include <LEDA/p_queue.h>

#define ADOLFO_HACK_20021228

#if USE_MODELNET
#include "gmlp.h"
#endif

#if USE_NS_ROUTING_TABLE
extern "C" {
#include <tcl.h>
}
#include "config.h"
#include "packet.h"
#include "ip.h"
#include "classifier.h"
#include "route.h"
#include "object.h"
#include "address.h"
#endif

list < edge > MIN_SPANNING_TREE1 (GRAPH<string,double>& G);
list < edge > DIJKSTRA1 (GRAPH<string,double>& G);
double maximum_over_spt=  0;
double average_over_spt=  0;
double maximum_over_cost=  0;
double average_over_cost=  0;
int times_delay_exceeded =  0;
int times_cost_exceeded =  0;
double SPT_ratio=  0;
double delay_bound = 0;
double target_MST_ratio=  0;
double desired_delay_bound_ratio =  0;
int changes_last_interval=  0;

GRAPH<string, double> G;
list < edge > overlay_edge_list;
node source_= NULL;

double MSTcost= 0.0;
double referenceMSTcost= 0.0;
double referencecost= 0.0;
double SPTdelay= 0.0;
double last_reported_cost= 0;
double experiment_started = 0.0;
double experiment_stopped = 0.0;
double measurements_made = 0;
double measurements_total = 0;

typedef bool Boolean;
const  int infinity = 999999 ;

bool readnetwork=false;

double   my_ns_tree_farthest = 0;
int use_ns_tree_farthest = 0;

int recompute_cost = 1;
int recompute_delay = 0;

node_map2<list < edge >* > modelnet_overlay_matrix(G, NULL);

node_matrix<edge> overlay_matrix;
edge_map<int> bandwidth(G);
edge_map<double> loss_rate(G);
/* edge_array<int> stress; */
list < node > participants;
edge_array<int> references;
edge_array<bool> is_up;
edge_map<double> original_delay(G);
edge_array<double> physical_cost;
edge_array<double> initial_physical_cost;
edge_map<int> edge_number(G);
int edge_counter= 0;
edge_map<int> emulators(G);
array<char*> emulator_mapping(10);

#if  USE_LIFT
#if  USE_NODE_NUMBER
node_array <int> node_number;
#endif 
#if USE_MODELNET
array<node> participants_array(15);//this one is dynamically resized
node_map<int> IP_addresses(G, 0);
node_map < double > modelnet_distance (G, infinity);

#endif
array<node> nodes_array(15);//this one is dynamically resized
array<edge> edges( 500000);//edges are counted starting at 1
//  edge edges[50000];//edges are counted starting at 1
//dmk:we have to preallocate because edges are assigned to this array as they're being read from the file
//node,we could do it on the fly, to resize the array when it reaches capacity
#endif
#if !(IN_USE_MAP)
node_matrix <bool> in_use;
#else
node_map2 <bool> in_use;
#endif

node_map <int> tree_status(G);
node_map <int> node_status(G);


#if USE_OVERLAY_MAP 
node_map2<edge> overlay_map(G);
#endif

//  edge_map <int> is_physical;
edge_array <int> is_physical;

list < edge > physical_edges;

node_array <double> parent_died;
node_array <double> reconnected;
int recoveries = 0;
double recovery_sum = 0;
list  <double> recovery_times;
double disconnected_sum = 0;

int skip_dijkstra=0;

char* network_name = "ts5k2-0.gml";
char* nodes_name = "ts5k2-0.nodes";
char* cost_name = "ts5k2-0.cost";
char* delay_name = "ts5k2-0.delay";
char* tree_name = "ts5k2-0.tree";
/* char* name = "ts600-0.gml"; */

void error (const char * Message) 
{ 
  cerr << "Error: " << Message << ".\n\n"; 
  abort();
}

double
my_clock()
{
  struct timezone tz_;
  timeval clock_;
  gettimeofday(&clock_, &tz_);
  return (clock_.tv_sec+(double)clock_.tv_usec/1000000.0);

}

int is_transit_node (node n)
{
  if (G[n])
    return G[n][0] =='T';
}  
int is_stub_node (node n)
{
  if (G[n])
    return G[n][0] =='S';
}  
edge get_edge_with_number (int r)
{
#if  USE_LIFT
  edge v=edges[r];

//    edge e,v;
//    forall_edges(e, G)
//      {
//        if (edge_number[e]==r)
//  	{
//  	  v=e;
//  	  break;
//  	}

//      }

#else

 edge v=G.first_edge();
  while (r--) v = G.succ_edge(v);
#endif
	return v;
}

node get_node_with_number (int r)
{
#if  USE_LIFT
//    cout << "get_node_with_number " << r << ".\n\n";
  node v=nodes_array[r];
#else
  node v=G.first_node();
  while (r--) v = G.succ_node(v);
#endif
	return v;
}
int get_node_number (node s)
{

#if  USE_LIFT
//    cout << "get_node_number " << s << ".\n\n";
  int value =-1;

#if  USE_NODE_NUMBER
  value = node_number[s];
#else
  for ( int index = 0; index <nodes_array.size() ;index++ )
    {
      if( nodes_array[index]==s)
	return index;
    }
#endif 
  if (value==-1)
    {
      printf ("cannot find  node %x\n",s);
      error("get_node_number:\n");
    }
  return  value;

#else
  int r=0;
  node v;
  forall_nodes(v,G) 
    {
      if (v==s)
	{
	  return r;
	}
      r++;
    }
#endif
}
//this should say get participant from name
node get_node_from_name(const char* name)
{
  node value= NULL;
#if USE_MODELNET
  value = participants_array[atoi(name+6)];
#else
 if (!strncmp (name,"client", 6))
    {
      value =participants.inf(participants.get_item(atoi(name+6)));
    }
#endif
  return value;
}
#if USE_MODELNET

node get_node_with_IP_address(int address)
{
  node value= NULL;
  node v;
  int counter = 0;
//    forall_nodes(v,G) 
  forall(v,participants) 
    {
      counter++;
      if (IP_addresses[v]==address)
	{
	  return v;
	}
    }
  forall(v,participants) 
    {
      cout <<"  address  "       << IP_addresses[v] << endl;
    }
  cout <<" couldn't find  addresses "       << address<< "  after " <<  counter<< endl;
  return value;
}
#endif
node get_physical_node_from_name(const char* name)
{
  node value= NULL;
  node v;
  forall_nodes(v,G) 
    {
      if (G[v]==name)
	{
	  return v;
	}
    }
  return value;
}

int random_integer2( int maximum)
{
  // returns a random int in the range {0, maximum-1}
  
  int retval;
  double value = drand48();
  value*= maximum;
  retval = (int) floor(value);
  if (retval == maximum)
    retval--;
  return retval;
}

node get_random_node_from_set (set < node >source)
{
  int n=source.size();	
  int r;
  int counter = 0;

  r =random_integer2(source.size());
  node v;
  forall(v,source) 
    {
      if (counter == r)
	{
	  return v;
	}
      counter++;
    }
}
//  	  random_source S(0,n-1);
edge get_random_edge()
{
  int n= physical_edges.size();
	  int r;
	  int counter = 0;

	  r=random_integer2(n);
	  edge e;

	    forall_edges(e,G) 
	      {
		if (counter == r)
		  {
		    return e;
		  }
		counter++;
	      }
}

edge reverse_edge(edge forward)
{
/*  return G.reverse(forward); */

  edge value = NULL;
  edge candidate = NULL;
  forall_in_edges(candidate, G.source (forward) )
    {
    if (G.source(candidate) == G.target(forward))
      {
		value = candidate;
		break; 
      }
    }
  if (value == NULL)
	{
  cerr << "Error: " << forward << ".\n\n"; 
	error (" reverse  _edge==NULL");
	}

  return value; 
}

#if USE_OVERLAY_MAP 

edge find_overlay_edge (node first_node, node second_node)
{
  return  overlay_map(first_node,second_node);
}
edge reverse_overlay_edge (edge forward)
{
  return  overlay_map( target (forward), source (forward));
}
#endif

edge find_edge (node first_node, node second_node)
{
	if ( G.is_hidden (first_node) || G.is_hidden (second_node))
	  {
  cout << "Error: hidden" << get_node_number(first_node) << " " << get_node_number(second_node)<< ".\n\n"; 
error (" find _edgehidden");
	  }

#if 1
  edge value = NULL;
  edge candidate = NULL;
  forall_in_edges(candidate, second_node )
    {
    if (source(candidate) == first_node)
      {
		value = candidate;
		break; 
      }
    }
  if (value == NULL)
	{
  cerr << "Error: " << get_node_number(first_node) << " " << get_node_number(second_node)<< ".\n\n"; 
	error (" find _edge==NULL");
	}

  return value; 

#else
  edge e;
  forall_edges(e,G)
    {
      if (source (e) == first_node && target (e) == second_node)
       {
	 	return e; 
       }
    } 
  cerr << "Error: " << get_node_number(first_node) << " " << get_node_number(second_node)<< ".\n\n"; 
	error (" find _edge==NULL");
   return NULL; 
#endif
}

#if USE_NS_ROUTING_TABLE

RouteLogic *routelogic_= NULL;
list < edge> ns_overlay_edges (node from, node to)
{
  if (!routelogic_) 
    {
    Tcl &tcl = Tcl::instance();
    tcl.evalc("[Simulator instance] get-routelogic");
    routelogic_= (RouteLogic*) TclObject::lookup(tcl.result());
    //tcl.evalf("%s info class", tcl.result());
  }
  list < edge> result;

  int first =get_node_number(from);
  int second =get_node_number(to);

  int next_hop;

  node temp = from;
//    cout<<" ns says "<<G[from]<<"(" <<get_node_number(from)+1
//   <<") -> "<< G[to]<<"(" <<get_node_number(to)+1 <<  ") == " << flush;
  while (temp!= to)
    {
      char* sources= Address::instance().print_nodeaddr
	(get_node_number(temp)+1);
      char* destination= Address::instance().print_nodeaddr
	(get_node_number(to)+1);

      routelogic_->lookup_flat(sources, destination, next_hop);
      delete [] sources;
      delete [] destination;
//        	  cout <<"looking for next hop"
//  	       <<" between: "  << get_node_number(temp)
//  	       <<" and "  << get_node_number(to)
//  	       <<" is "  << next_hop-1
//  	       <<endl;

      edge  e =  find_edge (temp,get_node_with_number(next_hop-1));
//        cout<<G[source (e)]<<"(" <<get_node_number(source (e))<<") -> (" <<" -> "<<
//  	G[e]<<") " <<G[target (e)]<<"(" <<get_node_number(target(e))<<"), "<<flush;
      result.  append (e); 
      temp = get_node_with_number(next_hop-1);
    }
  //   cout<< endl;

  return result;
}


//  list < edge>  overlay_edges (node from, node to)
//  {
//    return ns_overlay_edges (from,  to);
//  }

void dump_ns_next_hop()	

 {
  if (!routelogic_) 
    {
    Tcl &tcl = Tcl::instance();
    tcl.evalc("[Simulator instance] get-routelogic");
    routelogic_= (RouteLogic*) TclObject::lookup(tcl.result());
    //tcl.evalf("%s info class", tcl.result());
  }
  cout <<" dump_ns_routing_table" <<endl;

  node first_node, second_node;
  forall(first_node, participants )
    {
      forall(second_node, participants )
	{
      if (first_node == second_node)
	{
	  continue;
	}
	  int first =get_node_number(first_node)+1;
	  int second =get_node_number(second_node)+1  ;
	  int next_hop;
	  char* source= Address::instance().print_nodeaddr(first);
	  char* destination= Address::instance().print_nodeaddr(second);

	  routelogic_->lookup_flat(source, destination, next_hop);
	  delete [] source;
	  delete [] destination;

	  cout <<"ns next hop"
	       <<"between: "  << first
	       <<" and "  << second
	       <<" is "  << next_hop
	       <<endl;
	  list < edge> physical_edges =ns_overlay_edges(first_node, second_node);
	  edge outgoing = physical_edges.head();
	  cout <<"next hop"
	       <<"between: "  << first
	       <<" and "  << second
	       <<" is "  << get_node_number(target(outgoing))+1
	       <<endl;
	}
    }
  cout <<" done" <<endl;

}

#else
#endif

#if USE_MODELNET
list < edge>  overlay_edges (node from, node to)
{
  return *modelnet_overlay_matrix(from, to);
}

int overlay_hop_distance (node from, node to)
{
  int result=0;

  edge p;
  list < edge >* constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      result++;
    }
  return result;
}

double overlay_true_distance_single (node from, node to)
{
  double result=0;

  edge p;
  list < edge > *constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      result+= G[p];
    }

  return result;
}

double overlay_true_distance (node from, node to)
{
  double result=overlay_true_distance_single(from, to);

  double alternate =overlay_true_distance_single(to, from);
  result = (result + alternate)/2.0;

  return result;
}

double overlay_cost_distance (node from, node to)
{
  //dmk: todo
  double result=0;

  //  result =(IP_addresses[ from] + IP_addresses[ to])% 13 + 1;
  //  printf("cost from %x to %x is %lf\n", IP_addresses[from], IP_addresses[to], result);

  node temp;
  
//   if ((unsigned int) from < 
//       (unsigned int) to)
//     {
//       temp = from;
//       from = to;
//       to = temp;
//     }

  edge e;
  list < edge > constituents = overlay_edges(from, to);
  
  forall(e,constituents)
    {
      result+= physical_cost[e];
    }
  return result;
}
#else
list < edge>  overlay_edges (node from, node to)
{
  list < edge> result;
  node temp = to;
  /*        cout<<" before "<<G[from] <<" -> "<< G[to] <<  " == " << flush; */
  while (temp!= from)
    {
      edge e=overlay_matrix (from, temp);
      /*  	  cout<<G[source (e)]<<" -> "<<G[target (e)] <<", "<<flush; */
      result.  push (e); 
      temp = source(e);
    }
  /*       cout<< endl; */

  return result;
}


int overlay_hop_distance (node from, node to)
{
  int result=0;
  node temp = to;
  while (temp!= from)
    {
      edge e=overlay_matrix (from, temp);
      result++;
      temp = source(e);
    }
  return result;
}

double overlay_true_distance_single (node from, node to)
{
  double result=0;
  node temp = to;
  while (temp!= from)
    {
      edge e=overlay_matrix (from, temp);
      result+= G[e];
      temp = source(e);
    }
  return result;
}

double overlay_true_distance (node from, node to)
{
  double result=overlay_true_distance_single(from, to);

  double alternate =overlay_true_distance_single(to, from);
  result = (result + alternate)/2.0;

  return result;
}

double overlay_cost_distance (node from, node to)
{
  node temp;
  
  if ((unsigned int) from < 
      (unsigned int) to)
    {
      temp = from;
      from = to;
      to = temp;
    }
  
  
  double result=0;
  temp = to;
  while (temp!= from)
    {
      edge e=overlay_matrix (from, temp);
      result+= physical_cost[e];
      temp = source(e);
    }
  
  return result;
}
#endif
#if USE_NS_ROUTING_TABLE
int ns_overlay_hop_distance (node from, node to)
{
  int result=0;

  edge p;
  list < edge > constituents =ns_overlay_edges(from, to);

  forall(p,constituents)
    {
      result++;
    }
  return result;
}

double ns_overlay_true_distance_single (node from, node to)
{
  double result=0;

  edge p;
  list < edge > constituents =ns_overlay_edges(from, to);

  forall(p,constituents)
    {
      result+= G[p];
    }

  return result;
}

double ns_overlay_true_distance (node from, node to)
{
  double result=ns_overlay_true_distance_single(from, to);

  double alternate =ns_overlay_true_distance_single(to, from);
  result = (result + alternate)/2.0;

  return result;
}

double ns_overlay_cost_distance (node from, node to)
{
  node temp;
  if ((unsigned int) from < 
      (unsigned int) to)
    {
      temp = from;
      from = to;
      to = temp;
    }
  double result=0;

  edge p;
  list < edge > constituents =ns_overlay_edges(from, to);

  forall(p,constituents)
    {
      result+= physical_cost[p];
    }
  return result;
}
#endif
void assign_additional_edges (node first_node  )
{
  node  second_node;
  
  forall(second_node, participants)
    {
      if (first_node == second_node)
	{
	  continue;
	}
      edge new_edge =NULL;
      if (overlay_map.defined(first_node,second_node))
	{
	  continue;
	}
//        if (first_node==source_)

//        cout<<" assign "<<G[first_node]<<"(" <<get_node_number(first_node)+1
//  	  <<") -> "<< G[second_node]<<"(" <<get_node_number(second_node)+1 <<  ") == " << flush;

      new_edge = G.new_edge (first_node, second_node);
      double myDist = overlay_true_distance (first_node, second_node);
      G.assign (new_edge, myDist);
      
//        cout <<"assign: "  << new_edge <<" with distance: "  << myDist <<endl;
//        if (first_node==source_)

//        cout  <<new_edge <<" with distance: "  << myDist 
//  	    << flush <<endl;
#if USE_OVERLAY_MAP 
      overlay_map(first_node,second_node)= new_edge;
#endif
      /*  			originates_at[first_node].push(new_edge); */
//        is_physical[new_edge] = 0;
      G. hide_edge(new_edge);
      
      new_edge = G.new_edge ( second_node,first_node);
      
      G.assign (new_edge, myDist);
      
//        cout <<"assign: "  << new_edge <<" with distance: "  << myDist <<endl;
#if USE_OVERLAY_MAP 
      overlay_map(second_node,first_node)= new_edge;
#endif
      /*  			originates_at[first_node].push(new_edge); */
//        is_physical[new_edge] = 0;
      G. hide_edge(new_edge);
      
      
    }
  
}
#if USE_NS_ROUTING_TABLE

void install_ns_routing_table()	
 {

  cout <<" install_ns_routing_table" <<endl;

  node first_node, second_node;
  forall_nodes(first_node, G)
    {
      forall_nodes(second_node, G)
	{
	  if (first_node == second_node)
	    {
	      continue;
	    }

	  list < edge> physical_edges = ns_overlay_edges(first_node, second_node);

	  overlay_matrix (first_node, second_node) = physical_edges.tail();
//  	  int first =get_node_number(first_node);
//  	  int second =get_node_number(second_node)  ;

//  	  cout <<"ns previous hop"
//  	       <<"between: "  << first
//  	       <<" and "  << second
//  	       <<" is "  << get_node_number(source (overlay_matrix (first_node, second_node))) + 1
//  	       <<endl;
	}
    }
  cout <<" done" <<endl;
}
array<string> hostnames(120);

void dump_routing_table_as_pipes()	
 {
//     list < edge > all_edges= G. all_edges();
  cout <<"dump_routing_table_as_pipes" <<endl;

 FILE *myfile;
  int i=0;
  char name[80];
  char ip[80];
  char rest[80];
  myfile = (FILE *)fopen("hosts", "r");
  i=0;
  while (fscanf(myfile, "%s %s # on %s", &name, &ip, &rest)!=EOF)
    {
      if (!strncmp (name,"client", 6))
	{
//  	  int number =participants.inf(participants.get_item(atoi(name+6)));
	  int number =(atoi(name+6));
	  hostnames[number] = ip;
	  cout <<"client" << number<<" = " << ip <<endl;
	}

    }
  fclose(myfile);
  
  node first_node, second_node;
  forall_nodes(first_node, G)
    {
      forall_nodes(second_node, G)
	{
	  if (first_node == second_node)
	    {
	      continue;
	    }

	  if (participants.rank( first_node) && participants.rank (second_node))
	    {

//  	      cout <<"client"
//  		   <<participants.rank( first_node)-1
//  		   <<" client"
//  		   << participants.rank (second_node)- 1
//  		   <<" pipe " <<flush ;

	      cout <<"route add "
		   << hostnames[participants.rank( first_node)-1]
		   <<" "
		   << hostnames[participants.rank (second_node)- 1]
		   <<" pipe " <<flush ;


	  list < edge> physical_edges = ns_overlay_edges(first_node, second_node);	      double result=0;

	      edge p;

	      forall(p,physical_edges)
		{
		  cout<<edge_number[p]<<" "<<flush ;
		}
	    }
	  cout<<endl;
	}
    }
  cout <<" done" <<flush <<endl;
}


void dump_distance()	
 {

  cout <<"dump_distance" <<endl;

  node first_node, second_node;
  forall_nodes(first_node, G)
    {
      forall_nodes(second_node, G)
	{
	  if (first_node == second_node)
	    {
	      continue;
	    }

	  list < edge> physical_edges = ns_overlay_edges(first_node, second_node);
	  if (participants.rank( first_node) && participants.rank (second_node))
	    {
	      double result=0;

	      edge p;

	      forall(p,physical_edges)
		{
		  result+= G[p];
		}

	      cout <<"distance "
		   <<"between: "  << get_node_number(first_node) + 1
		   <<" and "  << get_node_number(second_node) + 1
		   <<" is "  << result
		   <<endl;
	    }
	}
    }
  cout <<" done" <<endl;
}
#endif

void dump_routing_table()	
 {
//     list < edge > all_edges= G. all_edges();
  cout <<"dump_routing_table" <<endl;

 FILE *myfile;
  int i=0;
  char name[80];
  char ip[80];
  char rest[80];
  myfile = (FILE *)fopen("table", "w");
  i=0;

 
  node first_node, second_node;
  forall_nodes(first_node, G)
    {
      forall_nodes(second_node, G)
	{
	  int number = 0;
	  if (first_node != second_node)
	    {
	      edge p=overlay_matrix (first_node,second_node);
	      number =edge_number[p];
	    }

	      fprintf(myfile," %d %d %d\n",
		      get_node_number(first_node),
		      get_node_number(second_node),
		      number);

	}
    }
  fclose(myfile);
 
  cout <<" done" <<flush <<endl;
}

void import_routing_table(char*routing_file)	
 {
  cout <<"import_routing_table" <<endl;

 FILE *myfile;
  int i=0;
  int number = 0;
  int from = 0;
  int to = 0;
  char name[80];
  char ip[80];
  char rest[80];
  myfile = (FILE *)fopen(routing_file, "r");
  i=0;
  node first_node, second_node;
  while (fscanf(myfile, "%d %d %d", &from, &to, &number)!=EOF)
    {
      first_node =get_node_with_number (from);
      second_node =get_node_with_number (to);
      
      if (first_node != second_node)
	{
	  edge p=get_edge_with_number (number);
	  overlay_matrix (first_node,second_node)= p;
//        	  cout <<"read previous hop"
//  	       <<" between: "  << from
//  	       <<" and "  << to
//  	       <<" is "  << number
//  	       <<endl;	  
//        	  cout <<"installed previous hop"
//  	       <<" between: "  << get_node_number(first_node)
//  	       <<" and "  << get_node_number(second_node)
//  	       <<" is "  << edge_number[p]
//  	       <<endl;
 	}
      else
	{
	  overlay_matrix (first_node,second_node)= NULL;
	}

    }

  fclose(myfile);
  cout <<" done" <<flush <<endl;

 }

int LEDA_get_simulator_ID(int number_)
{
get_node_number(participants.inf(participants.get_item(number_)));
}

void determine_constituents ()
{
cout <<"determine_constituents..." <<flush;
  edge_array<double > cost (G,0);
  node_array < double > distance (G);
  edge e;

  forall_edges(e,G) 
    {
      if (is_up [e])
	cost[e]=1;
//   	cost[e]=(double)G[e];
    }

  node s;
  forall_nodes(s,G)
	{
	node_array<edge>pred(G, nil);
	DIJKSTRA(G,s,cost,distance,pred);
	node v;
	forall_nodes(v,G)
	  {
	    overlay_matrix(s,v)=pred[v];
	  }
	overlay_matrix(s,s)=nil;
	}
cout <<" done" <<endl;
}
class pair_holder 
{
public:
  pair_holder (node a1,node b1)
  {
    a=a1;
    b=b1;
  }

  node a,b;
};
void check_shortest_paths()
{
  cout <<"check_shortest_paths..." <<flush;
  node_matrix<edge> new_overlay_matrix  (G);

  edge_array<double > cost (G,0);
  node_array < double > distance (G);
  edge e;

  forall_edges(e,G) 
    {
      if (is_up [e])
//	cost[e]=1;
 	cost[e]=G[e];
    }

  node s,v;
  forall_nodes(s,G)
	{
	node_array<edge>pred(G, nil);
	DIJKSTRA(G,s,cost,distance,pred);

	forall_nodes(v,G)
	  {
	    new_overlay_matrix(s,v)=pred[v];
	  }
	new_overlay_matrix(s,s)=nil;
	}
  cout <<" checking" <<endl;

  node first_node, second_node;
  forall_nodes(first_node, G )
    forall_nodes(second_node, G )
    {
      double result=0;
      node temp = second_node;
      while (temp!= first_node)
	{
	  edge e=new_overlay_matrix (first_node, temp);
	  result+= G[e];
	  temp = source(e);
	}
      if (result!=
	  overlay_true_distance_single(first_node, second_node))
	{
	  cout <<" problem"
	       <<"between: "  << get_node_number(first_node)
	       <<" and "  << get_node_number(second_node)  
	       <<" got "  << overlay_true_distance_single(first_node, second_node)
	       <<" instead "  << result
	       <<endl;
	}
      //        if (new_overlay_matrix(first_node, second_node)!=
      //  	  overlay_matrix(first_node, second_node))
      //  	{
      //  	  exit( 1);
      //  	}
    }
  cout <<" done" <<endl;
}

#if 0
void update_shortest_paths (edge e, double previous, double latest) 
{ 

cout <<" update_shortest_paths..." <<flush;
  node first_node, second_node;

  list < node > left, right; 
  list < pair_holder* > pairs; 

  node v1 = G.source (e);
  node v2 = G.target (e);
  
  if (previous > latest)
    {      
      G[e]=latest;
      forall_nodes(first_node, G )
	{    	 
	  if (overlay_true_distance (first_node,v2)> 
	      overlay_true_distance (first_node,v1) + G [e])
	    {
	      left.  append (first_node);
	    }
	}
      forall_nodes(first_node, G)
	{   
	  if (overlay_true_distance (v1,first_node)>  
	      overlay_true_distance (v2,first_node) + G [e])
	    {
	      right.  append (first_node);
	    }
	}
      forall(first_node, left )
	forall(second_node, right )
	{
	  if (overlay_true_distance (first_node, second_node)> 
	      overlay_true_distance (first_node,v1) + 
	      G [e]+
	      overlay_true_distance (v2,second_node))
	    {
	      if (second_node==v2)
		{
		  overlay_matrix(first_node,v2)=e;
		}
	      else
		{
		  overlay_matrix(first_node,second_node)= 
		    overlay_matrix(v2,second_node);
		}
	    }
	}

    }
  else if (previous < latest)
    {
      cout <<" choosing pairs..." <<flush;
      forall_nodes(first_node, G )
	forall_nodes(second_node, G )
	{

	  if (overlay_true_distance (first_node, second_node) ==
	      overlay_true_distance (first_node,v1) + 
	      previous +
	      overlay_true_distance (v2,second_node))
	    {
	      pairs.append (new pair_holder (first_node, second_node));
	    }
	}
      cout <<" going through list" <<flush;
      pair_holder* sample;

      G[e]=latest;

      forall_nodes(first_node, G )
	{
//  	  cout <<"first: "  << get_node_number(first_node) <<endl;
	  forall(sample, pairs )
	    {    	 
//  	      cout <<"got: "  << get_node_number(sample->a)
//  		   <<" "  << get_node_number(sample->b)  <<endl;
	      if (overlay_true_distance (sample->a,sample->b)> 
		  overlay_true_distance (sample->a,first_node) + 
		  overlay_true_distance (first_node,sample->b))
		{
		  overlay_matrix(sample->a,sample->b)= 
		    overlay_matrix(first_node,sample->b);
		}
	    }
	}

      forall(sample, pairs )
	{    	 
	  delete sample;
	}
    }
cout <<" done" <<endl;

}
#endif

double compute_cost ( edge_array<double >&dcost,
		    list < edge >& solution_edge_list)
{
  double cost = 0;
  edge e;

  forall(e,solution_edge_list)
    { 
/*        cout <<"got: "  << e  <<endl; */

//  #if USE_OVERLAY_MAP 
//        edge r = reverse_overlay_edge (e);
//  #else
//        edge r = reverse_edge (e);
//  #endif
      cost +=  overlay_cost_distance (source (e), target (e));
      dcost[e]= overlay_true_distance (source (e), target (e));
//        dcost[e]= G[e];
//         G[e]=dcost[e];//dmk!!!
//  dcost[r]= 
//  	overlay_true_distance (source (e), target (e));
    }  
 
  return cost;
}
double calculate_diameter (edge_array<double >&dcost,
			   node_map2<double>& ddistance,
			   int & connected,
			   int & satisfied,
			   list<double >&  distances,
			   int use_modelnet_distance = 0
)
{
  double maximum = 0,rmaximum = 0,total = 0, count = 0,
    rtotal = 0, rcount = 0; 
  double rdp = 0,rrdp = 0;
  connected = 0;
  satisfied = 0;

  node v;

//    ALL_PAIRS_SHORTEST_PATHS (G, dcost, ddistance);
  node_array<edge> pred(G, nil);
 node_array < double > distance (G, infinity);
 if (!use_modelnet_distance)
   {
     cout <<"computing shortest paths in the tree: "  <<endl;
     DIJKSTRA (G,source_, dcost, distance, pred);
   }

  node first_node, second_node;
  first_node = source_;
  double c=0;
  node worst;
  forall(second_node,participants)
    {
      if (node_status[second_node])
	{
	  //if it's told to acting dead
	  continue;
	}
      if (use_modelnet_distance)
	{
#if USE_MODELNET
	  distances  .append(modelnet_distance[second_node]);
	  c = modelnet_distance[second_node];
#else
	  distances  .append(distance[second_node]);
	  ddistance(first_node, second_node)=distance[second_node];
#endif

	}
      else
	{
	  distances  .append(distance[second_node]);
	  c = ddistance(first_node, second_node)=distance[second_node]; 
	}


      if (first_node == second_node)
	{
	  continue;
	}
//        double c=ddistance(first_node, second_node);
      if (c>rmaximum)
	{
	  worst = second_node;
	  rmaximum=c;
	}
      rtotal+=c;
      rcount++;
      
      rrdp += (double)c/((double)overlay_true_distance (first_node, second_node));
      if (c==infinity)
	{
	  //	  printf("\n unreachable node = %x ;\n", 
	  //		 get_node_number(second_node)+1);
	}
      else
	{
	  connected++;
	  //	  if (c <(SPTdelay*SPT_ratio))
	  if (c <= delay_bound)
	    {
	      satisfied ++;
	    }
	}
    }
  // account for root
  connected++;
  satisfied ++;

//    rardp= rrdp / rcount;
//    rdiam = rmaximum;
#if !USE_MODELNET
  double result=0;
  node temp = worst;
  if (rmaximum!=infinity &&rmaximum!=0)
    while (temp!=  source_)
      {
	edge e=pred[ temp];
	node previous =source(e);

	printf("\nworst path node{%x}.child = %x  with delay  = %lf, latency  = %lf hops = %d;\n", 
	       get_node_number(previous)+1,
	       get_node_number(temp)+1,
	       dcost[e],
	       distance[ temp]
	       ,
	       overlay_hop_distance (first_node, second_node));
 
	temp =  previous;
      }
#endif
return rmaximum; 
}


double calculate_stress (list<edge>&physical_edges,
		       edge_array<int>&is_physical,
			  double & average)
{
  edge_array<int>stress(G,0);
  edge e,o;
  double cumulative=0.0;
  int count=0;

  node s;
  forall_nodes(s,G)
	{
	node v;
	forall_nodes(v,G)
	  {
	    edge c;
#if IN_USE_MAP 
	    if (!in_use.defined (s,v) || !in_use (s,v))
#else
	    if (!in_use (s,v))
#endif
	      {
		continue;
	      }
/*        cout <<" examining "  << in_use (s,v) <<  flush; */

      edge p;
      list < edge > constituents =overlay_edges(s,v);

      forall(p,constituents)
      {
		stress[p]++;
/*  		cout <<p<<", "  << flush; */
      }
/*  	    cout << endl;  */
/*  	    count++; */
    }
	}
 int maximum = 0;
  forall(e,physical_edges)
    {
      if (stress[e])
		{
		  count+=1;
//    		   cout << " stress "  
//    			<<G[source (e)]<<" -> "<<G[target (e)]  
//    			<< " " << e << " " << stress[e]<< endl;   
		}
      if (stress[e]>maximum)
	{
	  maximum=stress[e];
	}
    cumulative+=stress[e];
    } 
  double average_stress;
  if (count>0)
    average_stress=cumulative/count;
  else
    average_stress=0;
//      cout << "average stress:"  <<   average_stress << endl;
  average =average_stress;
    return maximum; 
} 
double calculate_physical_network_diameter ()
{
  double maximum = 0; 

  node first_node, second_node;
  forall(first_node,participants)
    {
  forall(second_node,participants)
    {
      if (first_node == second_node)
	{
	  continue;
	}

      double c=overlay_true_distance (first_node, second_node);

      if (c>maximum)
	{
	  maximum=c;
	}
//        rtotal+=c;
//        rcount++;
	    
//        rrdp += (double)c/((double)overlay_true_distance (first_node, second_node));
    }
    }
//    rardp= rrdp / rcount;
//    rdiam = rmaximum;

return maximum; 
}

void DIJKSTRA_DVMRP( GRAPH<string,double>&  G, node s, edge_array<double>& cost, 
               node_array<double>& dist, node_array<edge>& pred )
{ 
  node_pq<double> PQ(G);
  node_array<int> hops(G, 0);

  double c;
  node u,v;
  edge e;


  forall_nodes(v,G) {
    pred[v] = 0;
    hops [v] = 0;
    dist[v] = infinity;
    PQ.insert(v,dist[v]);
  }

  dist[s] = 0;
  PQ.decrease_inf(s,0);
#if 1
  while ( ! PQ.empty()) {
    u = PQ.del_min();

    forall_adj_edges(e,u) {
      v = G.target(e) ; 
      c = dist[u] + cost[e] ;
      if ( c < dist[v] ) {
        dist[v] = c;
        pred[v] = e;
        PQ.decrease_inf(v,c);
	hops [v] = hops [u] +1; 
      }
    } // forall_adj_edges 
  } // while 
#endif   

#if 1
	bool  improvement = false; 

  do
    {
      improvement = false; 
      forall_edges(e,G) 
		{ 
		  u = G. source (e) ; 
		  v = G.target(e) ; 
		  c = dist[u] + cost[e] ;
		  if (( c <= dist[v]
//  			) || (c == dist[v] 
			&& hops [v] < hops [u] +1) ) 
		    {
		      improvement = true; 
		      dist[v] = c;
		      pred[v] = e;
		      hops [v] = hops [u] +1; 
		    }	  
		}
    }
  while (improvement);
#endif   
}

list < edge > DIJKSTRA1 (GRAPH<string,double>& G)
{ 
  list < edge >  solution_edge_list;
  node s = source_;

  //  if (s == nil) error("empty graph");
  if (s == nil) return solution_edge_list; //empty graph

  node_array<edge> pred(G, nil);

  edge_array < double > cost(G);
  node_array < double > distance (G);
  edge e ;
  forall_edges(e,G) 
  { 
//      cost[e]=G[e];
    cost[e]=overlay_true_distance (source (e), target (e));
  }
#ifndef NICE_VALIDATION   
  DIJKSTRA_DVMRP (G,s, cost, distance, pred);
#else
  DIJKSTRA (G,s, cost, distance, pred);
#endif 
  node v;
  forall_nodes(v,G) 
    { 
      if (v == s ) continue;

e = pred[v];
    if (e != nil ) {
      solution_edge_list.append (e);
    }
   }
  return solution_edge_list;
}

list < edge > DIJKSTRA_COST1 (GRAPH<string,double>& G)
{ 
  list < edge >  solution_edge_list;
  node s = source_;

  //  if (s == nil) error("empty graph");
  if (s == nil) return solution_edge_list; //empty graph

  node_array<edge> pred(G, nil);

  edge_array < double > cost(G);
  node_array < double > distance (G);
  edge e ;
  forall_edges(e,G) 
  { 
    cost[e]=overlay_cost_distance (source (e), target (e));
  }
    
//    DIJKSTRA_DVMRP (G,s, cost, distance, pred);
  DIJKSTRA (G,s, cost, distance, pred);
 
  node v;
  forall_nodes(v,G) 
    { 
      if (v == s ) continue;
e = pred[v];
    if (e != nil ) {
      solution_edge_list.append (e);
    }
   }
  return solution_edge_list;
}
  
void swap_parent (node latest, node old, node v,
		  node_array<node>& overcast_parent,
		  node_array<list < node >  >& overcast_children)
{
      if (old!=nil)
		{
		  overcast_children[old].remove(v);
		  edge p;
		  list < edge > constituents =overlay_edges(v, old);

		  forall(p,constituents)
		    {
		      references [p]--;
		    }
 		}
  overcast_children[latest].append(v);
  overcast_parent[v]=latest;
  edge p;
  list < edge > constituents =overlay_edges(v, latest);

  forall(p,constituents)
    {
      references [p]++;
    }

}
int better_or_equal (int a, int b)
{
  if (b*0.9 <= a )
	{
	  return 1;
	}
  else
	{
	  return 0;
	}

}
int measure_bandwidth_physical (node from, node to)
{
 int lowest_bandwidth= infinity;
     edge p;
      list < edge > constituents =overlay_edges(from, to);

      forall(p,constituents)
      {
	if (lowest_bandwidth > bandwidth[p]/(references[p]))
	{
	  lowest_bandwidth=bandwidth[p]/(references[p]);
	}
/*  	if (lowest_bandwidth > bandwidth[p]/(references[p]?references[p]: 1)) */
/* 	{ */
/* 	  lowest_bandwidth=bandwidth[p]/(references[p]?references[p]: 1); */
/* 	} */
    }
  return lowest_bandwidth;
}
int measure_bandwidth (node from, node via, node to,
		       node_array<node>& overcast_parent
)
{
  int lowest_bandwidth = measure_bandwidth_physical(from,via);; 
  if (via!=to)
    {
      int new_bandwidth =measure_bandwidth_physical(via,to);
      if (lowest_bandwidth > new_bandwidth)
		{
		  lowest_bandwidth=new_bandwidth;
		}
    }
/*   node temp = via; */
/*   while (overcast_parent[temp]) */
/*     { */
/*       int new_bandwidth =measure_bandwidth_physical(temp,overcast_parent[temp]); */
/*       if (lowest_bandwidth > new_bandwidth) */
/* 		{ */
/* 		  lowest_bandwidth=new_bandwidth; */
/* 		} */
/*       temp=overcast_parent[temp]; */
/*     } */

  return lowest_bandwidth;
}

void adjust_node (node v,
		 node_array<node>& overcast_parent,
		 node_array<list < node >  >& overcast_children,
		 bool&  improvement
		 )
{
  int bandwidth_through_parent; 
  int bandwidth_through_sibling; 
  int bandwidth_through_grandparent; 

  bandwidth_through_parent= measure_bandwidth
    (v,overcast_parent[v],overcast_parent[v],
     overcast_parent);  
  node gp=overcast_parent [overcast_parent[v]];
  node parent=overcast_parent [v];
#if 1
  if (gp)
    {
      swap_parent (gp,parent, v,
		   overcast_parent,
		   overcast_children);
      bandwidth_through_grandparent= measure_bandwidth
	(v,gp, gp, overcast_parent);

      if (!better_or_equal ( bandwidth_through_parent,bandwidth_through_grandparent ))
	{
/* 	  cout<<G[v]<<" moving under gp "<<G[gp] << */
/* 	    " instead of "<<G[parent]<<"bandwidth is" */
/* 	      <<bandwidth_through_grandparent */
/* 	      <<" instead of "<<  bandwidth_through_parent<<endl; */
	  improvement = true;
	  return;
/* 	  parent=gp; */
	}
      else
	{
	  swap_parent (parent, gp, v,
		       overcast_parent,
		       overcast_children);
	}
    }
#endif
  node c; 
  node best_sibling=nil;
  edge edge_to_best =nil;
  int best_bandwidth; 

  list < node > siblings = overcast_children[overcast_parent[v]];
  siblings.remove(v);

  forall(c,siblings) 
    { 
      if (v==c) continue;

      swap_parent (c, parent, v,
		   overcast_parent,
		   overcast_children);
      bandwidth_through_sibling = measure_bandwidth
/* 	(v,c,parent,overcast_parent); */
	(v,c,c,overcast_parent);
			
      if (better_or_equal (bandwidth_through_sibling,
			   bandwidth_through_parent))
	{
	  if (!best_sibling)
	    {
	      best_sibling =c;
	      best_bandwidth= bandwidth_through_sibling ;
	    }
	  else
	    {
	      if (better_or_equal (bandwidth_through_sibling,
				   best_bandwidth))
		{
		  if ( overlay_true_distance(v,c) <
		       overlay_true_distance(v,best_sibling))
		    {
		      best_sibling =c;
		      best_bandwidth= bandwidth_through_sibling ;
		    }
		}
	    }
	}
      swap_parent (parent, c, v,
		   overcast_parent,
		   overcast_children);
    }
  if (best_sibling)
    {
/*       cout<<G[v]<<" moving under "<<G[best_sibling] << */
/* 	" instead of "<<G[parent]<<"bandwidth is"<<best_bandwidth */
/* 	  <<" instead of "<<  bandwidth_through_parent<<endl; */
      swap_parent (best_sibling, parent, v,
		   overcast_parent,
		   overcast_children);
      improvement = true;
      return;
    }
}


list < edge >  OVERCAST (GRAPH<string,double>& G)
{ 
  node s = source_;

  node_array<node> overcast_parent(G,nil);
  overcast_parent[s]=nil;

  node_array<list < node >  > overcast_children(G);

  list < edge >  solution_edge_list;
  list < node >  transit_list;
  list < node >*  processing_list;
  list < node >  all_node_list;
  edge_array < int > cost(G);

  bool  improvement = false; 
  int margin= 10;

  node v;
  forall_nodes(v,G) 
    {
    if (is_transit_node (v))
      transit_list.append (v);
    else
      all_node_list.append (v);
    }
  processing_list = &transit_list;
  forall(v,*processing_list) 
    {
      if (v==s) continue;
      if (!overcast_parent[v])
	{
	  swap_parent (s, overcast_parent[v], v,
		       overcast_parent,
		       overcast_children);
	}
      do
	{
	  improvement = false; 
	    { 
	      if (v==s) continue;
	      adjust_node(v,
			  overcast_parent,
			  overcast_children,
			  improvement);
	    }
	}
      while (improvement);
    }
  processing_list = &all_node_list;

  for ( int i = 0; i < 2;i++ )
/*   for ( int i = 0; i < G.number_of_nodes ()*10000 ;i++ ) */
  {
/*   forall_nodes(v,G)  */
  forall(v,*processing_list) 
    {
/*       v=G.choose_node (); */
      if (v==s) continue;
/*       if (i == 0 || (i == 1 &&!is_transit_node (v))) */
      if (!overcast_parent[v])
	{
	  swap_parent (s, overcast_parent[v], v,
		       overcast_parent,
		       overcast_children);
	}
/*      } */
/*       do */
	{
	  improvement = false; 
/*        forall_nodes(v,G)  */
	    { 
	      if (v==s) continue;
	      adjust_node(v,
			  overcast_parent,
			  overcast_children,
			  improvement);
	    }
	}
/*       while (improvement); */
    }
    
/*   if (i == 0) */
/*     processing_list = &all_node_list; */
  }
  forall_nodes(v,G) 
    { 
      if (v==s)
	{
	  continue;
	}
#if USE_OVERLAY_MAP       
      edge e =  find_overlay_edge (v,overcast_parent[v]);
#else
      edge e =  find_edge (v,overcast_parent[v]);
#endif
      solution_edge_list.append(e);
//        edge r = reverse_edge(e);
//        solution_edge_list.append(r);
      }
  return solution_edge_list;
}

list < edge >   IP_UNICAST(GRAPH<string,double>& G)
{
  node s =source_;
  node v;
  list < edge >  solution_edge_list;

#if 0
   forall_nodes (v,G) 
    { 
      if (v==s) continue;
#if USE_OVERLAY_MAP 
      edge  e =  find_overlay_edge (s, v);
#else
      edge  e =  find_edge (s, v);
#endif
      solution_edge_list.append (e);
    }
#else
   edge e;
   forall_out_edges(e,s) 
		{ 
		  node u = G. source (e) ; 
		  node v = G.target(e) ; 
      if (u!=s) continue;
       solution_edge_list.append (e);
}     
#endif
/*         int total=0, count=0; */
/*     forall(e,solution_edge_list1)  */
/*       total+=G[e]; */
/*     forall(e,solution_edge_list)  */
/*       count +=G[e]; */
/*        cout <<  total */
/*  	   << " " <<count  */
/*  	   << " " << "is equal " <<endl; */
 return solution_edge_list;
}


list < edge >  MIN_SPANNING_TREE1 (GRAPH<string, double>& G)
{ 
  list < edge >  solution_edge_list;
  edge_array < double > cost(G);

  edge e ;
  forall_edges(e,G) 
  { 
//      cost[e]=G[e];
    cost[e]=overlay_true_distance (source (e), target (e));
  }
 solution_edge_list = MIN_SPANNING_TREE(G, cost);

 return solution_edge_list;
}


list < edge >  MIN_SPANNING_TREE_COST1 (GRAPH<string, double>& G)
{ 
  list < edge >  solution_edge_list;
  edge_array < double > cost(G);

  edge e ;
  forall_edges(e,G) 
  { 
    cost[e]=overlay_cost_distance (source (e), target (e));
  }
 solution_edge_list = MIN_SPANNING_TREE(G, cost);

 return solution_edge_list;
}


void assign_edge_weights (int type = 0)
{
//    edge e ;
//    forall_edges(e,G) 
//    { 
//      G[e] = G[e] * 10.0;
//    }

}

random_source inter_bw_noise (700,1500);
random_source transit_bw_noise (15000,45000);
random_source intra_bw_noise (70000,100000);

void assign_edge_bandwidth (int type = 0)
{
intra_bw_noise.set_seed (1);
inter_bw_noise.set_seed (1);

 edge e;
  forall_edges (e, G) 
    {
      if ((unsigned int) G.source(e) < 
	      (unsigned int) G.target(e))
      {
	continue;
      }

  node n = source (e);
/*    cout << G[n] << " " << is_transit_node (n)  */
/*         << " " << is_stub_node (n)<< endl; */
/*    n = target (e); */
/*    cout << G[n] << " " << is_transit_node (n)  */
/*         << " " << is_stub_node (n)<< endl; */

  if (is_transit_node (source (e)))
	{
	  if (is_transit_node (target (e)))
	    {
	      if (type ==0)
	    bandwidth[e]=45000;
	      if (type ==1)
	    bandwidth[e]=transit_bw_noise();
	      if (type ==2)
	    bandwidth[e]=155000;
	    }
	  else
	    {
	      if (type ==0)
	      bandwidth[e]=1500;
	      if (type ==1)
	      bandwidth[e]=inter_bw_noise();
	      if (type ==2)
	      bandwidth[e]=45000;
	    }	    
	}
  else
	{
	  if (is_transit_node (target (e)))
	    {
	      if (type ==0)
	      bandwidth[e]=1500;
	      if (type ==1)
	      bandwidth[e]=inter_bw_noise();
	      if (type ==2)
	      bandwidth[e]=45000;
	    }
	  else
	    {
	      if (type ==0)
	    bandwidth[e]= 100000 ;
	      if (type ==1)
	    bandwidth[e]= intra_bw_noise() ;
	      if (type ==2)
	      bandwidth[e]=10000;
	    }
	}
  edge r = reverse_edge(e);
  G[r]= G[e];
  bandwidth[r]=bandwidth[e];

    }

}


random_source transit_cost_noise (20, 40);
random_source inter_cost_noise (10, 20);
random_source intra_cost_noise (1,5);
random_source uniform_cost (1,1000);

void assign_single_edge_cost (edge e)
{
//    physical_cost[e]=uniform_cost();

  if (is_transit_node (source (e)))
	{
	  if (is_transit_node (target (e)))
	    {
	    physical_cost[e]=transit_cost_noise();
	    }
	  else
	    {
	      physical_cost[e]=inter_cost_noise();
	    }	    
	}
  else
	{
	  if (is_transit_node (target (e)))
	    {
	      physical_cost[e]=inter_cost_noise();
	    }
	  else
	    {
	    physical_cost[e]= intra_cost_noise() ;
	    }
	}
  edge r = reverse_edge(e);
//    G[r]= G[e];
  physical_cost[r]=physical_cost[e];
  initial_physical_cost[e]=physical_cost[e];
  initial_physical_cost[r]=physical_cost[e];

}

void assign_edge_cost ()
{
intra_cost_noise.set_seed (1);
transit_cost_noise.set_seed (1);
inter_cost_noise.set_seed (1);

 edge e;
  forall_edges (e, G) 
    {
      if ((unsigned int) G.source(e) < 
	      (unsigned int) G.target(e))
      {
	continue;
      }

      assign_single_edge_cost(e);
    }
}

bool get_edge_weight(const gml_object* gobj, graph* wG, edge e) {
  GRAPH<string, double> *gPtr = (GRAPH<string, double> *)wG;

  double edgeWeight = (double)gobj->get_int();
  gPtr->assign(e, edgeWeight);
  cout << "read link " << edgeWeight << " counter"<<edge_counter<<flush<<endl;
#if  USE_LIFT && !USE_MODELNET
  edge_number[e]= ++edge_counter;
  edges[edge_counter]=e;
#endif
  return true;
}

bool get_edge_pipe(const gml_object* gobj, graph* wG, edge e) {
  GRAPH<string, double> *gPtr = (GRAPH<string, double> *)wG;

  int edge_pipe = gobj->get_int();

#if USE_MODELNET
  cout << "read pipe " << edge_pipe <<flush<<endl;
  edge_number[e]= edge_pipe;
  edges[edge_pipe]=e;
#endif
  return true;
}

bool get_node_label(const gml_object* gobj, 
		    graph *wG, node n) 
{
  char *node_desc = (char *)gobj->get_string();
  GRAPH<string, double> *gPtr = (GRAPH<string, double> *)wG;
  
  if(!node_desc)
    return false;
  else {
#if USE_MODELNET
    char modelnet_node_desc[80];
    if (strlen(node_desc )> 79)
      {
	error("modelnet node description too long");
      }
    
    if (!strncmp (node_desc,"client", 6))
      {
	cout << "read participant" << node_desc <<endl;
	//  	  participants.append(n);
	//trying to skip the space here
	strcpy(modelnet_node_desc,"client");
	strcat(modelnet_node_desc,&node_desc [7]);
      }
    else
      {
	strcpy(modelnet_node_desc,node_desc);
      }

    gPtr->assign(n, modelnet_node_desc);

#else
    gPtr->assign(n, node_desc);
#endif
  }
  return true;
}



int dump_network1( char* name)
{
 
 edge e;
  forall_edges (e, G) 
    {
      if ((unsigned int) G.source(e) >
	      (unsigned int) G.target(e))
      {
	continue;
      }

      cout << "$ns duplex-link $n("
	   << get_node_number(source (e))
	   << ") $n("
	   << get_node_number(target (e) )
	   << ") $linkBW";
      if (is_transit_node (source (e)))
	{
	  if (is_transit_node (target (e)))
	    {
	      cout << "TT ";
	    }
	  else
	    {
	      cout << "TS ";
	    }
	}
  else
	{
	  if (is_transit_node (target (e)))
	    {
	      cout << "TS ";
	    }
	  else
	    {
	      cout << " ";
	    }
	}
cout << G[e]
<< "ms DropTail"
 << endl;

    }
 
}

int dump_network2( char* name)
{
 
 edge e;
  forall_edges (e, G) 
    {
      if ((unsigned int) G.source(e) > 
	      (unsigned int) G.target(e))
      {
	continue;
      }

      cout << "set links($linknum) "
	   << get_node_number(source (e))
	   << "; set links2($linknum) "
	   << get_node_number(target (e) )
	   << "; set links_delay($linknum) "
	   << G[e]
	   << ";  set linknum [expr $linknum+1];"
	   << endl;
    }
 
}


int dump_network3( char* name)
{
 
 edge e;
  forall_edges (e, G) 
    {
      if ((unsigned int) G.source(e) ==
	      (unsigned int) G.target(e))
      {
	continue;
      }
printf("pipe config %4d bw %dkbit/s delay %lfms plr 0.00000 queue 50slots\n",
       edge_number[e], bandwidth[e],G[e]);
    }
 
}

int read_network( char* name)
{
  // Parse the label as the weight
  gml_graph parser(G);
  parser.add_edge_rule(get_edge_weight, gml_int, "label");

  parser.add_node_rule(get_node_label, gml_string, "label");
  
  cout << "LEDA: preparing to parse ... " << flush;
  bool parsed_ok = parser.parse(name);
  cout << "LEDA: parsing done.  nodes:" << G. number_of_nodes()
       << " edges "<<  G. number_of_edges() << endl << flush;;

  if(!parsed_ok)
    error("Failed to parse config file properly");
#if USE_LIFT
#if  USE_NODE_NUMBER
 node_number = node_array<int>(G,-1);
#endif
  nodes_array.resize( G. number_of_nodes()+ 1);
  edges.resize( G. number_of_edges()+ 1);
  int i=0;
  node v;
 forall_nodes(v,G)
   {
     cout << "read node = "  << v << " i = " << i << endl << flush;
   nodes_array[i]=v;
#if  USE_NODE_NUMBER
   node_number[v]=i;
#endif
   i++;
   }
//   node_status = node_map<int>(G,0);
//   tree_status = node_map<int>(G,0);
#endif
// return 0;
//   dump_network1(NULL);
//   dump_network2(NULL);
//    bandwidth = edge_array<int> (G);
  is_up = edge_array<bool> (G, true);
  references = edge_array<int> (G, 0);
  physical_cost = edge_array <double> (G,0);
  initial_physical_cost = edge_array <double> (G,0);
  parent_died = node_array <double> (G, 0);
  reconnected = node_array <double> (G, 0);
  
  assign_edge_weights();

  assign_edge_cost();
  assign_edge_bandwidth (2);

  edge e;

  forall_edges(e,G) 
  { 
    physical_edges.append (e);
  }

/*    overlay_edges = node_matrix<list<edge> >(G); */

  overlay_matrix  = node_matrix < edge > (G,nil);

#if USE_NS_ROUTING_TABLE
  install_ns_routing_table();
#else
  determine_constituents ();
#endif
  
#if USE_OVERLAY_MAP 
      overlay_map=node_map2<edge>(G,nil);
#endif

//        is_physical = edge_map<int>(G,0);
//        forall(e, physical_edges) 
//         { 
//  	 	is_physical [e] = 1;
//         }

/*    run_minimum_spanning_tree(); */


/*    return 0; */
}

int LEDA_get_join_target(int except = -1)
{
  set < node >source;
  node v;
  forall_nodes(v,G)
    {
      if (tree_status[v] && !node_status[v]&&
	  (get_node_number(v)!=except))
	{
	  source.insert(v);
	}
    }
  return get_node_number(get_random_node_from_set(source));
}

int inside = 0;
int topology_nodes = 0;
int narada_nodes = 0;

void LEDA_tree_status(int address, int present=0)
{
  printf("Called with address %x and present %d\n", address+1, present);
  tree_status[get_node_with_number(address)]= present;
  if (!  present)
    {
      inside--;
      return;
    }
  else
    {
      inside++;
      cout << "inside " << address << " total " <<inside << endl;
    }
  int all = 1;
//    node first_node, second_node;
//    forall(first_node, participants)
//      {
//        if (!tree_status[first_node])
//  	{
//  	all = 0;
//  	break;
//  	}
//      }
//  all = (inside==(participants.size ()));
  if (inside == narada_nodes)
    {
      cout << "all " << inside << " aboard." << endl;
      recompute_delay = 1;
      LEDA_dump_tree(my_clock(),NULL);

#if USE_NS_ROUTING_TABLE
//        dump_distance();
//        dump_routing_table_as_pipes();
      dump_routing_table();
//        dump_network3("pipes.config");
      fflush(stdout);
//        exit( 0);
#endif
    }
}
void LEDA_node_status(int address, int act_dead=0)
{
  printf("node statusCalled with address %x and act_dead %d\n",
	 address+ 1, act_dead);

  node entry =get_node_with_number(address );
  if (!entry)
    {
      cout << " cannot find  node with id " <<  address << endl;
       return;
    }

  node_status[entry]= act_dead;

//    recompute_delay=1;
  recompute_cost=1;
//  return;

  if (act_dead==0 && !participants.rank( entry))
    {
      participants.append(entry);
    }
  if (act_dead==1 && participants.rank( entry))
    {
      participants.remove(entry);
    }
}

void LEDA_parent_died(int address,double when)
{
  printf("parent died called with address %x time %lf\n",
	 address+ 1, when);

  node entry =get_node_with_number(address );
  if (!entry)
    {
      cout << " cannot find  node with id " <<  address << endl;
       return;
    }

  parent_died[entry]= when;
  reconnected[ entry]= 0;
}

void LEDA_reconnected(int address,double when)
{
  printf("reconnected called with address %x time %lf \n",
	 address+ 1, when);

  node entry =get_node_with_number(address );
  if (!entry)
    {
      cout << " cannot find  node with id " <<  address << endl;
       return;
    }
  if (parent_died[entry]==0.0)
    {
      //initial join
      return;
    }

  reconnected[ entry]= when;
  recoveries ++;
  double how_long =reconnected[ entry] -  parent_died[entry];
  recovery_sum += how_long;
  recovery_times.append (how_long);
  printf("recovery %x time %lf \n",
	 address+ 1, how_long);

  parent_died[entry]=0.0;
}

void LEDA_experiment_stopped  (double time_index)
{
  experiment_stopped =time_index;
  cout << "experiment_stopped at time " << time_index << endl;
}


void LEDA_measure_set (int*target, int from, int count)
{
  node fentry =get_node_with_number( from );
  if (!  fentry)
    {
      cout << "measure_set: invalid node " <<  from<<  endl;
      return;
    }
  for (int i = 0; i < count ;i++ )
    {
      node entry =get_node_with_number(target[i] );
      if (!  entry)
	{
	  cout << "measure_set: invalid node " << target[i] <<  endl;
	  return;
	}
      measurements_total+=overlay_hop_distance( fentry, entry);
      measurements_made++;
    }

//        cout << "measure_set" << endl;
}

void LEDA_experiment_started  (double time_index)
{
  experiment_started =time_index;
  cout << "experiment_started at time " << time_index << endl;
      referenceMSTcost=MSTcost;
      referencecost=last_reported_cost;
}

void LEDA_experiment_parameters (double ratio, double MST_ratio, double bound, double ratio_met)
{
  SPT_ratio= ratio;
  delay_bound = bound;
  target_MST_ratio =MST_ratio;
  desired_delay_bound_ratio =ratio_met;
      printf("experiment parameters  SPT ratio %lf, delay bound %lf, MST_ratio %lf desired_delay_bound_ratio  %lf\n",
	     SPT_ratio, delay_bound, target_MST_ratio,desired_delay_bound_ratio); 
}

void LEDA_init(int address, int source=0, int topo=0, int topo_nodes=0, int num_nodes=0)
{

  char filename[256];
      printf("in LEDA init wif %x and source %x\n",
	     address+ 1, source); 
//    cout << "in LEDA init wif " << address << " and source " << source << endl << flush;
  
  if (source==1)
    {
      narada_nodes = num_nodes;
      topology_nodes = topo_nodes;
      cout << "set number of nodes in LEDA to " << narada_nodes << endl;
      cout << "set number of topology nodes in LEDA to " << topology_nodes << endl;
    }

  if (readnetwork == false)
    {
//        sprintf(filename, "/usr/project/spider1/ns/ns-allinone-2.1b8a/ns-2.1b8a/tcl/adolfo/topologies/tsx/topos/ts%d-%d.gml", topology_nodes, topo);
      sprintf(filename, "/usr/project/ari_scratch1/dkostic/ns-allinone-2.1b8a/ns-2.1b8a/tcl/adolfo/topologies/tsx/topos/ts%d-%d.gml", topology_nodes, topo);
      printf("LEDA: reading file %s\n", filename);
      read_network(filename);
    }
  readnetwork = true;

//    cout << "LEDA init getting node" << endl << flush;

  node entry =get_node_with_number(address );

//    cout << "LEDA init got node" << endl << flush;

  participants.append(entry);

//    cout << "LEDA init appended entry" << endl << flush;

  if (source)
    {
      source_= entry;
    }

//    cout << "LEDA init assigning additional edges" << endl << flush;

  assign_additional_edges(entry);

//   LEDA_node_status(address);
  node_status[entry]= 0;

//    recompute_delay=1;
  recompute_cost=1;
  cout << "leaving LEDA init" << endl << flush;
}

void LEDA_set_link_delay (int from, int to, double value)
{
  if (from ==0&&to==0)
    {
#if USE_NS_ROUTING_TABLE
//    install_ns_routing_table();
#else
  determine_constituents ();
#endif
      recompute_delay=1;
      recompute_cost=1;
      printf(" finished recomputing constituents\n");
      printf("sleep 25\n");

    }
  else
    {
      node s =get_node_with_number (from);
      node v =get_node_with_number (to);
      edge  e =  find_edge (s, v);
      printf(" changing physical link delay between %x and %x from %lf to %lf\n",
	     from+1, to+1,G[e], value); 
      double previous =G[e];
      G[e]=value;
//        update_shortest_paths(e,previous, value );
//        check_shortest_paths();
//        edge r = reverse_edge (e);
//        previous =G[r];
//        G[r]=value;
//        update_shortest_paths(r,previous, value);
printf("pipe config %4d bw %dkbit/s delay %lfms plr 0.00000 queue 50slots\n",
       edge_number[e], bandwidth[e],G[e]);
    }
}


void LEDA_change_link_cost (int from, int to)
{
  node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  edge  e =  find_edge (s, v);
  double old =LEDA_get_overlay_cost(from, to);
  assign_single_edge_cost(e);

  printf("changing physical link  cost between %x and %x from %lf to %lf\n",
	     from+1, to+1, old,LEDA_get_overlay_cost(from, to)) ;
}

random_source edge_random_count( 5,15);
void LEDA_change_random_link_costs ()
{
  int count=1;
//    edge_random_count >>count;
  printf(" changing random link costs for  %d links\n",count);

  for (int i = 0; i <count  ;i++ )

    {
      edge  e =  get_random_edge ();
       double previous =physical_cost[e];

#if 1
       double value = 2*(-log(drand48()))+ 1.0;
       physical_cost[e]=initial_physical_cost[e]*value;
       edge r = reverse_edge(e);
       physical_cost[r]=physical_cost[e];
       printf(" changing random link costs for %x from %lf to %lf \n",
	      e, previous,physical_cost[e]);
#else
        assign_single_edge_cost(e);
#endif	
    }
  recompute_cost= 1;
}


void LEDA_set_link_down (int from, int to)
{
  node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  edge  e =  find_edge (s, v);
  is_up[e]=false;
  edge r = reverse_edge (e);
  is_up[r]=false;

  determine_constituents();
}

void LEDA_set_link_up (int from, int to)
{
  node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  edge  e =  find_edge (s, v);
  is_up[e]=true;
  edge r = reverse_edge (e);
  is_up[r]=true;

  determine_constituents();
}

void LEDA_add_overlay_link (node s, node v)
{
#if USE_OVERLAY_MAP 
  edge  e =  find_overlay_edge (s, v);
  edge r = reverse_overlay_edge (e);
#endif
  if ( !overlay_edge_list.rank(e) )
    {
      printf("adding an overlay link between %x and %x costing  %lf \n",
      IP_addresses[s], IP_addresses[v], overlay_cost_distance(s, v)); 
      overlay_edge_list.append(e);
#ifndef ADOLFO_HACK_20021228
      overlay_edge_list.append(r);
#endif
    }
  else
    {
//        printf("duplicate overlay link between %x and %x costing  %lf \n",
//  	     from+1, to+1,LEDA_get_overlay_cost(from, to)); 
    }
}


void LEDA_remove_overlay_link (node s, node v)
{
#if USE_OVERLAY_MAP 
      edge  e =  find_overlay_edge (s, v);
      edge r = reverse_overlay_edge (e);
#endif
  if ( overlay_edge_list.rank(e) )
    {
//        printf("removing an overlay link between %x and %x costing  %lf \n",
//  	     from+1, to+1,LEDA_get_overlay_cost(from, to)); 
      overlay_edge_list.remove(e);
#ifndef ADOLFO_HACK_20021228
      overlay_edge_list.remove(r);
#endif
      changes_last_interval++;
    }
}


void LEDA_add_overlay_link (int from, int to)
{
//    cout << "adding an overlay link between "  << from  << " and "
//         << to << " costing "  << LEDA_get_overlay_cost(from, to) << endl << flush;
      node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_add_overlay_link %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_add_overlay_link");
    }
#if USE_OVERLAY_MAP 
  edge  e =  find_overlay_edge (s, v);
  edge r = reverse_overlay_edge (e);
#endif
  if ( !overlay_edge_list.rank(e) )
    {
      printf("adding an overlay link between %x and %x costing  %lf \n",
	     from+1, to+1,LEDA_get_overlay_cost(from, to)); 
      overlay_edge_list.append(e);
      overlay_edge_list.append(r);

    }
  else
    {
      printf("duplicate overlay link between %x and %x costing  %lf \n",
	     from+1, to+1,LEDA_get_overlay_cost(from, to)); 
    }
//    LEDA_add_overlay_link(s,v);
}


void LEDA_remove_overlay_link (int from, int to)
{
  node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_remove_overlay_link %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_remove_overlay_link");

    }
#if USE_OVERLAY_MAP 
      edge  e =  find_overlay_edge (s, v);
      edge r = reverse_overlay_edge (e);
#endif
  if ( overlay_edge_list.rank(e) )
    {
      printf("removing an overlay link between %x and %x costing  %lf \n",
	     from+1, to+1,LEDA_get_overlay_cost(from, to)); 
      overlay_edge_list.remove(e);
      overlay_edge_list.remove(r);
      changes_last_interval++;
    }
//  LEDA_remove_overlay_link(s,v);
}

double LEDA_get_overlay_cost (int from, int to)
{
//        printf("getting overlay link cost between %x and %x \n",
//  	     from+1, to+1); 
   node s =get_node_with_number (from);
   node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_get_overlay_cost %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_get_overlay_cost");

    }
      return overlay_cost_distance(s,v);
}

double LEDA_get_overlay_hops (int from, int to)
{
//        printf("getting overlay link cost between %x and %x \n",
//  	     from+1, to+1); 
   node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_get_overlay_hops %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_get_overlay_hops");

    }
      return overlay_hop_distance(s,v);
}

double LEDA_get_overlay_cost (const char*source,const char*destination)
{
      printf("getting overlay link cost between names %s and %s \n",
	     source, destination); 
  node s =get_node_from_name(source);
  node v =get_node_from_name(destination);
  if (!s || !v)
    {
      printf("LEDA_get_overlay_cost with names %s and %s nodes %x and %x \n",
	     source, destination,s,v); 
      error ("LEDA_get_overlay_cost");

    }
      return overlay_cost_distance(s,v);
}

double LEDA_get_overlay_delay (int from, int to)
{
//        printf("getting overlay link cost between %x and %x \n",
//  	     from+1, to+1); 
   node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_get_overlay_delay %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_get_overlay_cost");

    }
      return overlay_true_distance(s,v);
}
#if USE_NS_ROUTING_TABLE
double LEDA_get_ns_overlay_delay (int from, int to)
{
//        printf("getting overlay link cost between %x and %x \n",
//  	     from+1, to+1); 
   node s =get_node_with_number (from);
  node v =get_node_with_number (to);
  if (!s || !v)
    {
      printf("LEDA_get_ns_overlay_delay %x and %x nodes %x and %x \n",
	     from+1, to+1,s,v); 
      error ("LEDA_get_ns_overlay_cost");

    }
      return ns_overlay_true_distance(s,v);
}
#endif
void prepare_list(list<edge>& result)
  {
    node v;
    edge e;
    node first_node, second_node;
    
    forall_nodes(v,G) 
      G. hide_node (v);
    forall_edges(e, G)
      G. hide_edge (e);

    forall(first_node, participants)
      {
	if (!node_status[first_node] && G.is_hidden (first_node))
	  {
	    G. restore_node (first_node);
	  }
      }

#if IN_USE_MAP 
     in_use =node_map2 <bool> (G,false);
#else
  in_use = node_matrix <bool> (G,false);
#endif


edge
  new_edge;
    forall(new_edge,result)
      {
	if (!node_status[source(new_edge)] && 
	    !node_status[target(new_edge)] && 
	    G.is_hidden (new_edge))
	  {
		G. restore_edge(new_edge);
		in_use (source(new_edge), target(new_edge))= true;
	  }
      }

//      printf("prepare_list \n");
  }


void prepare_MST()
  {
    node v;
    edge e;
    node first_node, second_node;
    
    forall_nodes(v,G) 
      G. hide_node (v);
    forall_edges(e, G)
      G. hide_edge (e);

    forall(first_node, participants)
      {
	if (!node_status[first_node] && G.is_hidden (first_node))
	  {
	    G. restore_node (first_node);
	  }
      }
       forall(first_node,participants)
	 	forall(second_node, participants)
	 	  {
		    if (first_node == second_node)
		      {
			continue;
		      }
#if USE_OVERLAY_MAP 
			edge
			  new_edge =overlay_map(first_node,second_node);
			if (!node_status[source(new_edge)] && 
			    !node_status[target(new_edge)] && 
			    G.is_hidden (new_edge))
			  {
			    G. restore_edge(new_edge);
			    in_use (source(new_edge), target(new_edge))= true;
			  }
#endif
		  }
//      printf("prepare_MST \n");

  }


void restore_normal()
{
    edge e;
       G.restore_all_nodes();		  
       G.restore_all_edges();		  
       
    forall_edges(e, G)
      if (!  is_physical[e])
		G. hide_edge (e);
//      printf("restore_normal \n");

}
FILE *fp= NULL;
int printout = 0;

int walk_tree(node parent,node first_node,node_array<node>& visit,node_map2<double>& ddistance)
{
//    if (printout)
//      cout << "walking "  
//  	 << get_node_number(first_node) 
//  	 << " to "  
//  	 << (first_node) 
//  	 << endl << flush;
  edge candidate = NULL;
  int k= 0;
  int maximum = 0;
  forall_out_edges(candidate, first_node )
    {
      // this doesn't make sense since the solution list has edges in both directions

//        if (visit[target (candidate)]&&target (candidate)!= parent)
//  	{
//  	  printf("node{%x}.child_numparents > 1(offender = %x: visited from  %x);\n", 
//  		 get_node_number(first_node)+1,
//  		 get_node_number(target (candidate))+1,
//  		 get_node_number(visit[target (candidate)])+1);
//      cout << "exiting"  
//  	 << endl << flush;
//  	    fclose (fp);
//  	  exit(5); 
//  	}
      if (visit[target (candidate)])
	{
	  continue;
	}
      visit[target (candidate)]=first_node;
 
      k++;
      double latency_to_parent_=

	ddistance(source_, target (candidate));
      double cost_to_parent_ = overlay_cost_distance
	(
	 first_node,
	 target (candidate));	
#if USE_MODELNET
      char *color; 
      if (modelnet_distance[target (candidate)] <delay_bound )
	{
	  color = "blue";
	}
      else
	{
	  color = "red";
	}

      double delay_to_parent_=(modelnet_distance[target (candidate)]-modelnet_distance[first_node]);
      if (first_node==source_)
	{
	  delay_to_parent_=modelnet_distance[target (candidate)];
	}

      in_addr parentia;parentia.s_addr=IP_addresses[first_node];
      in_addr childia;childia.s_addr=IP_addresses[target (candidate)];

      fprintf(fp, "\"%s (%6.2lf) \" ->",
      &G[ first_node][6],modelnet_distance[first_node]);
fprintf(fp, " \"%s (%6.2lf) \"[color=\"%s\"][style=dotted] [label=\"%6.2lf\"];\n", 
	 &G[target (candidate)][6],modelnet_distance[target (candidate)],color,
	      delay_to_parent_);
//        fprintf(fp, "\"%s(%6.2lf)\" ->",
//        inet_ntoa(parentia),modelnet_distance[first_node]);
//  fprintf(fp, " \"%s(%6.2lf)\"[color=\"%s\"][style=dotted] [label=\"%6.2lf\"];\n", 
//  	inet_ntoa(childia),modelnet_distance[target (candidate)],color,
//  	      delay_to_parent_);
 
#else
      double delay_to_parent_ = overlay_true_distance
	(
	 first_node,
	 target (candidate));	
      fprintf(fp, "\"%x\" -> \"%x\"[color=\"%s\"] [label=\"%6.2lf\"];\n", 
	      get_node_number(first_node)+1,
	      get_node_number(target (candidate))+1,
	      "black",
	      delay_to_parent_);
#endif 
//        fprintf(fp, "\"%x\" -> \"%x\"[color=\"red\"] [label=\"%6.2lf\"];\n", 
//  	      get_node_number(first_node)+1,
//  	      get_node_number(target (candidate))+1,
//  	      delay_to_parent_);
//  	      ddistance(first_node, target (candidate)));
//  	      1000*(modelnet_distance[target (candidate)]-modelnet_distance[first_node]));
 
//        fprintf(fp, "node{%x}.child{%d} = %x;\n", 
//  	      get_node_number(first_node)+1, k,
//  	      get_node_number(target (candidate))+1);
//        // this is also the degress (minus 1)
//        fprintf(fp, "node{%x}.cost = %lf;\n", 
//  	      get_node_number(target (candidate))+1, cost_to_parent_ );
//        fprintf(fp, "node{%x}.delay = %lf;\n", 
//  	      get_node_number(target (candidate))+1, G[candidate]
//  	      );
//        fprintf(fp, "node{%x}.latency = %lf;\n", 
//  	      get_node_number(target (candidate))+1, latency_to_parent_
//  	      );
//        fprintf(fp, "node{%d}.child{%d} = %d;\n", 
//  	      get_node_number(first_node)+1, k,
//  	      get_node_number(target (candidate))+1);
//        // this is also the degress (minus 1)
//        fprintf(fp, "node{%d}.cost = %lf;\n", 
//  	      get_node_number(target (candidate))+1, cost_to_parent_ );
//        fprintf(fp, "node{%d}.delay = %lf;\n", 
//  	      get_node_number(target (candidate))+1, G[candidate]
//  	      );
//        fprintf(fp, "node{%d}.latency = %lf;\n", 
//  	      get_node_number(target (candidate))+1, latency_to_parent_
//  	      );
        if (printout)
	cout << "edge "  <<  candidate
	     << " between "
	     << get_node_number(first_node) 
	     << " to "  
	     << get_node_number(target (candidate))
	     << " delay " 
	     << G[candidate] 
	     << " cost " 
	     << 	overlay_cost_distance(first_node,target (candidate)) 
	     << " distance " << latency_to_parent_
	     << endl << flush;
      int value =walk_tree( first_node,target (candidate), visit, ddistance);
      if (maximum <value)
	{
	  maximum =value;
	}
    }
  if (maximum <k)
    {
      maximum =k;
    }
//    fprintf(fp, "node{%x}.numchildren = %d;\n", 
//  	  get_node_number(first_node)+1, k );
//    fprintf(fp, "node{%d}.numchildren = %d;\n", 
//  	  get_node_number(first_node)+1, k );
  int count = 0;
  forall_in_edges(candidate, first_node )
    count++;
//    if (count > 1)
//  {
//  printf("node{%d}.numparents = %d;\n", 
//  	  get_node_number(first_node)+1, count);
//   exit(5); 
//  }

  return maximum;
}

int dump_tree(node_map2<double>& ddistance,
	      char * name)
{

  fp = (FILE *)fopen(name, "w");
  if (!fp )
{
  return 0;
}
  int length =(int) participants.size()/8;
  if (length > 120)
{
  length = 120;
}

  int maximum = 0;
//    fprintf(fp, "digraph G {\nsize=\"50,50\";rotate=\"90\";\n");
  fprintf(fp, "digraph G {\n");
fprintf(fp, "size=\"%d,12\"\n",  length);
fprintf(fp, "ratio=fill\n");
fprintf(fp, "orientation=landscape\n");
fprintf(fp, "label=\"%6.2lf\"\n", delay_bound);

//  	  fprintf(fp, "node{%d}.cost = %lf;\n", 
//  		  get_node_number(source_)+1, 0.0);
//  	  fprintf(fp, "node{%d}.latency = %lf;\n", 
//  		  get_node_number(source_)+1, 0.0
//  		  );
//  	  fprintf(fp, "node{%d}.delay = %lf;\n", 
//  		  get_node_number(source_)+1, 0.0
//  		  );
	  node_array<node> visit(G,NULL);
  node first_node, second_node;
  visit [source_ ] = source_ ;
  maximum = walk_tree( NULL,source_, visit, ddistance);
  fprintf(fp, "}\n");

  fclose (fp);
  return maximum;
}

double LEDA_cost_ratio()
  {
    return last_reported_cost/MSTcost ;
}

double external_cost;
int satisfies[60];

void dump_list_stats(char * title, double time_index,list<edge>& result, 
		     char * name,int dump = 0,int cumulative = 0,
		     char * run= NULL,int  suppress= 0)
{
  int degree = 0;
  int connected = 0;
  int satisfied = 0;
  node_map2<double> ddistance (G);
  edge_array<double > dcost (G, infinity);
  list<double >  distances;
  double cost = compute_cost(dcost, result)/2;
  last_reported_cost= cost;
  double diameter;
#if USE_MODELNET
  if (strncmp("solution", title, 8)==0)
    diameter = calculate_diameter(dcost,ddistance, connected, satisfied, distances, 1);
  else
    diameter = calculate_diameter(dcost,ddistance, connected, satisfied, distances, 0);
#else
  diameter = calculate_diameter(dcost,ddistance, connected, satisfied, distances, 0);
#endif

  distances.sort( );
  double sample;
  int counter = 0;
  double percentile90 = 0;
  double percentile95 = 0;
  double percentile98 = 0;
  double percentile99 = 0;
  forall(sample,distances)
    {
      if (counter == (int)(0.9*( double) distances.length()))
	{
	  percentile90 = sample;
	}
      else if (counter == (int)(0.95*( double) distances.length()))
	{
	  percentile95 =sample;
	}
      else if (counter == (int)(0.98*( double) distances.length()))
	{
	  percentile98 = sample;
	}
      else if (counter == (int)(0.99*( double) distances.length()))
	{
	  percentile99 = sample;
	}

      counter++;
    }

  double LEDA_diameter = diameter;

  if (use_ns_tree_farthest != 0)
    diameter = my_ns_tree_farthest * 1000;

  double average = 0;
  double maximum = calculate_stress (physical_edges,
				     is_physical,
				     average);
  external_cost = cost;

  int active = 0;
  node second_node;
  forall(second_node,participants)
    {
      if (!node_status[second_node])
	{
	  active++;
	}
    }
  if (active==participants.size())
{
  skip_dijkstra=1;
}

  cout << "active "<<  active << " connected "<<  connected
       << endl << flush;

  if (experiment_started > 0.0 && experiment_stopped==0.0)
    {
      //      if (diameter >(SPTdelay*SPT_ratio))
      if (diameter > delay_bound)
	{
      cout << "delay violated "  << diameter
	   << " SPTdelay "  << SPTdelay
	   << " times_delay_exceeded "  <<times_delay_exceeded
	   << endl << flush;
 	  times_delay_exceeded++;
	  //	   double  ratio =diameter /(SPTdelay*SPT_ratio) -1;
	   double  ratio =diameter /delay_bound -1;
	  average_over_spt+=ratio;
	  if (ratio >maximum_over_spt)
	    {
	      maximum_over_spt= ratio;
	    }
	}
      if ((cost/MSTcost) >(referencecost/referenceMSTcost))
	{
      cout << "reference cost exceeded "  << cost/MSTcost
	   << " referenceMSTcost ratio"  <<  referencecost/referenceMSTcost
	   << " times_cost_exceeded "  <<times_cost_exceeded
	   << endl << flush;
 	  times_cost_exceeded++;
	  double ratio=(cost/MSTcost)/(referencecost/referenceMSTcost) -1;
	  average_over_cost+=ratio;
	  if (ratio >maximum_over_cost)
	    {
	      maximum_over_cost= ratio;
	    }
	}
     }
  double experiment_duration= 1;
  if (experiment_started > 0.0 && experiment_stopped==0.0)
    {
      experiment_duration = my_clock() - experiment_started;
    }
  else if (experiment_started > 0.0 && experiment_stopped>0.0)
    {
      experiment_duration = experiment_stopped - experiment_started;
    }
  disconnected_sum += 1 - (connected/ (double)active);

  char buffer [256];
//    if (!  run)
//      strcpy(buffer, name);
//    else
//      sprintf(buffer,"solutions/solution-%s.m" , run);

    sprintf(buffer,"solutions/solution-%d-%6lf.dot",(int) time_index,diameter);

      cout << title << time_index << " "
	   << "cost "  << cost
	   << " max root delay "  << diameter/1000.0;
	if (dump)
	  {
	    degree = dump_tree(ddistance, buffer);
	  }
	cout << " max degree "  <<  degree;
	double mystress = 0.0;
	if (measurements_made != 0)
	  mystress = measurements_total/measurements_made;

	double myrectime = 0.0;
	if (recoveries > 0)
	  myrectime = (recovery_sum/(double)recoveries);

	cout << " size "  << result.size()/2
	     << " average stress "   << average 
	     << " maximum stress "   << maximum 
	     << " cost ratio "  << cost/MSTcost
	  //	   << " delay ratio "  << diameter/(SPTdelay*SPT_ratio)
	     << " delay ratio "  << diameter/delay_bound
	     << " maximum_over_spt "  << maximum_over_spt
	     << " average_over_spt "  << average_over_spt
	     << " maximum_over_cost "  << maximum_over_cost
	     << " average_over_cost "  << average_over_cost
	     << " times_spt_exceeded "  <<times_delay_exceeded
	     << " times_cost_exceeded "  <<times_cost_exceeded
	     << " changes_last_interval "  <<changes_last_interval
	     << " measurements_stress "  << mystress
	     << " SPT delay "  << SPTdelay
	     << " LEDA_acheived delay "  << LEDA_diameter
	     << " ratio_connected "  << (connected/ (double)active)
	     << " ratio_active "  << (active/(double)narada_nodes)
	     << " ratio_satisfied "  << (satisfied/(double)active)
	     << " average_rec_time "  << myrectime
	     << " disconnected_sum "  << disconnected_sum
	     << " 90 "  << percentile90/1000.0
	     << " 95 "  << percentile95/1000.0
	     << " 98 "  << percentile98/1000.0
	     << " 99 "  << percentile99/1000.0
	     << " hundred "  << LEDA_diameter/1000.0
	     << endl << flush;
	changes_last_interval= 0;

//        if ( (desired_delay_bound_ratio> 0)&&
//  	   (active/(double)narada_nodes)== 1&&
//  	   (connected/ (double)active)== 1&&
//  	  (satisfied/(double)active)>desired_delay_bound_ratio)
//  	{
//  	  printf("desired_delay_bound_ratio_met  of %lf  met, exiting\n",
//  		 desired_delay_bound_ratio);
//  	  exit(17);
//  	}

  if (cumulative  && connected == active
)
    {
  char buffer [256];
    sprintf(buffer,"solutions/solution-%s-%lf-cdf.dat" , name,time_index);
       fp = (FILE *)fopen( buffer, "w");
      if (!fp )
	{
	  return;
	}
      for (int counter = 0; counter < 150; counter++ )
	satisfies [counter]= 0;
      for (int counter = 0; counter < 150; counter++ )
	{
	  //	  double sample = counter*0.01*SPTdelay*SPT_ratio/1000.0;
	  double sample = counter*0.01*delay_bound/1000.0;
	  node second_node;
	  forall(second_node,participants)
	    {
	      if (ddistance(source_,second_node)/1000.0<sample)
		{
		  satisfies [counter]++;
		}
	    }
	  fprintf(fp,"%lf %lf\n", sample,
		  (double)satisfies [counter]  /participants.size());
	}
      fclose (fp);
    }
}


void add_reverse_edges(list < edge >& result)
  {
  edge e ;
  list < edge >addition;
  forall(e,result) 
  { 
#if USE_OVERLAY_MAP 
      edge r = reverse_overlay_edge (e);
#endif
      addition.append(r);
  }
  result.conc(addition);
}

void fix_overlay_links(node s, list<edge>& result, node parent=0) {

  node v;
  forall_nodes(v,G)
    {
      
#if IN_USE_MAP 
      if (in_use.defined (s,v) && in_use (s,v))
#else
	if (in_use (s,v))
#endif
	  {
	    //	    cout << " overlay edge is correct " << G[s] << " " << G[v] << endl; 
	    fix_overlay_links(v, result, s);
	  }
      
#if IN_USE_MAP 
      if (in_use.defined (v,s) && in_use (v,s))
#else
	if (in_use (v,s))
#endif
	  {
	    if (v != parent) {
	      edge e;
	      forall (e,result)
		if (source(e) == v && target(e) == s) {
		  result.remove(e);
#if USE_OVERLAY_MAP       
		  edge f =  find_overlay_edge (s,v);
#else
		  edge f =  find_edge (s,v);
#endif
		  result.append(f);
		  //		  cout << " fixed overlay edge " << G[s] << " " << G[v] << endl; 
		  prepare_list(result);
		  fix_overlay_links(v, result, s);
		  continue;
		}
	    }
	  }
    }	   
}

double LEDA_dump_tree(double time_index,char * name,double ns_tree_farthest=-1.0)
{
  edge e ;
      is_physical = edge_array<int>(G,0);
      forall(e, physical_edges) 
       { 
	 is_physical [e] = 1;
       }

  prepare_MST();
  list < edge > result1 = MIN_SPANNING_TREE1(G);
#ifndef ADOLFO_HACK_20021228
  add_reverse_edges(result1);
#endif
  list < edge > result2 = DIJKSTRA1(G);
#ifndef ADOLFO_HACK_20021228
  add_reverse_edges(result2);
#endif
  list < edge > result6 = IP_UNICAST(G);
#ifndef ADOLFO_HACK_20021228
  add_reverse_edges(result6);
#endif
   list < edge > result4 = MIN_SPANNING_TREE_COST1(G);
#ifndef ADOLFO_HACK_20021228
   add_reverse_edges(result4);
#endif
   list < edge > result5 = DIJKSTRA_COST1(G);
#ifndef ADOLFO_HACK_20021228
   add_reverse_edges(result5);
#endif
  list < edge > result3 = OVERCAST(G);
#ifndef ADOLFO_HACK_20021228
  add_reverse_edges(result3);
#endif


//    cout << "MST      : time_index: " << time_index  << " ";
  restore_normal();
  prepare_list(result1);
#ifdef ADOLFO_HACK_20021228
  fix_overlay_links(get_node_from_name("client0"), result1);
#endif
  dump_list_stats("MST DELA : time_index: ", time_index,result1,"mst.m",1);
  
//    printout = 1;

//    cout << "DIJKSTRAD: time_index: " << time_index << " ";
  restore_normal();
  prepare_list(result2);
  dump_list_stats("DIJKSTRAD : time_index: ", time_index,result2,"dijkstra.m",1);
  printout =0;
//      printout = 1;
  
//    cout << "UNICAST  : time_index: " << time_index << " ";
  restore_normal();
  prepare_list(result6);
  dump_list_stats("UNICAST  : time_index: ", time_index,result6,"unicast.m",1);
  printout =0;

//    cout << "MST COST :  time_index: " << time_index  << " ";
  restore_normal();
  prepare_list(result4);
  dump_list_stats("MST COST :  time_index: ", time_index,result4,"mst-cost.m",1);

  cout << "DIJKSTRAC: time_index: " << time_index << " ";
  restore_normal();
  prepare_list(result5);
  dump_list_stats("DIJKSTRAC: time_index: ", time_index,result5,"dijkstra-cost.m",1);

//    cout << "OVERCAST : time_index: " << time_index << " ";
  restore_normal();
  prepare_list(result3);
  dump_list_stats("OVERCAST : time_index: ", time_index,result3,"overcast.m",1);

//    cout << "solution : time_index " << time_index << " ";
  restore_normal();
  prepare_list(overlay_edge_list);

  if (ns_tree_farthest!= -1.0)
    {
      use_ns_tree_farthest = 1;
    }
  else
    {
      use_ns_tree_farthest = 0;	
    }

  dump_list_stats("solution: time_index ", time_index,overlay_edge_list,"solution.m", 1, 1, name);
  use_ns_tree_farthest = 0;
  restore_normal();


    {
      double sample1;
      double maximum = 0;
      forall(sample1,recovery_times) 
	{
	  if (sample1> maximum)
	    {
	      maximum = sample1;
	    }
	}
      if (maximum> 50.0)
	{
	  maximum=50.0;
	}

	char buffer [256];
	sprintf(buffer,"solutions/recovery-cdf.dat" );
       fp = (FILE *)fopen( buffer, "w");
      if (!fp )
	{
	  return 0;
	}
      for (int counter = 0; counter < 100; counter++ )
	satisfies [counter]= 0;
      for (int counter = 0; counter < 100; counter++ )
	{
	  double sample = counter*0.01*maximum;
	  forall(sample1,recovery_times) 
	    {
	      if (sample1<sample)
		{
		  satisfies [counter]++;
		}
	    }
	  fprintf(fp,"%lf %lf\n", sample,
		  (double)satisfies [counter]/recovery_times.size());
	}
      fclose (fp);
    }

//    dump_ns_next_hop();
    cout << "diameter"<< calculate_physical_network_diameter()<< endl;
}

double compute_SPT_delay()
{
  prepare_MST();
  list < edge > result2 = DIJKSTRA1(G);
#ifndef ADOLFO_HACK_20021228
  add_reverse_edges(result2);
#endif
  restore_normal();

  prepare_list(result2);
#ifdef ADOLFO_HACK_20021228
  fix_overlay_links(get_node_from_name("client0"), result2);
#endif
  int degree = 0;
  node_map2<double> ddistance (G);
  edge_array<double > dcost (G, infinity);
  int connected = 0;
  int satisfied = 0;

  double cost = compute_cost(dcost, result2)/2;
list<double >  distances;
  double diameter = calculate_diameter(dcost,ddistance, connected, satisfied, distances);
  double average = 0;
  restore_normal();
    cout << "compute_SPT_delay "<< diameter<< endl;
  return diameter;
}
double compute_MST_cost()
{
    prepare_MST();
    list < edge > result4 = MIN_SPANNING_TREE_COST1(G);
#ifndef ADOLFO_HACK_20021228
    add_reverse_edges(result4);
#endif

    prepare_list(result4);
    edge_array<double > dcost (G, infinity);
    double cost =compute_cost(dcost, result4)/2;
    restore_normal();
    cout << "compute_MST_cost "<< cost<< endl;

    return cost;
}
void recompute_if_needed()
{
      if (recompute_cost)
	{
	  MSTcost =compute_MST_cost();
	  recompute_cost = 0;
	}
      if (recompute_delay)
	{
	  SPTdelay = compute_SPT_delay();
	  recompute_delay = 0;
	}
}

double LEDA_MST_cost()
{
  recompute_if_needed();

    return MSTcost;
}

double LEDA_SPT_delay()
{
  recompute_if_needed();

  return SPTdelay;
}


double LEDA_dump_stats(double time_index, double ns_tree_farthest)
{

  my_ns_tree_farthest = ns_tree_farthest;

  edge e ;
      is_physical = edge_array<int>(G,0);
      forall(e, physical_edges) 
       { 
	 is_physical [e] = 1;
       }
  recompute_if_needed();

//    cout << "solution: time_index " << time_index << " ";
  prepare_list(overlay_edge_list);
  if (ns_tree_farthest!= -1.0)
    {
      use_ns_tree_farthest = 1;
    }
  else
    {
      use_ns_tree_farthest = 0;	
    }

  dump_list_stats("solution: time_index " , time_index,overlay_edge_list,"solution", 1);
  use_ns_tree_farthest = 0;
  restore_normal();

}

void import_node_assignment(int topology,int topology_nodes, int participant_nodes, int seed)
{
  FILE *myfile;
  int  from;
  int i=0;
  char assignment_file[256];
//        sprintf(assignment_file, "/usr/project/spider1/ns/ns-allinone-2.1b8a/ns-2.1b8a/tcl/adolfo/topologies/tsx/topos/assign-%d-%d-%d.list", topology_nodes, participant_nodes, seed);
      sprintf(assignment_file, "/usr/project/ari_scratch1/dkostic/ns-allinone-2.1b8a/ns-2.1b8a/tcl/adolfo/topologies/tsx/topos/assign-%d-%d-%d.list", topology_nodes, participant_nodes, seed);
cout << assignment_file << endl;
  myfile = (FILE *)fopen(assignment_file, "r");
  i=0;
  int route = 0;
  while (fscanf(myfile, "%d", &from)!=EOF)
    {
//        cout << from << endl;

      i++;
      if (i==1)
	{
	LEDA_init(from, 1,topology,topology_nodes, participant_nodes);
	}
      else
	{
	LEDA_init(from);
	}
   }
  fclose (myfile);
      cout << "assignment done" << endl;
}

void replay_file( char* replay_file,char* routing_file)
{
  double time_index= 0;
  double dump_time_index= 0;
  double starting_time_index= 0;
  int starting_set = 0;


 FILE *myfile;
  int i=0;
  char intro[80];
  char name[80];
  char ip[80];
  char rest[512];
  char command[80];
  char status[80];
  char parent[80];
  char child[80];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  myfile = (FILE *)fopen(replay_file, "r");
  i=0;
//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
//        cout << whole_line << endl;
//        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      if (strncmp (whole_line,"Node", 4))
	{
	  continue;
	}
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %lf %d %s %[^\n]s", 
		 &intro, &ip, &name,  &time_when, &tid,&command, &rest))!= 7)
	{
	  continue;
	}
//  printf("got %d, %s %s/%s(%f): %s %s\n", 
//         successful,intro,ip, name,  time_when, command, rest);
//  printf("got %f %f %f\n", 
//         time_when,starting_time_index,dump_time_index);

      if (!strcmp (command,"REPLAY_EXPERIMENT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed;
	  if (sscanf(rest, "%s %d %d %d %d %d %lf %lf", 
		     &name, &root,&topology, &topology_nodes, &participant_nodes, &seed, &delay_bound, &SPT_ratio)!= 8)
	    continue;
	  cout
	    << "Running with delay bound "  << delay_bound<< endl
	    << "Running with delay ratio " << SPT_ratio<< endl;
	  delay_bound*= 1000; 
	  import_node_assignment(topology, topology_nodes, participant_nodes, seed); 
	  import_routing_table(routing_file);
	  node entry;
	  //and make all all of them act dead, except root
	  forall_nodes(entry,G) 
	    {
	      node_status[entry]= 1;
	    }
	  node_status[get_node_from_name(name)]= 0;
  printf("initialize root %x\n",get_node_number(get_node_from_name(name))+ 1);
	  initialized = 1;
	}
//        if (!initialized)
//  	{
//  	  continue;
//  	}
      if (!strcmp (command,"REPLAY_INIT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed, delay_bound;
	  if (sscanf(rest, "%s %d", 
		     &name, & root)!= 2)
	    continue;
	  //make sure they don't act dead anymore
	  node_status[get_node_from_name(name)]= 0;
	  recompute_cost = 1;

	  printf("initialize  %x\n",get_node_number(get_node_from_name(name))+ 1);
	}

      if (!initialized)
	{
	  continue;
	}
      if (!starting_set)
	{
	  starting_set = 1;
	  starting_time_index =time_when;
	}

      if (!strcmp (command,"REPLAY_ADD")||
	  !strcmp (command,"REPLAY_REMOVE"))
	{
//  printf("got %s %s\n", 
//         command, rest);

	  if (sscanf(rest, "%s %s", 
		     &parent, &child)!= 2)
	    continue;
	  if (!strcmp (command,"REPLAY_REMOVE"))
	    {
printf("REPLAY_REMOVE %s %s\n", 
       parent, child);
	      LEDA_remove_overlay_link(get_node_from_name(parent),
				       get_node_from_name(child));
	      //we dont want erroneous disconnected printouts
	      continue;
	    }
	  else
	    {
	      LEDA_add_overlay_link(get_node_from_name(parent),
				    get_node_from_name(child));
printf("REPLAY_ADD %s %s\n", 
       parent, child);
	    }
	}
      else if(!strcmp (command,"REPLAY_HEIGHT"))
	{
	  double farthest = 0;
	  sscanf(rest, "%lf", 
		 &farthest);
	  my_ns_tree_farthest = farthest;
	  printf("SETTING TREE HEIGHT %lf \n",farthest);
	}

      if (!initialized)
	{
	  continue; 
	}
      if (!dump_time_index||
	  time_when> dump_time_index+ 1)
	{
	  dump_time_index=time_when; 
	  LEDA_dump_stats(time_when-starting_time_index, my_ns_tree_farthest);
	}
    }
  fclose(myfile);
  LEDA_dump_tree(dump_time_index-starting_time_index,NULL);

}

void LEDA_initialize_ModelNet(int topology,int topology_nodes, int participants, int seed)
{
  import_node_assignment(topology,topology_nodes, participants,  seed); 
  import_routing_table("/usr/research/home/dkostic/mesh/table");
}

#if USE_MODELNET




void read_modelnet_ip()	
 {
  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }

  char database_file[256];
  strcpy(database_file,mn_mdb);
  strcat(database_file,"/out.ip");

  FILE *myfile;
  int i=0;
  char lead[80];
  char host[80];
  char name[80];
  char virtual_name[80];
  char maskword[80];
  char mask[80];
  char ip[80];
  char client[80];
  char client_number[80];
  char rest[80];
  char whole_line[1024];

  myfile = (FILE *)fopen(database_file, "r");
  i=0;
  //node arch32.cs.duke.edu	10.0.2.0	# client 851 
// arch31.cs.duke.edu	node vn5503	10.0.1.1 netmask 255.255.255.0 # client 503

  while (fgets(whole_line, 1024, myfile)!=NULL)
    {
      //        cout << whole_line << endl;
      if (whole_line[0]=='#' )
	{
	  continue;
	}

      //   if (fscanf(myfile, "%s %s %s # %s %s  %[^\n]s", &lead, &host, &ip, &client, &client_number, &rest)!=EOF)
      if (sscanf(whole_line, "%s %s %s %s %s %s # %s %s",&host, &lead, &virtual_name, &ip, &maskword, &mask, &client, &client_number)==8)
	{
//  	  cout <<lead <<" " << host  <<" = "<< ip <<endl;

	  if (!strcmp (lead,"node"))
	    {
	      strcpy( name, client);

	      if (strlen(client)==strlen("client"))
		{
		  strcat( name, client_number);
		}

	      //  	  int number =participants.inf(participants.get_item(atoi(name+6)));
	      node s =get_node_from_name(name);
	      int number =  inet_addr(ip);
	      IP_addresses[s] = number;
//  	      cout <<name <<" = " << hex << number <<endl;
	    }
	}
    }
  fclose(myfile);
      cout << "host imports done" << endl;

 }

void read_modelnet_hosts()	
 {
  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }

  char database_file[256];
  strcpy(database_file,mn_mdb);
  strcat(database_file,"/hosts");

  FILE *myfile;
  int i=0;
  char name[80];
  char ip[80];
  char rest[80];
  myfile = (FILE *)fopen(database_file, "r");
  i=0;
  while (fscanf(myfile, "%s %s # on %s", &name, &ip, &rest)!=EOF)
    {
      if (!strncmp (name,"client", 6))
	{
//  	  int number =participants.inf(participants.get_item(atoi(name+6)));
	  node s =get_node_from_name(name);
	  int number =  inet_addr(ip);
	  IP_addresses[s] = number;
//  	  cout <<name <<" = " << hex << number <<endl;
	}

    }
  fclose(myfile);
 }

void import_pipe_path()
{
  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }

  char database_file[256];
  strcpy(database_file,mn_mdb);
  strcat(database_file,"/out.path");
  FILE *myfile;

  int i=0;
  char route[80];
  char add[80];
  char from[80];
  char to[80];
  char command[80];
  char pipe[80];
  char whole_line[1024];
  char rest[1024];
  char remainder[1024];
//    cout << database_file << endl;
  myfile = (FILE *)fopen(database_file, "r");
  if ( !myfile)
    {
      error("there is no out.path in your MN_MDB");
    }

  i=0;

  while (fgets(whole_line, 1024, myfile)!=NULL)
    {
//        cout << whole_line << endl;
      if (whole_line[0]=='#' )
	{
	  continue;
	}

      //        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      int successful = 0;
      //route add 10.0.15.5 10.0.12.5 pipe 18386 5800 1712 39 11041 18545
      if ((successful =sscanf(whole_line, "%s %s %s %s %s %[^\n]s", 
			      &route, &add,  &from, &to, &command, &rest))!= 6)
	{
	  continue;
	}
      if (strncmp (route,"route", 5))
	{
	  continue;
	}
      node node_from, node_to;


      char fake_from[256], fake_to[256];
      if (!strncmp (from,"vn", 2))
	{
	  strcpy(fake_from,"client"); strcpy(fake_to, "client");
	  strcat(fake_from, from+2); strcat(fake_to, to+2);
	  node_from = get_node_from_name((fake_from));
	  node_to = get_node_from_name((fake_to));
	}
      else
	{
	  node_from = get_node_with_IP_address(inet_addr(from));
	  node_to = get_node_with_IP_address(inet_addr(to));
	}


      if (!node_from)
	{
	  error("can't find source node");
	  exit(5);
	}
      if (!node_to)
	{
	  error("can't find target node");
	  exit(5);
	}

//        cout <<  node_from << " "  <<  node_to << " " ;
      list<edge>* blank= new list<edge>;

//        cout << "route add " <<  fake_from << " "  <<  fake_to << " " ;
      while ((successful =sscanf(rest, "%s %[^\n]s", 
				 &pipe,  &remainder))>=1)
	{
	  int pipe_number =atoi(pipe);
	  //only debugging
//  	  if (pipe_number==2*(pipe_number/2))
//  	    {
//  	      pipe_number--;
//  	    }
//  	  cout <<  pipe_number << " "<< flush;// <<   edge_pipe << " ";

	  edge edge_pipe =get_edge_with_number(pipe_number);
	  if (!  edge_pipe )
	    {
	      cout <<  pipe_number << ": cannot get edge "<< flush;// <<   edge_pipe << " ";
	      exit (3);
	    }

	  blank->append(edge_pipe);
	  if (successful==1)
	    {
	      break;
	    }

	  strcpy(rest, remainder);
	}
      modelnet_overlay_matrix(node_from, node_to) =blank;
//       cout  << endl;

   }
  fclose (myfile);
//        cout << "pipe imports done" << endl;
}
void import_pipe_routes()
{
  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }

  char database_file[256];
  strcpy(database_file,mn_mdb);
  strcat(database_file,"/pipes.routes");
  FILE *myfile;

  int i=0;
  char from[80];
  char to[80];
  char command[80];
  char pipe[80];
  char whole_line[1024];
  char rest[1024];
  char remainder[1024];
//    cout << database_file << endl;
  myfile = (FILE *)fopen(database_file, "r");
  if ( !myfile)
    {
      error("there is no pipes.routes in your MN_MDB");
    }

  i=0;
  int route = 0;
  while (fgets(whole_line, 1024, myfile)!=NULL)
    {
//        cout << whole_line << endl;
      if (whole_line[0]=='#' )
	{
	  continue;
	}

      //        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %[^\n]s", 
			      &from, &to, &command, &rest))!= 4)
	{
	  continue;
	}
      node node_from = get_node_from_name(from);
      node node_to = get_node_from_name(to);
//        cout <<  node_from << " "  <<  node_to << " " ;
      list<edge>* blank= new list<edge>;

      cout <<  from << " "  <<  to << " " ;
      while ((successful =sscanf(rest, "%s %[^\n]s", 
				 &pipe,  &remainder))>=1)
	{
	  int pipe_number =atoi(pipe);
	  //only debugging
//  	  if (pipe_number==2*(pipe_number/2))
//  	    {
//  	      pipe_number--;
//  	    }
	  cout <<  pipe_number << " "<< flush;// <<   edge_pipe << " ";

	  edge edge_pipe =get_edge_with_number(pipe_number);
	  if (!  edge_pipe )
	    {
	      exit (3);
	    }

	  blank->append(edge_pipe);
	  if (successful==1)
	    {
	      break;
	    }

	  strcpy(rest, remainder);
	}
      modelnet_overlay_matrix(node_from, node_to) =blank;
      cout  << endl;

   }
  fclose (myfile);
      cout << "pipe imports done" << endl;
}
void fill_in_raitner_name(mn_graph& g, node v, char*modelnet_node_desc)
{
    Attributes & attributes = g[v];
    string name = attributes.s("label");
   char node_desc[80];

    if (name.length()> 79)
      {
	error("modelnet node description too long");
      }
    strcpy(node_desc,&name[0]);
   if (!strncmp (node_desc,"client", 6))
      {
	strcpy(modelnet_node_desc,"client");
	strcat(modelnet_node_desc,&node_desc [7]);
      }
    else
      {
	strcpy(modelnet_node_desc,node_desc);
      }
}


void read_example_model_emulators( char* model_file)
{
  FILE *myfile;
  char intro[80];
  char name[80];
  char whole_line[512];
  char*emulator_mark = "int_emul=";
  char*edge_mark = "int_idx=";
  myfile = (FILE *)fopen(model_file, "r");
  if (!myfile)
    {
      printf("nonexistent model file  %s\n", 
	     model_file);
      abort();
    }

//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
      char* whole_copy = strdup(whole_line);
      char* emul = strstr(whole_line, emulator_mark);
      if (!emul)
	{
	  continue;
	}
//        printf("emul: %s\n", emul);

      emul+=strlen(emulator_mark);
      emul++;//move past "
      char* ending = strchr(emul,'"');
      ending[0] = '\0';
      int emulator = atoi(emul);

      char* e = strstr(whole_copy, edge_mark);
      if (!e)
	{
	  continue;
	}
//        printf("e: %s\n", e);
      e+=strlen(edge_mark);
      e++;//move past "
      ending = strchr(e,'"');
      ending[0] = '\0';
      int edgei = atoi(e);
      edge ee = edges[edgei];
      emulators[ee] = emulator;
//        printf("emulator[%d]=%d\n", edgei, emulator);

      free(whole_copy);
    }
  fclose(myfile);
}
void read_example_model_emulator_mapping( char* model_file)
{
  struct hostent *phe;
  struct in_addr addr;

  FILE *myfile;
  char intro[80];
  char name[80];
  char whole_line[512];
  char*emulator_mark = "emul hostname=";
  char*index_mark = "int_idx=";
  myfile = (FILE *)fopen(model_file, "r");
  if (!myfile)
    {
      printf("nonexistent model file  %s\n", 
	     model_file);
      abort();
    }

//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
      char* whole_copy = strdup(whole_line);
      char* emul = strstr(whole_line, emulator_mark);
      if (!emul)
	{
	  continue;
	}
//        printf("emul: %s\n", emul);

      emul+=strlen(emulator_mark);
      emul++;//move past "
      char* ending = strchr(emul,'"');
      ending[0] = '\0';
      char*  emulator = NULL;

      if ((phe = gethostbyname(emul)) == 0)
	{
	  printf("Bad host lookup.\n");
	  abort();
	}

      memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));

      emulator = strdup(inet_ntoa (addr));

      char* e = strstr(whole_copy, index_mark);
      if (!e)
	{
	  continue;
	}
//        printf("e: %s\n", e);
      e+=strlen(index_mark);
      e++;//move past "
      ending = strchr(e,'"');
      ending[0] = '\0';
      int index = atoi(e);
      emulator_mapping[index] = emulator;
//        printf("emulator[%d]=%s/%s\n", index, emul,emulator);

      free(whole_copy);
    }
  fclose(myfile);
}

int read_modelnet_network( int escape_assignment = 0)
{
  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }
  
  char network_file[256];
  strcpy(network_file,mn_mdb);
  strcat(network_file,"/assigned_topo_with_clients_and_link_specs.gml");
  char model_file[256];
  strcpy(model_file,mn_mdb);
  strcat(model_file,"/example.model");
//    cout << "LEDA: preparing to raitnerparse ... " << flush;

  mn_graph g;
  raitner_parser gml(g);

  int ok = gml.parse(network_file);
  if (!ok) {
    cerr << "error raitnerparsing input file: \""<<network_file<<"\"\n";
    exit(1);
  }
//    cout << "LEDA: parsing done.  nodes:" << g. number_of_nodes()
//         << " edges "<<  g. number_of_edges() << endl << flush;;
  node sample;
  forall_nodes(sample,g)
    {
//        cout << "read node = "  << sample << endl << flush;
      node created =G. new_node();
      char modelnet_node_desc[80];
      fill_in_raitner_name(g, sample,modelnet_node_desc);
      if (!strncmp (modelnet_node_desc,"client", 6))
	{
//  	  cout << "read participant " << modelnet_node_desc <<endl;
	}
      G[created]=modelnet_node_desc;
    }
  edge some;
  forall_edges(some,g)
    {
      char from[80],to[80];
      fill_in_raitner_name(g, source( some),from);
      fill_in_raitner_name(g, target( some),to);
      edge created = G. new_edge(get_physical_node_from_name(from), 
				 get_physical_node_from_name(to));
      Attributes & attributes = g[some];
      double latency = (double)attributes.i("lat");
      G[created]= latency;
      int edge_pipe =attributes.i("pipe_nr");
      int edge_bandwidth =attributes.i("bw");
      double loss =attributes.d("plr");
//        cout << "read pipe " << edge_pipe << " latency " << latency << " bandwidth " << edge_bandwidth <<flush<<endl;
      if (loss > 1.0)
{
  error ("loss is > 1");
}

      bandwidth[created] = edge_bandwidth;//*(1- loss);//dmk
      original_delay[created] = latency;
      edge_number[created]= edge_pipe;
      loss_rate[created]= loss;
      edges[edge_pipe]=created;
    }

//    // Parse the label as the weight
//    gml_graph parser(G);
//    parser.add_edge_rule(get_edge_weight, gml_int, "lat");
//    parser.add_edge_rule(get_edge_pipe, gml_int, "pipe_nr");
//    parser.add_node_rule(get_node_label, gml_string, "label");
  
//    cout << "LEDA: preparing to parse ... " << flush;
//    bool parsed_ok = parser.parse(network_file);
//    cout << "LEDA: parsing done.  nodes:" << G. number_of_nodes()
//         << " edges "<<  G. number_of_edges() << endl << flush;;

//    if(!parsed_ok)
//      error("Failed to parse config file properly");
#if USE_LIFT
#if  USE_NODE_NUMBER
 node_number = node_array<int>(G,-1);
#endif
  nodes_array.resize( G. number_of_nodes()+ 1);
  participants_array.resize( G. number_of_nodes()+ 1);
//    participants_array.resize( G. number_of_nodes()+ 1);
//    edges.resize( G. number_of_edges()+ 1);
  int i=0;
  node v;
 forall_nodes(v,G)
   {
//       cout << "read node = "  << v << " i = " << i << endl << flush;
   nodes_array[i]=v;
#if  USE_NODE_NUMBER
   node_number[v]=i;
#endif
   i++;
   if (strncmp (G[v],"client", 6))
     {
       continue;
     }
//     cout << "read participant" << G[v]<<endl;
   participants.append(v);
   participants_array[atoi(&G[v][6])] =v;

   if (!strcmp(&G[v][6],"0"))
     {
       source_= v;
//     cout << "source is " << G[v]<<endl;
     }
   }
#endif
//    bandwidth = edge_array<int> (G);
//    loss_rate = edge_array<double> (G);
//    original_delay = edge_array<double> (G);
  is_up = edge_array<bool> (G, true);
  references = edge_array<int> (G, 0);
  physical_cost = edge_array <double> (G,0);
  initial_physical_cost = edge_array <double> (G,0);
  parent_died = node_array <double> (G, 0);
  reconnected = node_array <double> (G, 0);
  
  assign_edge_weights();

  assign_edge_cost();
//    assign_edge_bandwidth (2);

  edge e;

  forall_edges(e,G) 
  { 
    physical_edges.append (e);
  }

  import_pipe_path();


  read_example_model_emulators(model_file);
  read_example_model_emulator_mapping(model_file);

  if (escape_assignment)
    {
      return 0;
    }

#if USE_OVERLAY_MAP 
//    overlay_map=node_map2<edge>(G,nil);
#endif
//    import_pipe_routes();
//    read_modelnet_ip();
//    read_modelnet_hosts();

 forall(v,participants)
   {
     assign_additional_edges(v);
     node_status[v]= 0;
   }
  recompute_cost = 1;
  recompute_delay=1;
      is_physical = edge_array<int>(G,0);
      forall(e, physical_edges) 
       { 
	 is_physical [e] = 1;
       }
  recompute_if_needed();

}
#ifdef FAST_RANDOMIZATION

int new_in_epoch[10000];
int new_in_epoch_count[10000];
int new_in_epoch_c[10000];
typedef node_map<int> sequence_entry;
const int maximum_knowledge=2000;
sequence_entry knowledge[maximum_knowledge];
#endif


void replay_modelnet_file( char* replay_file)
{
  //matters start
  read_modelnet_network( );
  double time_index= 0;
  double dump_time_index= 0;
  double starting_time_index= 0;
  int starting_set = 0;
  
#ifdef FAST_RANDOMIZATION
  
  for (int i = 0; i < maximum_knowledge ;i++ )
    {
      knowledge[i]= node_map<int>(G,0);
    }
#endif
  
  FILE *myfile;
  int i=0;
  char intro[80];
  char name[80];
  char ip[80];
  char rest[512];
  char command[80];
  char status[80];
  char parent[80];
  char child[80];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  int max_sequence = 0;
  
  myfile = (FILE *)fopen(replay_file, "r");
  if (!myfile)
    {
      printf("nonexistent replay file  %s\n", 
	     replay_file);
      
    }
  
  node entry;
  //and make all all of them act dead, except root
  forall_nodes(entry,G) 
    {
      node_status[entry]= 1;
    }
  i=0;
//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
//        cout << whole_line << endl;
//        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      if (strncmp (whole_line,"Node", 4))
	{
	  continue;
	}
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %lf %d %s %[^\n]s", 
		 &intro, &ip, &name,  &time_when, &tid,&command, &rest))!= 7)
	{
	  continue;
	}
//  printf("got %d, %s %s %s %f %d : %s %s\n", 
//         successful,intro,ip, name,  time_when, command, rest);
//  printf("got %f %f %f\n", 
//         time_when,starting_time_index,dump_time_index);

      if (!strcmp (command,"REPLAY_EXPERIMENT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed;
	  if (sscanf(rest, "%s %d %d %d %d %d %lf %lf", 
		     &name, &root,&topology, &topology_nodes, &participant_nodes, &seed, &delay_bound, &SPT_ratio)!= 8)
	    continue;
	  cout
	    << "Running with delay bound "  << delay_bound<< endl
	    << "Running with delay ratio " << SPT_ratio<< endl;
	  delay_bound*= 1000; 
	  narada_nodes = participants.size();
	  topology_nodes = G.number_of_nodes();

	  node_status[get_node_from_name(name)]= 0;
	  int myip = 0;
	  sscanf(ip, "%x", &myip);
	  IP_addresses[get_node_from_name(name)]=myip;
  printf("initialize root %x\n",get_node_number(get_node_from_name(name))+ 1);
	  initialized = 1;
	}
      if (!strcmp (command,"REPLAY_INIT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed, delay_bound;
	  if (sscanf(rest, "%s %d", 
		     &name, & root)!= 2)
	    continue;
	  //make sure they don't act dead anymore
	  node_status[get_node_from_name(name)]= 0;
	  int myip = 0;
	  sscanf(ip, "%x", &myip);
	  IP_addresses[get_node_from_name(name)]=myip;
	  //dmk: ust to save time, commenting out
//  	  recompute_cost = 1;

	  printf("initialize  %x %s %x\n",get_node_number(get_node_from_name(name))+ 1, ip, myip);	
	}

//        if (!initialized)
//  	{
//  	  continue;
//  	}
      if (!starting_set)
	{
	  starting_set = 1;
	  starting_time_index =time_when;
	}

      if (!strcmp (command,"REPLAY_ADD")||
	  !strcmp (command,"REPLAY_REMOVE"))
	{
	  //  printf("got %s %s\n", 
	  //         command, rest);

	  if (sscanf(rest, "%s %s", 
		     &parent, &child)!= 2)
	    continue;
	  if (!strcmp (command,"REPLAY_REMOVE"))
	    {
	      printf("REPLAY_REMOVE %s %s\n", 
		     parent, child);
	      LEDA_remove_overlay_link(get_node_from_name(parent),
				       get_node_from_name(child));
	      //we dont want erroneous disconnected printouts
	      continue;
	    }
	  else
	    {
	      LEDA_add_overlay_link(get_node_from_name(parent),
				    get_node_from_name(child));
	      printf("REPLAY_ADD %s %s\n", 
		     parent, child);
	    }
	}
      else if(!strcmp (command,"REPLAY_HEIGHT"))
	{
	  double farthest = 0;
	  sscanf(rest, "%lf", 
		 &farthest);
	  my_ns_tree_farthest = farthest;
	  printf("SETTING TREE HEIGHT %lf \n",farthest);
	}
      else if(!strcmp (command,"REPLAY_DELAY"))
	{
	  double farthest = 0;
	  int sequence = 0;
	  sscanf(rest, "%d %lf", 
		 & sequence,&farthest);
	  modelnet_distance[get_node_from_name(name)] = 1000*farthest;
	  //  	  printf("SETTING TREE DELAY for %s to %lf \n",name,farthest);
	}
#ifdef FAST_RANDOMIZATION

      else if(!strcmp (command,"REPLAY_RANDOMIZATION"))
	{
	  int new_nodes = 0;
	  int new_nodes_c = 0;
	  int sequence = 0;
	  sscanf(rest, "%d %d %d", 
		 & sequence,&new_nodes,&new_nodes_c);
	  new_in_epoch[sequence] += new_nodes;
	  new_in_epoch_c[sequence] += new_nodes_c;
	  new_in_epoch_count[sequence] += 1;
	  knowledge[sequence][get_node_from_name(name)] = new_nodes_c;
//  	  printf("SETTING RANDOMIZATION for %s to %d \n",name,new_nodes_c);
	  if (sequence > max_sequence)
	    {
	      max_sequence = sequence;
	    }

	}
#endif

      else if(!strcmp (command,"REPLAY_PAYLOAD"))
	{
	  double farthest = 0;
	  int payload = 0;
	  int payload_nonprobes = 0;
	  int payload_probes = 0;
	  sscanf(rest, "%d %d %d", 
		 &payload, &payload_nonprobes, &payload_probes);
	  printf("%s %s (%f): payload = %d nonprobes = %d probes = %d\n", 
		 intro,ip,  time_when-starting_time_index, payload, payload_nonprobes, payload_probes);

	  //  	  printf("SETTING TREE DELAY for %s to %lf \n",name,farthest);
	}

      else if(!strcmp (command,"REPLAY_HEARD"))
	{
	  int address = 0;
	  int times = 0;
	  sscanf(rest, "%x %d", 
		 &address, &times);
	  printf("HEARD %x %d\n", 
		 address, times);
	}

      if (!initialized)
	{
	  continue; 
	}
      if (!dump_time_index||
	  time_when> dump_time_index+ 1)
	{
	  dump_time_index=time_when; 
#ifndef FAST_RANDOMIZATION
	  LEDA_dump_stats(time_when-starting_time_index, my_ns_tree_farthest);
#endif
	}
    }
  fclose(myfile);

#ifndef FAST_RANDOMIZATION
  LEDA_dump_tree(dump_time_index-starting_time_index,NULL);
#endif

#ifdef FAST_RANDOMIZATION
  printf("max is %d\n",max_sequence);
  for (int i = 0; i < max_sequence ;i++ )
    {
      int cumulative = 0;
      node v;
      int counter = 0;
      double cumulativesquare=0;

      forall(v,participants) 
	{
	  int max_knowledge = 0;
	  for (int j = 0; j <= i ;j++ )
	    {
	      if (knowledge[j][v]> max_knowledge)
		{
		  max_knowledge = knowledge[j][v];
		}
	    }
	  cumulative += max_knowledge;
	  cumulativesquare += (double)(max_knowledge*max_knowledge);
	}
      //  	  printf("RANDOMIZATION1 %d %lf %lf \n", i, new_in_epoch [i]/(double)new_in_epoch_count[i],new_in_epoch_c[i]/(double)new_in_epoch_count[i]);
      double variance =  (cumulativesquare-((double)cumulative*(double)cumulative/(double)participants.size()))/(double)(participants.size()-1);
      double deviation = sqrt( variance);
      double stderr = deviation / sqrt((double)participants.size() );
      double average = (double)cumulative/(double)participants.size();
      if (i%10==0)
	if (average-deviation < 0.0)
	  printf("KNOWLEDGE %d %lf %lf %lf\n", i, average, average+deviation, 0);
	else
	  printf("KNOWLEDGE %d %lf %lf %lf\n", i, average, average+deviation, average-deviation);
    }
  for (int i = 1; i < participants.size() ;i++ )
    {
      int cumulative = 0;
      node v;
      int counter = 0;
      forall(v,participants) 
	{
	  int max_knowledge = 0;
	  for (int j = 0; j < max_sequence  ;j++ )
	    {
	      if (knowledge[j][v]> max_knowledge)
		{
		  max_knowledge = knowledge[j][v];
		}
	    }
	  if (max_knowledge>=i)
	    {
	      cumulative ++;
	    }
	}
      printf("CUMULATIVE %d %lf \n", i,cumulative/(double)participants.size());
    }
#endif
  //matters end
}


void replay_generic_modelnet_file( char* replay_file)
{
  read_modelnet_network( );
  double time_index= 0;
  double dump_time_index= 0;
  double starting_time_index= 0;
  int starting_set = 0;

#ifdef FAST_RANDOMIZATION

  for (int i = 0; i < maximum_knowledge ;i++ )
    {
      knowledge[i]= node_map<int>(G,0);
    }
#endif

 FILE *myfile;
  int i=0;
  char intro[80];
  char name[80];
  char ip[80];
  char rest[512];
  char command[80];
  char status[80];
  char parent[80];
  char child[80];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  int max_sequence = 0;

  myfile = (FILE *)fopen(replay_file, "r");
  if (!myfile)
    {
      printf("nonexistent replay file  %s\n", 
	     replay_file); 
    }

  node entry;

  //and make all all of them act dead, except root
  forall_nodes(entry,G) 
    {
      node_status[entry]= 1;
    }
  i=0;

  while (fgets(whole_line, 512, myfile)!=NULL)
    {
      if (strncmp (whole_line,"Node", 4))
	{
	  continue;
	}
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %lf %d %s %[^\n]s", 
		 &intro, &ip, &name,  &time_when, &tid,&command, &rest))!= 7)
	{
	  continue;
	}

      if (!strcmp (command,"REPLAY_EXPERIMENT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed;
	  if (sscanf(rest, "%s %d %d %d %d %d", 
		     &name, &root, &topology, &topology_nodes, &participant_nodes, &seed)!= 6)
	    {
	      printf ("I do not understand this REPLAY_EXPERIMENT rest:  %s\n", rest);
	      exit (56);
	    }
	  narada_nodes = participants.size();
	  topology_nodes = G.number_of_nodes();
	  modelnet_distance[get_node_from_name(name)] = 0;
	  node_status[get_node_from_name(name)]= 0;
	  printf("initialize root %x\n",get_node_number(get_node_from_name(name))+ 1);
	  initialized = 1;
#ifdef NICE_VALIDATION
	  if (participants.size() == 32)
	{
	  source_=get_node_from_name("client6");
	}
else if (participants.size() == 64)
	{
	  source_=get_node_from_name("client12");
	}
else if (participants.size() == 96)
	{
	  source_=get_node_from_name("client18");
	}
	  modelnet_distance[source_] = 0;
#endif
	}
      if (!strcmp (command,"REPLAY_INIT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed, delay_bound;
	  if (sscanf(rest, "%s %d", 
		     &name, & root)!= 2)
	    {
	      printf ("I do not understand this REPLAY_INIT rest:  %s\n", rest);
	      exit (57);
	    }
	  //make sure they don't act dead anymore
	  node_status[get_node_from_name(name)]= 0;
	  //dmk: ust to save time, commenting out
//  	  recompute_cost = 1;
	  modelnet_distance[get_node_from_name(name)] = 0;

	  printf("initialize  %x\n",get_node_number(get_node_from_name(name))+ 1);	
	}

      if (!starting_set)
	{
	  starting_set = 1;
	  starting_time_index =time_when;
	}

      if (!strcmp (command,"REPLAY_ADD")||
	  !strcmp (command,"REPLAY_REMOVE"))
	{
	  if (sscanf(rest, "%s %s", 
		     &parent, &child)!= 2)
	    {
	      printf ("I do not understand this REPLAY_ADD/REMOVE rest:  %s\n", rest);
	      exit (58);
	    }
	  if (!strcmp (command,"REPLAY_REMOVE"))
	    {
	      printf("REPLAY_REMOVE %s %s\n", 
		     parent, child);
	      LEDA_remove_overlay_link(get_node_from_name(parent),
				       get_node_from_name(child));
	      //we dont want erroneous disconnected printouts
	      continue;
	    }
	  else
	    {
	      LEDA_add_overlay_link(get_node_from_name(parent),
				    get_node_from_name(child));
	      printf("REPLAY_ADD %s %s\n", 
		     parent, child);
	    }
	}
      else if(!strcmp (command,"REPLAY_DELAY"))
	{
	  double mydelay = 0;
	  char from[80];
	  if (sscanf(rest, "%s %lf", 
		     & from, &mydelay) != 2)
	    {
	      printf ("I do not understand this REPLAY_DELAY rest:  %s\n", rest);
	      exit (59);
	    }
	  modelnet_distance[get_node_from_name(name)] = 1000*mydelay/2 + modelnet_distance[get_node_from_name(from)];

// 	  printf("%s: my delay is %lf and my papas (%s) is %lf total %f\n", name, 1000*mydelay/2, from, modelnet_distance[get_node_from_name(from)], modelnet_distance[get_node_from_name(name)]);
	  //	  modelnet_distance[get_node_from_name(name)] = 10;
	}
#ifdef FAST_RANDOMIZATION

      else if(!strcmp (command,"REPLAY_RANDOMIZATION"))
	{
	  int new_nodes = 0;
	  int new_nodes_c = 0;
	  int sequence = 0;
	  sscanf(rest, "%d %d %d", 
		 & sequence,&new_nodes,&new_nodes_c);
	  new_in_epoch[sequence] += new_nodes;
	  new_in_epoch_c[sequence] += new_nodes_c;
	  new_in_epoch_count[sequence] += 1;
	  knowledge[sequence][get_node_from_name(name)] = new_nodes_c;
//  	  printf("SETTING RANDOMIZATION for %s to %d \n",name,new_nodes_c);
	  if (sequence > max_sequence)
	    {
	      max_sequence = sequence;
	    }

	}
#endif

      else if(!strcmp (command,"REPLAY_PAYLOAD"))
	{
	  double farthest = 0;
	  int payload = 0;
	  if (sscanf(rest, "%d", 
		     &payload) != 1)
	    {
	      printf ("I do not understand this REPLAY_PAYLOAD rest:  %s\n", rest);
	      exit (58);
	    }
	  printf("%s %s (%f): payload = %d \n", 
		 intro,ip,  time_when-starting_time_index, payload);

	}

      if (!initialized)
	{
	  continue; 
	}
      if (!dump_time_index||
	  time_when> dump_time_index+ 1)
	{
	  dump_time_index=time_when;
#ifndef GENERIC_FAST_RANDOMIZATION
	  LEDA_dump_stats(time_when-starting_time_index, -1.0);
#endif
	}
    }
  fclose(myfile);
  LEDA_dump_tree(dump_time_index-starting_time_index,NULL);

#ifdef GENERIC_FAST_RANDOMIZATION
  for (int i = 0; i < max_sequence ;i++ )
    {
      int cumulative = 0;
      node v;
      int counter = 0;
      forall(v,participants) 
	{
	  int max_knowledge = 0;
	  for (int j = 0; j <= i ;j++ )
	    {
	      if (knowledge[j][v]> max_knowledge)
		{
		  max_knowledge = knowledge[j][v];
		}
	    }
	  cumulative += max_knowledge;
	}
      printf("KNOWLEDGE %d %lf \n", i,(double)cumulative/(double)participants.size());
	
    }
  for (int i = 1; i < participants.size() ;i++ )
    {
      int cumulative = 0;
      node v;
      int counter = 0;
      forall(v,participants) 
	{
	  int max_knowledge = 0;
	  for (int j = 0; j < max_sequence  ;j++ )
	    {
	      if (knowledge[j][v]> max_knowledge)
		{
		  max_knowledge = knowledge[j][v];
		}
	    }
	  if (max_knowledge>=i)
	    {
	      cumulative ++;
	    }
	}
      printf("CUMULATIVE %d %lf \n", i,cumulative/(double)participants.size());
    }
#endif
}
const int SITES = 8;
const int NICE_CLIENTS_32 = 32;
const int NICE_CLIENTS_64 = 64;
const int NICE_CLIENTS_96 = 96;
int pairs_matrix[3][SITES]={{4, 2, 6, 8, 4, 4, 2, 2},
			    {8, 4, 12, 16, 8, 8, 4, 4},
			    {12, 6, 18, 24, 12, 12, 6, 6}};
int pairs[SITES];
double stretch_sum[SITES];
double latency_sum[SITES];
double stretch_sum_squared[SITES];
double latency_sum_squared[SITES];
int degrees[NICE_CLIENTS_96];

int  get_nice_site(int client)
{
  int counter = 0;
  int sum = pairs[counter];
  while (client > (sum-1) && counter < SITES)
    {
      counter ++;
      sum += pairs[counter];
    }
 return counter;
}

void  nice_validation()
{
  int choice = (participants.size()/32)-1;
  for (int counter = 0; counter < SITES ;counter++ )
    {
      pairs[counter] = pairs_matrix[choice][counter];
    }

  cout << "source " << &G[source_][0] <<  flush<< endl;
//    return;
  node v;
  forall(v,participants) 
    {
//        cout << "checking distance to participants " << &G[v][0] <<  flush<< endl;

      double overlay_delay = modelnet_distance[v];
      double network_delay = 0;
      double client_stretch = 1;
      if (v != source_)
	{
	  network_delay = overlay_true_distance(source_,v);
	  //account for 0 propagation delay by assigning something reasonable on Fast Ethernet
	  if (network_delay == 0)
	    {
	      network_delay = 0.4;
	    }

	  client_stretch = overlay_delay/network_delay;
	}
//        printf("OVERLAY_DELAY %s %lf \n", &G[v][0],overlay_delay);
      printf("STRETCH %s %lf %lf %lf \n", &G[v][0], client_stretch ,overlay_delay, network_delay);
      int site = get_nice_site(atoi(&G[v][6]));
printf("site for %d: %d\n", atoi(&G[v][6]), site);

      stretch_sum[site] += client_stretch;
      latency_sum[site] += overlay_delay;
      stretch_sum_squared[site] += client_stretch*client_stretch;
      latency_sum_squared[site] += overlay_delay*overlay_delay;
    }
  for (int counter = 0; counter < SITES ;counter++ )
    {
      double average = stretch_sum[counter]/pairs[counter];
      double deviation = sqrt((stretch_sum_squared[counter]*pairs[counter] - 
			       (stretch_sum[counter]*stretch_sum[counter]))/
			      ( pairs[counter]*( pairs[counter] - 1))			   );
      double ci95 = deviation*1.960/(sqrt(participants.size()));
      printf("STRETCH_SITE %d %lf %lf %lf \n", counter, average,  average-ci95,  average+ ci95
	     );
    }

  for (int counter = 0; counter < SITES ;counter++ )
    {
      double average =  latency_sum[counter]/pairs[counter];
      double deviation = sqrt((latency_sum_squared[counter]* pairs[counter] -
			       (latency_sum[counter]*latency_sum[counter]))/
			      ( pairs[counter]*( pairs[counter] - 1))			   );
      double ci95 = deviation*1.960/(sqrt(participants.size()));

      printf("LATENCY_SITE %d %lf %lf %lf \n", counter, average,  average-ci95,  average+ ci95
	     );
    }
  edge sample;
  forall( sample, overlay_edge_list)
    {
      degrees[atoi(&G[source( sample)][6])]++;
    }
  for (int counter = 0; counter < participants.size() ;counter++ )
    {
      printf(" DEGREE %d %d\n", counter, degrees[counter ]);
    }

}


void change_physical_delays (int total_time,
			     int initial_time,
			     int spacing,
			     double links, 
			     double magnitude)
{
  
  printf("# total %d  initial  %d  spacing %d  percentage_links %lf  magnitude %lf \n", total_time,initial_time, spacing, links, magnitude);
  printf("sleep %d \n",initial_time);

  int seconds = initial_time;
  int count =(int)(links*G. number_of_edges());

  while (seconds < total_time)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");

      for (int i = 0; i < count ;i++ )
	{
	  //pipes are numbered starting with one
	  int link = random_integer2(G. number_of_edges()-1)+1;
	  edge  e = edges[link];
	  double previous =G[e];
	  double value = (int)(drand48()*magnitude*original_delay[e]);
	  G[e]+=value;
	  edge r = reverse_edge (e);
	  previous =G[r];
	  G[r]+=value;
	  printf("pipe config %4d bw %dKb/s delay %dms plr %lf \n",
		 edge_number[e], bandwidth[e],(int)G[e], loss_rate[e]);
	  printf("pipe config %4d bw %dKb/s delay %dms plr %lf \n",
		 edge_number[r], bandwidth[r],(int)G[r], loss_rate[r]);
//  	  printf(" changing physical link delay for %d from %lf to %lf\n",
//  		link,previous,G[e]); 

	}
printf("EOF\n");
printf("/usr/research/proj/modelnet/bin/modelmod /var/tmp/mn.$$\n");
printf("rm -f /var/tmp/mn.$$\n");

      printf("sleep %d \n",spacing);
      seconds+=spacing;
    }

}

void change_physical_bandwidth (int total_time,
				int initial_time,
				int spacing,
				int length,
				double percentage, 
				double magnitude_minimum,
				double magnitude_maximum
)
{
  
  printf("# total %d  initial  %d  spacing %d  length %d percentage %lf  magnitude_minimum %lf magnitude_maximum %lf \n", total_time,initial_time, spacing, length, percentage, magnitude_minimum, magnitude_maximum);
  printf("sleep %d \n",initial_time);
  if (magnitude_minimum > magnitude_maximum)
    {
      printf("magnitude_minimum: %d\n", magnitude_minimum);
      printf("magnitude_maximum: %d\n", magnitude_maximum);
      exit(1);
    }
  node_array< double> accumulated(G,0);
  node_array< double> current(G,0);
  node_array< double> normal(G,0);
  node_array< double> last_change(G,0);
  int seconds = initial_time;

  node sample;
  forall (sample, participants)
    {
      if (sample == source_)
	{
	  continue;
	}

      list < edge>  reverse_route = overlay_edges ( source_, sample);
      edge r  = reverse_route.pop_back();

      double previous_reverse = bandwidth[r];

      normal[ sample] = bandwidth[r];
      current[ sample] = bandwidth[r];
      printf("client_bw %s %d %d %d \n", &G[sample][6], seconds, (int)current[ sample], (int)current[ sample]);
    }

  int count =(int)( percentage*participants.size());
  while (seconds < total_time)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      list < edge> altered;
      list < node> nodes_altered;
//  #define  FIFTY_FIFTY_experiment 1
#if FIFTY_FIFTY_experiment
      for (int participant = 3; participant < participants.size() ;participant++ )
	{
	  if ((participant%2 == 0) || participant == 0 || participant == 1)
	    {
	      continue;
	    }
	  node chosen = participants_array[ participant];
//  	  printf("participant: %d\n", participant);
#else
      for (int i = 0; i < count ;i++ )
	{
	  //pipes are numbered starting with one
	  int  participant = 1;
	  node chosen = participants_array[ participant];
	  while (participant==0 || participant==1 ||nodes_altered.rank( chosen) )
	    {
	  printf("participant: %d\n", participant);
	      participant = random_integer2( participants.size()-1) + 1;
chosen = participants_array[ participant];
	    }
#endif
	  printf("cparticipant: %d\n", participant);

	  list < edge>  forward_route = overlay_edges ( chosen, source_);
	  list < edge>  reverse_route = overlay_edges ( source_, chosen);
	  int modified = 0;
	  edge e;
	  while (1)
	    {
	      e = forward_route. pop_front();
//  	      edge r = reverse_edge (e);
	      edge r  = reverse_route.pop_back();
	      altered.  push_back(e);
	      altered.  push_back(r);
	      nodes_altered.  push_back(chosen);

	      if ( edges[edge_number[e]] != e)
		{
		  printf(" discrepancy edge_number[e]: %d\n", edge_number[e]);
		  exit( 1);
		}
	      if ( edges[edge_number[r]] != r)
		{
		  printf(" discrepancy edge_number[r]: %d\n", edge_number[r]);
		  exit( 1);
		}

	      double previous_direct = bandwidth[e];
	      double previous_reverse = bandwidth[r];
	      double multiplier = magnitude_minimum + drand48()*(magnitude_maximum - magnitude_minimum);
	      int  direct_value = (int)(multiplier*previous_direct);
	      int  reverse_value = (int)(multiplier* previous_reverse);
	      int direct_delay = (int)G[e];
	      int reverse_delay = (int)G[r];
	      double direct_loss = loss_rate[e];
	      double reverse_loss = loss_rate[r];
	      if (multiplier ==0)
		{
		  direct_value = (int)previous_direct;
		  reverse_value = (int)previous_reverse;
		  direct_loss = 1.0;
		  reverse_loss = 1.0;
		}

//  	      if (direct_delay == 0) direct_delay=1;
//  	      if (reverse_delay == 0) reverse_delay=1;
	      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		     edge_number[e], (int)direct_value, direct_delay, direct_loss, emulator_mapping[emulators[e]]);
	      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		     edge_number[r], (int)reverse_value,reverse_delay, reverse_loss, emulator_mapping[emulators[r]]);
	      //  	  printf(" changing physical link delay for %d from %lf to %lf\n",
	      //  		link,previous,G[e]);

	      double elapsed = seconds - last_change[chosen];
	      accumulated[chosen] += elapsed*current[chosen];
	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
	      current[chosen]= reverse_value;
	      last_change[chosen]= seconds;
	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);

	      modified ++;
	      if (modified == 1)
		{
		  //  assume we're we are using INET topology
		  break;
		}
	    }
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");

      int initial = length;
      printf("sleep %d \n",length);
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      for (int participant = 1; participant < participants.size() ;participant++ )
	{
	  node chosen = participants_array[ participant];
	  if (!nodes_altered.rank( chosen))
	    {
  	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);

	    }
	}
//        for (int i = seconds; i < seconds + length ;i++ )
//  	{
//  	  node chosen ;
//  	  forall (chosen, nodes_altered)
//  	    {
//    	      printf("client_bw %s %d %d %d \n", &G[chosen][6], i, (int)current[ chosen], (int)current[ chosen]);
//  	    }
//  	}

      seconds += length;
      node chosen ;
      forall (chosen, nodes_altered)
	{
	  double elapsed = seconds - last_change[chosen];
	  accumulated[chosen] += elapsed*current[chosen];
	  printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
	  current[chosen]= normal[chosen];
	  last_change[chosen]= seconds;
	  printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
	}
      edge e;
      forall(e, altered)
	{
	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 edge_number[e], (int) bandwidth[e], G[e], loss_rate[e], emulator_mapping[emulators[e]]);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");
      printf("sleep %d \n",spacing - length);
      for (int participant = 1; participant < participants.size() ;participant++ )
	{
	  node chosen = participants_array[ participant];
	  if (!nodes_altered.rank( chosen))
	    {
  	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);

	    }
	}
//        for (int i = seconds; i < seconds + length ;i++ )
//  	{
//  	  node chosen ;
//  	  forall (chosen, nodes_altered)
//  	    {
//    	      printf("client_bw %s %d %d %d \n", &G[chosen][6], i, (int)current[ chosen], (int)current[ chosen]);
//  	    }
//  	}

      seconds += (spacing - length);

    }
  node chosen ;
  forall (chosen, participants)
    {
      if (chosen == source_)
	{
	  continue;
	}
      double elapsed = seconds - last_change[chosen];
      accumulated[chosen] += elapsed*current[chosen];
      double average = accumulated[chosen] / (double)seconds;
      printf("average: %f %s\n", average, &G[chosen][6]); //, &G[chosen]
    }

}

void check_complete (int seconds,
		     int file_size_in_bytes,
		     node_array< double>& accumulated,
		     node_array< double>& current,
		     node_array< double>& completed, 
		     double source_downstream
		     )
{
  node chosen;
  forall (chosen, participants)
    {
      if (chosen == source_)
	{
	  continue;
	}
      accumulated[chosen] += current[chosen]*1;//(1 second granularity)
//        if (!completed[chosen]) printf("%d accumulated[%s]: %f current %f\n", seconds, &G[chosen][6],accumulated[chosen], current[chosen]);

      //bandwidth is in kbps
      if (!completed[chosen] && accumulated[chosen]*1000.0 >= file_size_in_bytes *8)
	{
	  int possible = (int)(file_size_in_bytes *8/(source_downstream*1000.0));
	  if (seconds < possible)
	  {
	  completed[chosen]=possible;
	    
	  }
	  else
	  {
	    completed[chosen]=seconds;
	  }
	}
    }
}


int BANDWIDTH_CHANGE_step = 5;
 double MyPI = 3.14159265358979323846;

 void change_physical_edge_bandwidth_wave (int initial_time,
					   int total_time,
					   int spacing_minimum,
					   int spacing_maximum,
					   int length_minimum,
					   int length_maximum,
					   double percentage_minimum, 
					   double percentage_maximum, 
					   double magnitude_minimum,
					   double magnitude_maximum,
					   int file_size_in_bytes,
					   node_array< double>& accumulated,
					   node_array< double>& current,
					   node_array< double>& normal,
					   node_array< double>& completed,
					   node_array< double>& multipliers,
					   double source_downstream
					   )
   {
     int seconds = initial_time;

     node sample;
     node_array< double> periods(G,0);
     node_array< double> phases(G,0);

     printf("cat > /var/tmp/mn.$$ << EOF\n");
     list < edge> altered;
     list < node> nodes_altered;
     double percentage =  percentage_minimum + drand48()*(percentage_maximum - percentage_minimum);
     int count =(int)( percentage*participants.size());
     for (int i = 0; i < count ;i++ )
       {
	 //pipes are numbered starting with one
	 int  participant = 1;
	 node chosen = participants_array[ participant];
	 while (participant==0 || participant==1 ||nodes_altered.rank( chosen) )
	   {
	     printf("participant: %d\n", participant);
	     participant = random_integer2( participants.size()-1) + 1;
	     chosen = participants_array[ participant];
	   }
	 printf("cparticipant: %d\n", participant);

	 nodes_altered.  push_back(chosen);
	 double multiplier = magnitude_minimum + drand48()*(magnitude_maximum - magnitude_minimum);
	 multipliers[chosen]=1-multiplier;
	 int length =  length_minimum + (int)(drand48()*(length_maximum - length_minimum));
	 phases[chosen]=length;

	 int spacing =  spacing_minimum + (int)(drand48()*(spacing_maximum - spacing_minimum));
	 periods[chosen]=spacing;       }
      
     printf("EOF\n");
     printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
     printf("rm -f /var/tmp/mn.$$\n");
     for (int counter = initial_time; counter < total_time; counter+= BANDWIDTH_CHANGE_step)
       {
	 printf("cat > /var/tmp/mn.$$ << EOF\n");
	 node chosen ;
	 forall (chosen, nodes_altered)
	   {
	     list < edge>  forward_route = overlay_edges ( chosen, source_);
	     list < edge>  reverse_route = overlay_edges ( source_, chosen);
	     edge e = forward_route. pop_front();
	     edge r  = reverse_route.pop_back();
	     if (!altered.  rank(e)) altered.  push_back(e);
	     if (!altered.  rank(r)) altered.  push_back(r);

	     double previous_direct = bandwidth[e];
	     double previous_reverse = bandwidth[r];
	     double multiplier = multipliers[chosen];

	     int direct_value = (int)previous_direct + (int)(multiplier*previous_direct*sin(phases[chosen] + 2*MyPI*(counter - initial_time)/periods[chosen]*1.0));
	     int reverse_value = (int)previous_reverse + (int)(multiplier*previous_reverse*sin(phases[chosen] + 2*MyPI*(counter - initial_time)/periods[chosen]*1.0));
	     int direct_delay = (int)G[e];
	     int reverse_delay = (int)G[r];
	     double direct_loss = loss_rate[e];
	     double reverse_loss = loss_rate[r];

	     current[chosen]= reverse_value;

	     if (direct_value ==0)
	       {
		 direct_value = (int)previous_direct;
		 direct_loss = 1.0;
	       }

	     if (reverse_value ==0)
	       {
		 reverse_value = (int)previous_reverse;
		 reverse_loss = 1.0;
	       }

	     printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		    edge_number[e], (int)direct_value, direct_delay, direct_loss, emulator_mapping[emulators[e]]);
	     printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		    edge_number[r], (int)reverse_value,reverse_delay, reverse_loss, emulator_mapping[emulators[r]]);

	     //skip client_bw for wave
	   }
	 for (int timeline = counter; timeline < counter + BANDWIDTH_CHANGE_step; timeline++ )
	   {
	     check_complete(timeline,  file_size_in_bytes, accumulated, current, completed, source_downstream);
	   }
	 printf("EOF\n");
	 printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
	 printf("rm -f /var/tmp/mn.$$\n");
	 printf("sleep %d \n", BANDWIDTH_CHANGE_step);
       }
     printf("cat > /var/tmp/mn.$$ << EOF\n");
     edge e;
     forall(e, altered)
       {
	      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		     edge_number[e], (int) bandwidth[e], (int)G[e], loss_rate[e], emulator_mapping[emulators[e]]);
       }
     printf("EOF\n");
     printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
     printf("rm -f /var/tmp/mn.$$\n");
   }

enum  BANDWIDTH_CHANGE {
  BANDWIDTH_CHANGE_pulse= 0,
  BANDWIDTH_CHANGE_wave_bottom_synchronized= 1,
  BANDWIDTH_CHANGE_wave= 2
};

//note flipped initial and total   compared to  change_physical_bandwidth
void change_physical_edge_bandwidth_generalized (int initial_time,
						 int total_time,
						 int spacing_minimum,
						 int spacing_maximum,
						 int length_minimum,
						 int length_maximum,
						 double percentage_minimum, 
						 double percentage_maximum, 
						 double magnitude_minimum,
						 double magnitude_maximum,
						 BANDWIDTH_CHANGE type = BANDWIDTH_CHANGE_pulse,
						 int file_size_in_bytes = 0
)
{
  
  printf("# total %d  initial  %d  spacingminimum %d spacing_maximum %d  length_minimum  %d length_maximum %d percentage_minimum %lf  percentage_maximum %lf  magnitude_minimum %lf magnitude_maximum %lf type %d\n", total_time,initial_time, spacing_minimum, spacing_maximum, length_minimum, length_maximum, percentage_minimum, percentage_maximum, magnitude_minimum, magnitude_maximum, type);
  printf("sleep %d \n",initial_time);
  if (magnitude_minimum > magnitude_maximum)
    {
      printf("magnitude_minimum: %d\n", magnitude_minimum);
      printf("magnitude_maximum: %d\n", magnitude_maximum);
      exit(1);
    }
  if (spacing_minimum > spacing_maximum)
    {
      printf("spacing_minimum: %d\n", spacing_minimum);
      printf("spacing_maximum: %d\n", spacing_maximum);
      exit(1);
    }
  if (type != BANDWIDTH_CHANGE_wave && length_minimum > length_maximum)
    {
      printf("length_minimum: %d\n", length_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  if (type != BANDWIDTH_CHANGE_wave && spacing_minimum < length_maximum)
    {
      printf(" spacing_minimum: %d\n",  spacing_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  node_array< double> accumulated(G,0);
  node_array< double> current(G,0);
  node_array< double> normal(G,0);
  node_array< double> last_change(G,0);
  node_array< double> completed(G,0);
  node_array< double> multipliers(G,0);
  int seconds = initial_time;

  node sample;
  double source_downstream = 0;
  forall (sample, participants)
    {
      if (sample == source_)
	{
	  continue;
	}
      if (source_downstream == 0.0)
	{
	  list < edge>  direct_route = overlay_edges ( source_, sample);
	  edge e  = direct_route.pop_front();
	  source_downstream=bandwidth[e];
	}

      list < edge>  reverse_route = overlay_edges ( source_, sample);
      edge r  = reverse_route.pop_back();
      
      double previous_reverse = bandwidth[r];

      normal[ sample] = bandwidth[r];
      current[ sample] = bandwidth[r];
      printf("client_bw %s %d %d %d \n", &G[sample][6], seconds, (int)current[ sample], (int)current[ sample]);
    }

  for (int counter = 0; counter <initial_time ;counter++ )
    {
      check_complete(counter, file_size_in_bytes, accumulated, current, completed, source_downstream);
    }
  if (type == BANDWIDTH_CHANGE_wave)
    {
      change_physical_edge_bandwidth_wave (initial_time,
					   total_time,
					   spacing_minimum,
					   spacing_maximum,
					   length_minimum,
					   length_maximum,
					   percentage_minimum, 
					   percentage_maximum, 
					   magnitude_minimum,
					   magnitude_maximum,
					   file_size_in_bytes,
					   accumulated,
					   current,
					   normal,
					   completed,
					   multipliers,
					   source_downstream
					   );
	}
  else
    {
      while (seconds < total_time)
	{
	  printf("cat > /var/tmp/mn.$$ << EOF\n");
	  list < edge> altered;
	  list < node> nodes_altered;
	  double percentage =  percentage_minimum + drand48()*(percentage_maximum - percentage_minimum);
	  int length =  length_minimum + (int)(drand48()*(length_maximum - length_minimum));
	  int spacing =  spacing_minimum + (int)(drand48()*(spacing_maximum - spacing_minimum));
	  int count =(int)( percentage*participants.size());
	  for (int i = 0; i < count ;i++ )
	    {
	      //pipes are numbered starting with one
	      int  participant = 1;
	      node chosen = participants_array[ participant];
	      while (participant==0 || participant==1 ||nodes_altered.rank( chosen) )
		{
		  printf("participant: %d\n", participant);
		  participant = random_integer2( participants.size()-1) + 1;
		  chosen = participants_array[ participant];
		}
	      printf("cparticipant: %d\n", participant);

	      nodes_altered.  push_back(chosen);
	      double multiplier = magnitude_minimum + drand48()*(magnitude_maximum - magnitude_minimum);
	      multipliers[chosen]=multiplier;

	      if (type == BANDWIDTH_CHANGE_pulse)
		{
		  list < edge>  forward_route = overlay_edges ( chosen, source_);
		  list < edge>  reverse_route = overlay_edges ( source_, chosen);
		  edge e = forward_route. pop_front();
		  edge r  = reverse_route.pop_back();
		  altered.  push_back(e);
		  altered.  push_back(r);

		  double previous_direct = bandwidth[e];
		  double previous_reverse = bandwidth[r];
		  double multiplier = multipliers[chosen];
		  int  direct_value = (int)(multiplier*previous_direct);
		  int  reverse_value = (int)(multiplier* previous_reverse);
		  int direct_delay = (int)G[e];
		  int reverse_delay = (int)G[r];
		  double direct_loss = loss_rate[e];
		  double reverse_loss = loss_rate[r];
		  //for modelnet, bandwidth of 0 means infinite bandwidth !? therefore, we set the loss rate to 100 percent
		  if (multiplier ==0)
		    {
		      direct_value = 1;
		      reverse_value = 1;
		      direct_loss = 1.0;
		      reverse_loss = 1.0;
		    }

		  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
			 edge_number[e], (int)direct_value, direct_delay, direct_loss, emulator_mapping[emulators[e]]);
		  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
			 edge_number[r], (int)reverse_value,reverse_delay, reverse_loss, emulator_mapping[emulators[r]]);

		  double elapsed = seconds - last_change[chosen];
		  //  	      accumulated[chosen] += elapsed*current[chosen];
		  printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
		  current[chosen]=  (int)(multiplier* previous_reverse);
		  last_change[chosen]= seconds;
		  printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
		}
	    }
	  printf("EOF\n");
	  printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
	  printf("rm -f /var/tmp/mn.$$\n");

	  if (type == BANDWIDTH_CHANGE_wave_bottom_synchronized)
	    {
	      for (int counter = seconds; counter <  seconds+length ;counter+= BANDWIDTH_CHANGE_step)
		{
		  printf("sleep %d \n",BANDWIDTH_CHANGE_step);
		  printf("cat > /var/tmp/mn.$$ << EOF\n");
		  node chosen ;
		  forall (chosen, nodes_altered)
		    {
		      list < edge>  forward_route = overlay_edges ( chosen, source_);
		      list < edge>  reverse_route = overlay_edges ( source_, chosen);
		      edge e = forward_route. pop_front();
		      edge r  = reverse_route.pop_back();
		      if (!altered.  rank(e)) altered.  push_back(e);
		      if (!altered.  rank(r)) altered.  push_back(r);

		      double previous_direct = bandwidth[e];
		      double previous_reverse = bandwidth[r];
		      double multiplier = multipliers[chosen];
		      double high_direct = bandwidth[e];
		      double high_reverse = bandwidth[r];
		      double low_direct = bandwidth[e]*multiplier;
		      double low_reverse = bandwidth[r]*multiplier;
		      int direct_value = (int)high_direct - (int)((high_direct-low_direct)*sin(MyPI*(counter - seconds)/length*1.0));
		      int reverse_value = (int)high_reverse - (int)((high_reverse-low_reverse)*sin(MyPI*(counter - seconds)/length*1.0));
		      int direct_delay = (int)G[e];
		      int reverse_delay = (int)G[r];
		      double direct_loss = loss_rate[e];
		      double reverse_loss = loss_rate[r];
		      current[chosen]= reverse_value;

		      if (direct_value ==0)
			{
			  direct_value = (int)previous_direct;
			  direct_loss = 1.0;
			}

		      if (reverse_value ==0)
			{
			  reverse_value = (int)previous_reverse;
			  reverse_loss = 1.0;
			}

		      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
			     edge_number[e], (int)direct_value, direct_delay, direct_loss, emulator_mapping[emulators[e]]);
		      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
			     edge_number[r], (int)reverse_value,reverse_delay, reverse_loss, emulator_mapping[emulators[r]]);

		      //skip client_bw for wave
		    }
		  for (int timeline = counter; timeline < counter + BANDWIDTH_CHANGE_step; timeline++ )
		    {
		      check_complete(timeline,  file_size_in_bytes, accumulated, current, completed, source_downstream);
		    }
		  printf("EOF\n");
		  printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
		  printf("rm -f /var/tmp/mn.$$\n");
		}
	      int remaining = length - ((int)length/BANDWIDTH_CHANGE_step)*BANDWIDTH_CHANGE_step;
	      if (remaining > 0)
		{
		  printf("sleep %d \n", remaining);
		  printf("cat > /var/tmp/mn.$$ << EOF\n");
		  printf("EOF\n");
		  printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
		  printf("rm -f /var/tmp/mn.$$\n");
		}
	    }
	  else if (type == BANDWIDTH_CHANGE_pulse)
	    {
	      printf("sleep %d \n",length);
	      printf("cat > /var/tmp/mn.$$ << EOF\n");
	      for (int participant = 1; participant < participants.size() ;participant++ )
		{
		  node chosen = participants_array[ participant];
		  if (!nodes_altered.rank( chosen))
		    {
		      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);

		    }
		}

	      for (int counter = seconds; counter < seconds+length ;counter++ )
		{
		  check_complete(counter,  file_size_in_bytes, accumulated, current, completed, source_downstream);
		}
	    }
	  seconds += length;

	  node chosen ;
	  forall (chosen, nodes_altered)
	    {
	      double elapsed = seconds - last_change[chosen];
	      //  	  accumulated[chosen] += elapsed*current[chosen];
	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
	      current[chosen]= normal[chosen];
	      last_change[chosen]= seconds;
	      printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);
	    }
	  edge e;
	  forall(e, altered)
	    {
	      int emulator = emulators[e];
//  	      printf("emulator: %d\n", emulator);

	      char* mapping = emulator_mapping[emulator];
//  	      printf("mapping: %s\n", mapping);
	      printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		     edge_number[e], (int) bandwidth[e], (int)G[e], loss_rate[e], emulator_mapping[emulators[e]]);
	    }
	  printf("EOF\n");
	  printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
	  printf("rm -f /var/tmp/mn.$$\n");
	  printf("sleep %d \n",spacing - length);
	  for (int participant = 1; participant < participants.size() ;participant++ )
	    {
	      node chosen = participants_array[ participant];
	      if (!nodes_altered.rank( chosen))
		{
		  printf("client_bw %s %d %d %d \n", &G[chosen][6], seconds, (int)current[ chosen], (int)current[ chosen]);

		}
	    }
	  for (int counter = seconds; counter < seconds+spacing - length ;counter++ )
	    {
	      check_complete(counter, file_size_in_bytes, accumulated, current, completed, source_downstream);
	    }
	  seconds += (spacing - length);

	}
	
    }

  node chosen ;
  forall (chosen, participants)
    {
      if (chosen == source_)
	{
	  continue;
	}
      double elapsed = seconds - last_change[chosen];
//        accumulated[chosen] += elapsed*current[chosen];
      double average = accumulated[chosen] / (double)seconds;
      printf("average: %f %s\n", average, &G[chosen][6]); 
      printf("completed: %d client%s\n", (int)completed[chosen], &G[chosen][6]); 
    }

}
void change_intransitive_routing_broken (int initial_time,
				  int total_time,
				  int spacing_minimum,
				  int spacing_maximum,
				  int length_minimum,
				  int length_maximum,
				  double percentage_minimum, 
				  double percentage_maximum, 
				  double magnitude_minimum,
				  double magnitude_maximum
				  )
{
  
  printf("# total %d  initial  %d  spacingminimum %d spacing_maximum %d  length_minimum  %d length_maximum %d percentage_minimum %lf  percentage_maximum %lf  magnitude_minimum %lf magnitude_maximum %lf \n", total_time,initial_time, spacing_minimum, spacing_maximum, length_minimum, length_maximum, percentage_minimum, percentage_maximum, magnitude_minimum, magnitude_maximum);
  printf("sleep %d \n",initial_time);
  if (magnitude_minimum > magnitude_maximum)
    {
      printf("magnitude_minimum: %d\n", magnitude_minimum);
      printf("magnitude_maximum: %d\n", magnitude_maximum);
      exit(1);
    }
  if (spacing_minimum > spacing_maximum)
    {
      printf("spacing_minimum: %d\n", spacing_minimum);
      printf("spacing_maximum: %d\n", spacing_maximum);
      exit(1);
    }
  if (length_minimum > length_maximum)
    {
      printf("length_minimum: %d\n", length_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  if ( spacing_minimum < length_maximum)
    {
      printf(" spacing_minimum: %d\n",  spacing_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  node_array< double> accumulated(G,0);
  node_array< double> current(G,0);
  node_array< double> normal(G,0);
  node_array< double> last_change(G,0);
  int seconds = initial_time;

  while (seconds < total_time)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      list < edge> altered;
      double percentage =  percentage_minimum + drand48()*(percentage_maximum - percentage_minimum);
      int length =  length_minimum + (int)(drand48()*(length_maximum - length_minimum));
      int spacing =  spacing_minimum + (int)(drand48()*(spacing_maximum - spacing_minimum));
      int count =(int)( percentage*participants.size()*participants.size());
      for (int i = 0; i < count ;i++ )
	{	
	  edge e;
  
	  do 
	    {
	      e = nil;
	      //pipes are numbered starting with one
	      int source=0;
	      while (source==0 || source==1)
		{
		  source = random_integer2( participants.size()-1) + 1;
		}

	      int destination=0;
	      while (destination==0 || destination==1 ||destination==source)
		{
		  destination = random_integer2( participants.size()-1) + 1;
      
		}

	      node chosen_source = participants_array[ source];
	      node chosen_destination = participants_array[ destination];
	      list < edge>  forward_route = overlay_edges ( chosen_source, chosen_destination);
	      e = forward_route. pop_back();
	    }
	  while (!altered.rank(e));
	  altered.  push_back(e);


	  if ( edges[edge_number[e]] != e)
	    {
	      printf(" discrepancy edge_number[e]: %d\n", edge_number[e]);
	      exit( 1);
	    }

	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 edge_number[e], (int) bandwidth[e], (int)G[e], 1.0, emulator_mapping[emulators[e]]);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");

      int initial = length;
      printf("sleep %d \n",length);
      printf("cat > /var/tmp/mn.$$ << EOF\n");

      seconds += length;
      edge e;
      forall(e, altered)
	{
	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 edge_number[e], (int) bandwidth[e], G[e], loss_rate[e], emulator_mapping[emulators[e]]);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");
      printf("sleep %d \n",spacing - length);

      seconds += (spacing - length);

    }
}

void change_intransitive_routing (int number_participants,
				  int initial_time,
				  int total_time,
				  int spacing_minimum,
				  int spacing_maximum,
				  int length_minimum,
				  int length_maximum,
				  double percentage_minimum, 
				  double percentage_maximum, 
				  double magnitude_minimum,
				  double magnitude_maximum
				  )
{
  
  printf("# number_participants %d total %d  initial  %d  spacingminimum %d spacing_maximum %d  length_minimum  %d length_maximum %d percentage_minimum %lf  percentage_maximum %lf  magnitude_minimum %lf magnitude_maximum %lf \n", number_participants, total_time,initial_time, spacing_minimum, spacing_maximum, length_minimum, length_maximum, percentage_minimum, percentage_maximum, magnitude_minimum, magnitude_maximum);
  printf("sleep %d \n",initial_time);
  if (magnitude_minimum > magnitude_maximum)
    {
      printf("magnitude_minimum: %d\n", magnitude_minimum);
      printf("magnitude_maximum: %d\n", magnitude_maximum);
      exit(1);
    }
  if (spacing_minimum > spacing_maximum)
    {
      printf("spacing_minimum: %d\n", spacing_minimum);
      printf("spacing_maximum: %d\n", spacing_maximum);
      exit(1);
    }
  if (length_minimum > length_maximum)
    {
      printf("length_minimum: %d\n", length_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  if ( spacing_minimum < length_maximum)
    {
      printf(" spacing_minimum: %d\n",  spacing_minimum);
      printf("length_maximum: %d\n", length_maximum);
      exit(1);
    }
  node_array< double> accumulated(G,0);
  node_array< double> current(G,0);
  node_array< double> normal(G,0);
  node_array< double> last_change(G,0);
  int seconds = initial_time;

  while (seconds < total_time)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      list < int> altered;
      double percentage =  percentage_minimum + drand48()*(percentage_maximum - percentage_minimum);
      int length =  length_minimum + (int)(drand48()*(length_maximum - length_minimum));
      int spacing =  spacing_minimum + (int)(drand48()*(spacing_maximum - spacing_minimum));
      int count =(int)( percentage*number_participants*number_participants);
      for (int i = 0; i < count ;i++ )
	{	
	  int  chosen = 0;
	  do 
	    {
	      int source=0;
	      while (source==0 || source==1)
		{
		  source = random_integer2(  number_participants-1) + 1;
		}

	      int destination=0;
	      while (destination==0 || destination==1 ||destination==source)
		{
		  destination = random_integer2( number_participants-1) + 1;
      
		}
	      chosen =  destination + (source)*number_participants;
//  	      printf("chosen: %d\n", chosen);
     
	    }
	  while (altered.rank(chosen));
	  altered.  push_back(chosen);

	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 chosen, 0, 0, 1.0, emulator_mapping [emulators[edges[chosen]]]);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");

      int initial = length;
      printf("sleep %d \n",length);
      printf("cat > /var/tmp/mn.$$ << EOF\n");

      seconds += length;
      int e;
      forall(e, altered)
	{
	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 e, 0, 0, 0.0, emulator_mapping [emulators[edges[e]]]);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");
      printf("sleep %d \n",spacing - length);

      seconds += (spacing - length);

    }
}


class altered_pair
{
public:
  altered_pair(int source1, int destination1): source(source1), destination(destination1) {}
  int source;
  int destination;
};
 
void change_physical_route (int total_time,
				int initial_time,
				int spacing,
				int length,
				double percentage, 
				double magnitude_minimum,
				double magnitude_maximum
)
{
 
  printf("# total %d  initial  %d  spacing %d  length %d percentage %lf  magnitude_minimum %lf magnitude_maximum %lf \n", total_time,initial_time, spacing, length, percentage, magnitude_minimum, magnitude_maximum);
  printf("sleep %d \n",initial_time);
  if (magnitude_minimum > magnitude_maximum)
    {
      printf("magnitude_minimum: %d\n", magnitude_minimum);
      printf("magnitude_maximum: %d\n", magnitude_maximum);
      exit(1);
    }

  int seconds = initial_time;
  int count =(int)( percentage*participants.size());
  while (seconds < total_time)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      list < altered_pair*> altered;
      for (int i = 0; i < count ;i++ )
	{
	  int source=0;
	  while (source==0 || source==1)
	    {
	      source = random_integer2( participants.size()-1) + 1;
	    }

	  int destination=0;
	  while (destination==0 || destination==1 ||destination==source)
	    {
	      destination = random_integer2( participants.size()-1) + 1;
      
	    }

	  node chosen_source = participants_array[ source];
	  node chosen_destination = participants_array[ destination];
	  altered_pair* pair = new altered_pair (source, destination);
	  altered.  push_back(pair);
 
	  double multiplier = magnitude_minimum + drand48()*(magnitude_maximum - magnitude_minimum);
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/mesh/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");

      int initial =length;
      printf("sleep %d \n",length);
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      altered_pair * p;
      forall(p, altered)
	{
	  printf("route add  vn%d vn%d pipe ",
		 p->source, p->destination );
	  node chosen_source = participants_array[ p->source];
	  node chosen_destination = participants_array[ p->destination];
	  list < edge>  forward_route = overlay_edges ( chosen_source, chosen_destination);
	  while (forward_route.size())
	    {
	      edge e = forward_route. pop_front();
	      printf(" %d", edge_number[e]);
	    }

	  printf("\n" );
	}
      printf("EOF\n");
      printf("/usr/research/home/dkostic/mesh/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");
      printf("sleep %d \n",spacing - initial);

      seconds+=spacing;
    }

}


void change_physical_bandwidth_partition (int total_time,
				 int initial_time,
				 int spacing,
				 double percentage, 
				 double magnitude)
{
  
  printf("# total %d  initial  %d  spacing %d  percentage_links %lf  magnitude %lf \n", total_time,initial_time, spacing, percentage, magnitude);
  printf("sleep %d \n",initial_time);

  int seconds = initial_time;
  int count =(int)( percentage*participants.size());
  int round = 0;
  while ( round < 3)
    {
      printf("cat > /var/tmp/mn.$$ << EOF\n");
      list < edge> altered;
      for (int i = 2; i < participants.size() ;i++ )
	{
	  //pipes are numbered starting with one
	  int  participant = i; 
//  	  printf("participant: %d\n", participant);

	  node chosen = participants_array[ participant];
	  list < edge>  forward_route = overlay_edges ( chosen, source_);
	  list < edge>  reverse_route = overlay_edges ( source_, chosen);
	  int modified = 0;
	  edge e;
	  e = forward_route. pop_front();
	  edge r  = reverse_route.pop_back();

	  double previous_direct = bandwidth[e];
	  double previous_reverse = bandwidth[r];
	  double multiplier = magnitude;
	  int  direct_value ;
	  int  reverse_value ;
	  if (  round==0 && i<participants.size()/2 ||
		round==1 && i>=participants.size()/2)
	    {
	      direct_value = (int)(multiplier*previous_direct);
	      reverse_value = (int)(multiplier* previous_reverse);
	    }
	  else
	    {
	      direct_value = (int)previous_direct;
	      reverse_value = (int)previous_reverse;
	    }

	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 edge_number[e], (int)direct_value,(int)G[e], loss_rate[e], emulator_mapping[emulators[e]]);
	  printf("config %4d bw %dKbps delay %dms plr %lf owner %s\n",
		 edge_number[r], (int)reverse_value,(int)G[r], loss_rate[r], emulator_mapping[emulators[r]]);
	}
      round++;

      printf("EOF\n");
      printf("/usr/research/home/dkostic/mesh/modelmod /var/tmp/mn.$$\n");
      printf("rm -f /var/tmp/mn.$$\n");

      int initial =spacing/2;
      printf("sleep %d \n",spacing/2);

      seconds+=spacing/2;
    }

}

void replay_planetlab_file( char* replay_file)
{
  //  read_modelnet_network( );

  double time_index= 0;
  double dump_time_index= 0;
  double starting_time_index= 0;
  int starting_set = 0;


 FILE *myfile;
  int i=0;
  char intro[80];
  char name[80];
  char ip[80];
  char rest[512];
  char command[80];
  char status[80];
  char parent[80];
  char child[80];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  myfile = (FILE *)fopen(replay_file, "r");
if (!myfile)
{
 printf("nonexistent replay file  %s\n", 
       replay_file);

}

 node entry;
 //and make all all of them act dead, except root
 forall_nodes(entry,G) 
   {
     node_status[entry]= 1;
   }
  i=0;
//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
//        cout << whole_line << endl;
//        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      if (strncmp (whole_line,"Node", 4))
	{
	  continue;
	}
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %lf %d %s %[^\n]s", 
		 &intro, &ip, &name,  &time_when, &tid,&command, &rest))!= 7)
	{
	  continue;
	}
//  printf("got %d, %s %s %s %f %d : %s %s\n", 
//         successful,intro,ip, name,  time_when, command, rest);
//  printf("got %f %f %f\n", 
//         time_when,starting_time_index,dump_time_index);

      if (!strcmp (command,"REPLAY_EXPERIMENT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed;
	  if (sscanf(rest, "%s %d %d %d %d %d %lf %lf", 
		     &name, &root,&topology, &topology_nodes, &participant_nodes, &seed, &delay_bound, &SPT_ratio)!= 8)
	    continue;
	  cout
	    << "Running with delay bound "  << delay_bound<< endl
	    << "Running with delay ratio " << SPT_ratio<< endl;
	  delay_bound*= 1000; 
	  narada_nodes = participants.size();
	  topology_nodes = G.number_of_nodes();

	  //	  node_status[get_node_from_name(name)]= 0;
	  //  printf("initialize root %x\n",get_node_number(get_node_from_name(name))+ 1);
	  initialized = 1;
	}
      if (!strcmp (command,"REPLAY_INIT"))
	{
	  int root,topology, topology_nodes, participant_nodes, seed, delay_bound;
	  if (sscanf(rest, "%s %d", 
		     &name, & root)!= 2)
	    continue;
	  //make sure they don't act dead anymore
	  //	  node_status[get_node_from_name(name)]= 0;
	  //dmk: ust to save time, commenting out
//  	  recompute_cost = 1;

	  //  printf("initialize  %x\n",get_node_number(get_node_from_name(name))+ 1);	
	}

//        if (!initialized)
//  	{
//  	  continue;
//  	}
      if (!starting_set)
	{
	  starting_set = 1;
	  starting_time_index =time_when;
	}

      if (!strcmp (command,"REPLAY_ADD")||
	  !strcmp (command,"REPLAY_REMOVE"))
	{
	  //  printf("got %s %s\n", 
	  //         command, rest);

	  if (sscanf(rest, "%s %s", 
		     &parent, &child)!= 2)
	    continue;
	  if (!strcmp (command,"REPLAY_REMOVE"))
	    {
	      printf("REPLAY_REMOVE %s %s\n", 
		     parent, child);
	      //      LEDA_remove_overlay_link(get_node_from_name(parent),
	      //		       get_node_from_name(child));
	      //we dont want erroneous disconnected printouts
	      continue;
	    }
	  else
	    {
	      //LEDA_add_overlay_link(get_node_from_name(parent),
	      //		    get_node_from_name(child));
	      printf("REPLAY_ADD %s %s\n", 
		     parent, child);
	    }
	}
      else if(!strcmp (command,"REPLAY_HEIGHT"))
	{
	  double farthest = 0;
	  sscanf(rest, "%lf", 
		 &farthest);
	  my_ns_tree_farthest = farthest;
	  printf("SETTING TREE HEIGHT %lf \n",farthest);
	}
      else if(!strcmp (command,"REPLAY_DELAY"))
	{
	  double farthest = 0;
	  int sequence = 0;
	  sscanf(rest, "%d %lf", 
		 & sequence,&farthest);
	  //	  modelnet_distance[get_node_from_name(name)] = 1000*farthest;
	  //  	  printf("SETTING TREE DELAY for %s to %lf \n",name,farthest);
	}
      else if(!strcmp (command,"REPLAY_PAYLOAD"))
	{
	  double farthest = 0;
	  int payload = 0;
	  sscanf(rest, "%d", 
		 &payload);
	  printf("%s %s (%f): payload = %d \n", 
		 intro,ip,  time_when-starting_time_index, payload);

	  //  	  printf("SETTING TREE DELAY for %s to %lf \n",name,farthest);
	}

      if (!initialized)
	{
	  continue; 
	}
      if (!dump_time_index||
	  time_when> dump_time_index+ 1)
	{
	  dump_time_index=time_when;
	  LEDA_dump_stats(time_when-starting_time_index, my_ns_tree_farthest);
	}
    }
  fclose(myfile);
  //  LEDA_dump_tree(dump_time_index-starting_time_index,NULL);

}

int available_slots;
edge_map<int>flows(G,0);

//setting residual 0 means that you don't want to know what bandwidth might be available to an incoming flow
//instead,you just want to know the fair share of the physical link bandwidth each of the flows will be getting
double overlay_bottleneck_bandwidth (node from, node to, int residual = 1)
{
  double result=infinity;

  edge p;
  list < edge > *constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      double predicted_bandwidth = bandwidth[p]/(residual+flows[p]);
      if (predicted_bandwidth< result)
	{
	  result=predicted_bandwidth;
	}
    }

  return result;
}
double overlay_loss_rate (node from, node to)
{
  double result=1;

  edge p;
  list < edge > *constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      double success = 1-loss_rate[p];
      result*=success;
    }

  return 1-result;
}

double predicted_TCP_throughput(node from, node to)
{
  double sender_rtt=(overlay_true_distance( from, to)+overlay_true_distance( to, from))/1000.0;
//    printf("sender_rtt %f\n", sender_rtt);  
  double loss=overlay_loss_rate( from, to);
  double prediction =(1472*8)/((sender_rtt*sqrt((2*loss)/3.0))+(4*sender_rtt)*(3*sqrt((3*loss)/8.0))*(loss*(1+32*loss*loss)));

//  cout << "prediction "<< prediction << "for loss rate "<< loss <<endl;
  return prediction/1000.0;
}

double overlay_predicted_bandwidth (node from, node to, int residual = 1)
{
  double possible =predicted_TCP_throughput(from, to);
  double result=overlay_bottleneck_bandwidth(from, to, residual);
//  cout << "bandwidth "<< result <<endl;
  if (result > possible)
    {
      result = possible;
    }
//  cout << "result "<< result <<endl;

  return result;
}

double bandwidth_to_root (node from, node_array<node>& pred)
{
  double result=infinity;
  node v=from;
  while (pred[v])
    {
      double bbw = overlay_predicted_bandwidth(pred[v], v, 0);
      if ( bbw < result ) 
	{
	  result = bbw;
	}
      v= pred[v];
    }

  if (result==infinity)
    {
      result = 0;
    }

  return result;
}

double delay_to_root (node from, node_array<node>& pred)
{
  double result=0;
  node v=from;
  while (pred[v])
    {
      double d =  overlay_true_distance(pred[v], v);
      result += d;
      v= pred[v];
    }
 return result;
}

int dump_bottleneck_tree( node_array<node>& pred,
			  node_array<double>& root_bandwidth,
	      char * name)
{

  fp = (FILE *)fopen(name, "w");
  if (!fp )
    {
      return 0;
    }
  int length =(int) participants.size()/8;
  if (length > 120)
    {
      length = 120;
    }

  int maximum = 0;
  //    fprintf(fp, "digraph G {\nsize=\"50,50\";rotate=\"90\";\n");
  fprintf(fp, "digraph G {\n");
  fprintf(fp, "size=\"%d,12\"\n",  length);
  fprintf(fp, "ratio=fill\n");
  fprintf(fp, "orientation=portrait\n");
  fprintf(fp, "label=\"%6.2lf\"\n", delay_bound);
  node v;
  forall(v, participants) 
    {
      root_bandwidth[v] = bandwidth_to_root (v, pred);
      double bbw =0;
      if (pred[v]) 
	{
	  bbw = overlay_predicted_bandwidth(pred[v], v, 0);
	  fprintf(fp, "\"%s (%6.2lf/%6.2lf) \" ->",
		  &G[ pred[v]][6],root_bandwidth[pred[v]], delay_to_root( pred[v], pred)) ;
	  fprintf(fp, " \"%s (%6.2lf/%6.2lf) \" [label=\"%6.2lf\"];\n", 
		  &G[v][6],root_bandwidth[v], 
		  delay_to_root(v, pred),
		  bbw
		  );
	}
    }

  fprintf(fp, "}\n");

  fclose (fp);
  return maximum;
}

int file_bottleneck_tree( node_array<node>& pred,
			  node_array<double>& root_bandwidth,
	      char * name)
{

  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }
  
  chdir(mn_mdb);

  fp = (FILE *)fopen(name, "w");
  if (!fp )
    {
      return 0;
    }
  node v;
  forall(v, participants) 
    {
      if (pred[v]) 
	{
	  fprintf(fp, "%s %s \n", 
		  &G[v][0], &G[pred[v]][0]
		  );
	}
    }

  fclose (fp);
  return 0;
}

void route_flow (node from, node to, int add = 1)
{
  if (!  from || !to)
    {
      return;
    }

  edge p;
  list < edge > *constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      if (add)
	{
	  flows[p]++;
	}
      else
	{
	  flows[p]--;
	}

      printf("edge_number[p]: %d (%d)has %d flows (%d)\n", edge_number[p],bandwidth[p], flows[p],bandwidth[p]/ flows[p]);
    }
}

//use the bullet format
double bandwidth_printer (node v, double parent_bandwidth)
{
      struct timezone tz_;
      timeval clock_;

      gettimeofday(&clock_, &tz_);
      double now = (clock_.tv_sec+(double)clock_.tv_usec/1000000.0);

      printf ("%s %lf %d REPLAY_BULLET_BANDWIDTH %d %d %d %d %d %d\n", 
	    &G[v][0], 1234432100.0, 0,
	    (int) parent_bandwidth,
	    (int) 0,
	    //  	(int) streaming_bandwidth_parent,
	    (int) parent_bandwidth,
	    (int) parent_bandwidth,
	    (int) parent_bandwidth,
	    (int) parent_bandwidth
	    );
} 
double GREEDY_BOTTLENECK_TREE( GRAPH<string,double>&  G, 
			       int max_degree = infinity,
			       int max_height = infinity
)
{ 
    
  node_pq<double> PQ(G);
  node_array<double> degree(G, 0);
  node_array<double> height(G, 0);
  node_array<double> bottleneck(G, infinity);
  node_array<node> pred(G, nil);
  node_array<double> root_bandwidth(G, infinity);

  list < edge >  solution_edge_list;
  list < node >  solution_node_list;
  list < node >  remaining_node_list = participants;
  node s = source_;

  if (s == nil) return 0; //empty graph

  pred[source_] = source_;

  double c;
  node u,v,r;
  forall_nodes(v, G)
    {
      pred[v]=nil;
      root_bandwidth[v]=infinity;
    }

  edge e;
  forall_edges(e, G)
    {
      flows[e]=0;
    }

  bottleneck[s] = infinity;
  while ( !remaining_node_list.empty()) 
    {
      node best_node = NULL;
      double best_bottleneck= 0;
      forall(r, remaining_node_list) 
	{
//  	  cout << "considering "<< G[r] <<endl;
	  if (pred[r]) 
	    {
//  	      cout  << "potential "  << " "  << G[pred[r]] << " "  << degree[pred[r]] << " "
//  		   << " " << height[pred[r]] <<endl;
	    }
	  //2 these are largely superfluous, because we don't pick nodes unless they're good potential parents
	  if (best_bottleneck < bottleneck[r] &&
	      (!pred[r] ||(pred[r] && height[pred[r]]< max_height)) &&
	      degree[r]< max_degree 
	      )
	    {
	      best_bottleneck = bottleneck[r];
	      best_node = r;
	    }
	}
      if (!best_node)
	{
	  cout << "couldn't find next best node " <<endl;
	  exit(1);
	}

    u = best_node;
    if (pred[best_node]) 
      {
	degree[pred[best_node]]++;
	height[best_node]=height[pred[best_node]]+ 1;
      }
    cout << "picking "<< G[u] <<endl;
cout  << "w bottleneck "  << best_bottleneck  << endl;
    solution_node_list.append(u);
    remaining_node_list.remove(u);
    if (pred[u]) route_flow(pred[u], u);

    forall(r, remaining_node_list) 
      {
	pred[r]=nil;

	double new_bottleneck = 0;
	forall(v, solution_node_list) 
	  {
	    double bbw = overlay_predicted_bandwidth(v, r);
//  	    printf("%s->(%f)%s\n", G[v], bbw, G[r]);
//  	    cout <<G[v] << "->("  << bbw  << ")" <<  G[r]<<endl;
	    if ( new_bottleneck < bbw ) 
	      {
		if (height[v]< max_height &&
		    degree[v]< max_degree 
		    )
		  {
		    new_bottleneck = bbw;
		    pred[r] = v; //this is merely a possibility
//  		cout <<G[v] << "->("  << bbw  << ")" <<  G[r]<<endl;
		  }
	      }
	  }
	bottleneck[r]=new_bottleneck;
//  	cout <<G[r] << " new bottleneck is "  << new_bottleneck  << endl;
      } 
    }
  double result = infinity;
  forall(v, solution_node_list) 
    {
      root_bandwidth[v] = bandwidth_to_root (v, pred);
//        printf("root_bandwidth %f\n", root_bandwidth[v]);
      bandwidth_printer (v, 1000.0*root_bandwidth[v]);

      if (pred[v]) 
	{
	  double bbw = root_bandwidth[v];
	  if ( bbw < result ) 
	    {
	      result = bbw;
	    }
	}
    }
  file_bottleneck_tree(pred, root_bandwidth,"bottleneck.parent" );
  dump_bottleneck_tree(pred, root_bandwidth,"bottleneck.tree" );
  return result;
}

double bottleneck_tree (int degree, int height )
{

  double result = GREEDY_BOTTLENECK_TREE(G ,degree, height);

  return result;
}
#if 1
double IP_bandwidth (node from, node to)
{
  double result=infinity;

  edge p;
  list < edge > *constituents =modelnet_overlay_matrix(from, to);

  forall(p,*constituents)
    {
      double predicted_bandwidth = bandwidth[p];
      if (predicted_bandwidth< result)
	{
	  result=predicted_bandwidth;
	}
    }

  return result;
}

 double  IP_MULTICAST(GRAPH<string,double>& G)
{
  node v, s = source_;
  double result = infinity;
  forall(v, participants) 
    {
      if (v==s) continue;
      double parent_bandwidth=IP_bandwidth(s,v);

      bandwidth_printer (v, 1000.0*parent_bandwidth);

      if ( parent_bandwidth < result ) 
	{
	  result = parent_bandwidth;
	}

    }
  return  result; 
}
double  multicast_tree ( )
{

  double result = IP_MULTICAST(G);

  return result;
}
#endif


struct color_route
{
  color_route (int s, int d, int c) : source(s), destination(d), color(c) {}
  int source;
  int destination;
  int color;
};
const int COLOR_SIZE = 10;
const double INBOUND_UTILIZATION= 0.8;
const double OUTBOUND_UTILIZATION= 0.8;
const double ROOT_OUTBOUND_UTILIZATION= 0.85;

enum  GOOD_MESH_STRATEGY
{
  OLD = -1, // weird behavior, sometimes first, sometimes last on the list
  FIRST_FOUND,
  MOST_CAPABLE,
  LEAST_UTILIZED,
  LAST_FOUND,
  RANDOM,
  FIRST_AFFINITY,
  RANDOM_LIMITED,
  FIRST_FOUND_LIMITED_COUNT,
  LEAST_CONTENTION
};

// assuming an unconstrained core, find a good mesh
double  good_mesh(int strategy = FIRST_FOUND)
{

  //this code assumes that participants are added via stub links
  //and that all routes to other participants traverse the same inbound/outbound link
  double result = 0;
  node n, v, other=nil;
  node_array< list<int> >  node_colors(G);
  node_array< list<node> >  node_receivers(G);
  node_array< list<node> >  node_senders(G);
  node_array<int> inbound_capacity(G, 0);
  node_array<int> inbound_physical_capacity(G, 0);
  node_array<int> outbound_capacity(G, 0);
  node_array<int> inbound_bandwidth(G, 0);
  node_array<int> outbound_bandwidth(G, 0);
  dictionary <double, node> capacity_ratios;
  list< color_route*>  color_mapping;

  forall(n, participants)
    {
      if (source_ == n)
	{
	  continue;
	}
      if (other==nil)other = n; 
      list < edge> outbound_route = overlay_edges(n, source_);
      list < edge> inbound_route = overlay_edges(source_, n);
      inbound_capacity[n]=  (int)(INBOUND_UTILIZATION*bandwidth[inbound_route.tail()]/COLOR_SIZE);
      inbound_physical_capacity[n]=  (int)(bandwidth[inbound_route.tail()]);
      outbound_capacity[n]=  (int)(OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);
      capacity_ratios.insert((double)bandwidth[outbound_route.front()]+drand48(),
			     n);//to distinguish between nodes with identical bandwidth
      printf("%s inbound_capacity: %d outbound_capacity: %d outbound: %d\n", &G[n][0], inbound_capacity[n], outbound_capacity[n], bandwidth[outbound_route.front()]);
    }
  list < edge> outbound_route = overlay_edges(source_,get_node_from_name("client2"));// other);
  outbound_capacity[source_]=  (int)(ROOT_OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);
  
  int root_capacity = outbound_capacity[source_];
  array <list < node> > color_locations(root_capacity);
  printf("root_capacity: %d: %d\n", outbound_route.length(), root_capacity);
  dic_item it;
  int root_color = 0;
//    while (outbound_bandwidth[source_]< outbound_capacity[source_])
  forall_rev_items(it,capacity_ratios) 
    {
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	<<  " cap: "  << inbound_capacity[n];
//        if (outbound_bandwidth[source_]< outbound_capacity[source_])
	{
	  int allows = inbound_capacity[n];
	  int max = root_color+ allows;
	  if (max > root_capacity) max = root_capacity;
	  for (; root_color < max ;root_color++ )
	    {
	      color_mapping.  append ( new color_route(0, atoi (&G[n][6]),root_color));
	      color_locations[root_color].append (n);
	      inbound_bandwidth[n ]++;
	      outbound_bandwidth[source_]++;
	      node_colors[n].append (root_color);
	      if  ( !node_receivers[source_].rank(n))
		node_receivers[source_].append(n);
	      if  ( !node_senders[n].rank(source_))
		node_senders[n].append(source_);
	    }
	  printf(" abw %d\n", inbound_bandwidth[n ]);

	  if  (root_color>=root_capacity)
	    break;
	}
    }
printf("color_count %d\n", root_color);

printf("capacity_ratios.size(): %d\n", capacity_ratios.size());
 int incoming = 0; 
 forall_rev_items(it,capacity_ratios) 
    {
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	   <<  " cap: "  << inbound_capacity[n]
	   <<  " bw: "  << inbound_bandwidth[n];
      if ( inbound_bandwidth[n]<  inbound_capacity[n])
	{
	  int missing = inbound_capacity[n] - inbound_bandwidth[n];
	  for (int i = 0; i < root_capacity ;i++ )
	    {
	      if  ( node_colors[n].rank(i))
		continue;
	      node from = nil;
	      node candidate;
	      list<node> candidates;
	      forall (v,color_locations[i])
		{
		  if (outbound_bandwidth[v]<  outbound_capacity[v])
		    {
		      if (strategy == OLD)
			{
			  from = v;
			  break;
			}
		      else if (strategy == RANDOM_LIMITED)
			{
			  if (node_receivers[v].size()>=2)
			    continue;
			}
		      candidates.append(v);
		    }
		}

	      if (candidates.size())
		{
		  if (strategy == FIRST_FOUND)
		    {
		      from = candidates.front();
		    }
		  else if (strategy == MOST_CAPABLE)
		    {
		      int outbound_best = 0;
		      forall (candidate,candidates)
			{
			  if (outbound_capacity[v] > outbound_best)
			    {
			      outbound_best = outbound_bandwidth[v];
			      from = candidate;
			    }
			}
		    }
		  else if (strategy == LEAST_UTILIZED)
		    {
		      int outbound_best = 0;
		      forall (candidate,candidates)
			{
			  int outbound_sample =  outbound_capacity[candidate] - outbound_bandwidth[candidate];
			  if (outbound_sample > outbound_best)
			    {
			      outbound_best = outbound_sample;
			      from = candidate;
			    }
			}
		    }
		  else if (strategy == LAST_FOUND)
		    {
		      from = candidates.back();
		    }
		  else if (strategy == RANDOM)
		    {
		      int which = random_integer2(candidates.size());
		      list_item wi = candidates.get_item(which);
		      from = candidates[wi];
		    }
		  else if (strategy == RANDOM_LIMITED)
		    {
		      int which = random_integer2(candidates.size());
		      list_item wi = candidates.get_item(which);
		      from = candidates[wi];
		    }
		  else if (strategy == FIRST_AFFINITY)
		    {
		      int outbound_best = 0;
		      forall (candidate,candidates)
			{
			  if  ( node_receivers[candidate].rank(n))
			    {
			    from = candidate;
			    break;
			    }
			}
		      if (!from)
			from = candidates.front();
		    }
		}  
	      if  (from)
		{
		  color_mapping.  append ( new color_route(atoi (&G[from][6]), atoi (&G[n][6]), i));
		  color_locations[i].append (n);
		  node_colors[n].append (i);
		  outbound_bandwidth[from]++;
		  inbound_bandwidth[n]++;
		  if  ( !node_receivers[from].rank(n))
		    node_receivers[from].append(n);
		  if  ( !node_senders[n].rank(from))
		    node_senders[n].append(from);
		  missing--;
		  if (missing <=0) break;
		}
	    }
	}
      printf(" abw %d\n", inbound_bandwidth[n ]*COLOR_SIZE);
      incoming += inbound_bandwidth[n ]*COLOR_SIZE;
    }


 forall_rev_items(it,capacity_ratios) 
    {
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	   <<  " incap: "  << inbound_capacity[n]*COLOR_SIZE
	   <<  " inbw: "  << inbound_bandwidth[n]*COLOR_SIZE
	   <<  " outcap: "  << outbound_capacity[n]*COLOR_SIZE
	   <<  " outbw: "  << outbound_bandwidth[n]*COLOR_SIZE
	   <<  " util: "  << inbound_bandwidth[n]*COLOR_SIZE/(double)inbound_physical_capacity[n]
	   <<  " getters: "  << node_receivers[n].size()
	   <<  " givers: "  << node_senders[n].size()
	   << endl;
      
    }
//   color_route* route;
//   forall( route, color_mapping) 
//      {
//        printf("color_route client%d client%d %d\n",  route->source, route->destination, route->color);
//      }
 printf("incoming/participants.size(): %f\n", incoming/(double)participants.size());
printf("strategy: %d\n", strategy);

 return result;
}

// assuming an unconstrained core, find a good mesh
double  good_color_mesh(int strategy = FIRST_FOUND)
{

  //this code assumes that participants are added via stub links
  //and that all routes to other participants traverse the same inbound/outbound link
  double result = 0;
  node n, v, other=nil;
  node_array< list<int> >  node_colors(G);
  node_array< list<node> >  node_receivers(G);
  node_array< list<node> >  node_senders(G);
  node_array<int> inbound_capacity(G, 0);
  node_array<int> inbound_physical_capacity(G, 0);
  node_array<int> outbound_capacity(G, 0);
  node_array<int> inbound_bandwidth(G, 0);
  node_array<int> outbound_bandwidth(G, 0);
  dictionary <double, node> capacity_ratios;
  list< color_route*>  color_mapping;
  node_pq<double> PQ(G);

  forall(n, participants)
    {
      if (source_ == n)
	{
	  continue;
	}
      if (other==nil)other = n; 
      list < edge> outbound_route = overlay_edges(n, source_);
      list < edge> inbound_route = overlay_edges(source_, n);
      inbound_capacity[n]=  (int)(INBOUND_UTILIZATION*bandwidth[inbound_route.tail()]/COLOR_SIZE);
      inbound_physical_capacity[n]=  (int)(bandwidth[inbound_route.tail()]);
      outbound_capacity[n]=  (int)(OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);
      capacity_ratios.insert((double)bandwidth[outbound_route.front()]+drand48(),
			     n);//to distinguish between nodes with identical bandwidth
      printf("%s inbound_capacity: %d outbound_capacity: %d outbound: %d\n", &G[n][0], inbound_capacity[n], outbound_capacity[n], bandwidth[outbound_route.front()]);
    }
  list < edge> outbound_route = overlay_edges(source_,get_node_from_name("client2"));// other);
  outbound_capacity[source_]=  (int)(ROOT_OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);

  int root_capacity = outbound_capacity[source_];
  array <list < node> > color_locations(root_capacity);
  printf("root_capacity: %d: %d\n", outbound_route.length(), root_capacity);
  dic_item it;

  int root_color = root_capacity;
  printf("color_count %d\n", root_color);

  printf("capacity_ratios.size(): %d\n", capacity_ratios.size());
 int incoming = 0; 

 for (int i = 0; i < root_capacity ;i++ )
   {
     color_locations[i].push_back  (source_);
     forall(n, participants)
       {
	 if (source_ == n)
	   {
	     continue;
	   }
	 PQ.insert(n, outbound_bandwidth[n] - outbound_capacity[n]);
       }
     while ( ! PQ.empty()) 
       {
	 n = PQ.del_min();

	 if ( inbound_bandwidth[n]<  inbound_capacity[n])
	   {
	     list<node> candidates;
	     node from = nil;
	     node candidate;
	     forall (v,color_locations[ i])
	       {
		 if (outbound_bandwidth[v]<  outbound_capacity[v])
		   {
		     if (v ==  source_ && color_locations[ i].size() > 1)
		       {	
			 continue;
		       }
		     candidates.append(v);
		   }
	       }

	     if (candidates.size())
	       {
		 if (strategy == FIRST_FOUND)
		   {
		     from = candidates.front();
		   }
		 else if (strategy == LEAST_UTILIZED)
		   {
		     int outbound_best = 0;
		     forall (candidate,candidates)
		       {
			 int outbound_sample =  outbound_capacity[candidate] - outbound_bandwidth[candidate];
			 if (outbound_sample > outbound_best)
			   {
			     outbound_best = outbound_sample;
			     from = candidate;
			   }
		       }
		   }
	       }  
	     if  (from)
	       {
		 color_mapping.  append ( new color_route(atoi (&G[from][6]), atoi (&G[n][6]), i));
		 color_locations[i].append (n);
		 node_colors[n].append (i);
		 outbound_bandwidth[from]++;
		 inbound_bandwidth[n]++;
//  		 PQ.decrease_inf(from,outbound_bandwidth[from] - outbound_capacity[from]);
		 if  ( !node_receivers[from].rank(n))
		   node_receivers[from].append(n);
		 if  ( !node_senders[n].rank(from))
		   node_senders[n].append(from);
	       }
	   }
       }
   }

 forall_rev_items(it,capacity_ratios) 
    {
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	   <<  " incap: "  << inbound_capacity[n]*COLOR_SIZE
	   <<  " inbw: "  << inbound_bandwidth[n]*COLOR_SIZE
	   <<  " outcap: "  << outbound_capacity[n]*COLOR_SIZE
	   <<  " outbw: "  << outbound_bandwidth[n]*COLOR_SIZE
	   <<  " util: "  << inbound_bandwidth[n]*COLOR_SIZE/(double)inbound_physical_capacity[n]
	   <<  " getters: "  << node_receivers[n].size()
	   <<  " givers: "  << node_senders[n].size()
	   << endl;
      incoming += inbound_bandwidth[n ]*COLOR_SIZE;
     
    }
 printf("incoming/participants.size(): %f\n", incoming/(double)participants.size());
printf("strategy: %d\n", strategy);
 return result;
 color_route* route;
 forall( route, color_mapping) 
    {
      printf("color_route client%d client%d %d\n",  route->source, route->destination, route->color);
    }

 return result;
}

// assuming an unconstrained core, find a good mesh
double   best_mesh(int strategy = FIRST_FOUND)
{

  //this code assumes that participants are added via stub links
  //and that all routes to other participants traverse the same inbound/outbound link
  double result = 0;
  node n, v, other=nil;
  node_array< list<int> >  node_colors(G);
  node_array< list<node> >  node_receivers(G);
  node_array< list<node> >  node_senders(G);
  node_array<int> inbound_capacity(G, 0);
  node_array<int> inbound_physical_capacity(G, 0);
  node_array<int> outbound_capacity(G, 0);
  node_array<int> inbound_bandwidth(G, 0);
  node_array<int> outbound_bandwidth(G, 0);
  dictionary <double, node> capacity_ratios;
  list< color_route*>  color_mapping;

  forall(n, participants)
    {
      if (source_ == n)
	{
	  continue;
	}
      if (other==nil)other = n; 
      list < edge> outbound_route = overlay_edges(n, source_);
      list < edge> inbound_route = overlay_edges(source_, n);
      inbound_capacity[n]=  (int)(INBOUND_UTILIZATION*bandwidth[inbound_route.tail()]/COLOR_SIZE);
      inbound_physical_capacity[n]=  (int)(bandwidth[inbound_route.tail()]);
      outbound_capacity[n]=  (int)(OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);
      capacity_ratios.insert((double)bandwidth[outbound_route.front()]+drand48(),
			     n);//to distinguish between nodes with identical bandwidth
      printf("%s inbound_capacity: %d outbound_capacity: %d outbound: %d\n", &G[n][0], inbound_capacity[n], outbound_capacity[n], bandwidth[outbound_route.front()]);
    }
  list < edge> outbound_route = overlay_edges(source_,get_node_from_name("client2"));// other);
  outbound_capacity[source_]=  (int)(ROOT_OUTBOUND_UTILIZATION*bandwidth[outbound_route.front()]/COLOR_SIZE);
  
  int root_capacity = outbound_capacity[source_];
  array <list < node> > color_locations(root_capacity);
  array <list < node> > accessible_color_locations(root_capacity);
  printf("root_capacity: %d: %d\n", outbound_route.length(), root_capacity);
  dic_item it;
//    while (outbound_bandwidth[source_]< outbound_capacity[source_])
  int root_colors = root_capacity;

printf("color_count %d\n", root_colors);

printf("capacity_ratios.size(): %d\n", capacity_ratios.size());
 int incoming = 0; 

 p_queue<int, int> color_appearances;
 for (int i = 0; i < root_capacity ;i++ )
   {
     color_locations[i].push_back(source_);
     accessible_color_locations[i].push_back(source_);
     node_colors[source_].append (i);
   }
 int available_colors = root_colors;

 forall_rev_items(it,capacity_ratios) 
    {
//        for (int i = 0; i < root_capacity ;i++ )
//  	{
//  	  color_appearances.insert(i, accessible_color_locations[i].size());
//  	}
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	   <<  " cap: "  << inbound_capacity[n]
	   <<  " bw: "  << inbound_bandwidth[n];
      if ( inbound_bandwidth[n]<  inbound_capacity[n])
	{
	  int missing = inbound_capacity[n] - inbound_bandwidth[n];
	  while (missing)
	    {
	      color_appearances.clear();
	      for (int j = 0; j < root_capacity ;j++ )
		{
		 if (accessible_color_locations[j].size() > 0) color_appearances.insert(j, accessible_color_locations[j].size());
		}
	      int i = -1;
	      while (!color_appearances.empty())
		{
		  int ic = color_appearances.del_min();
		  if  ( node_colors[n].rank(ic))
		    continue;
		  i = ic; 
		  break;
		}
	      if (i == -1)
		{
		  break;
		}
//  printf("i: %d, %d\n", i, accessible_color_locations[i].size());
	      list<node> candidates;

	      node from = nil;
	      node candidate;
	      forall (v,accessible_color_locations[i])
		{
		  if (outbound_bandwidth[v]<  outbound_capacity[v])
		    {
		      if (strategy == RANDOM_LIMITED)
			{
			  if (node_receivers[v].size()>=2)
			    continue;
			}
		      candidates.append(v);
		    }
		  else
		    {
		      printf(" deceiving i: %d, %d bw %d cap %d \n", i, accessible_color_locations[i].size(), outbound_bandwidth[v],  outbound_capacity[v]);
		      return 0;
		    }

		}

	      if (candidates.size())
		{
		  if (strategy == FIRST_FOUND)
		    {
		      from = candidates.front();
		    }
		  else if (strategy == LEAST_UTILIZED)
		    {
		      int outbound_best = 0;
		      forall (candidate,candidates)
			{
			  int outbound_sample =  outbound_capacity[candidate] - outbound_bandwidth[candidate];
			  if (outbound_sample > outbound_best)
			    {
			      outbound_best = outbound_sample;
			      from = candidate;
			    }
			}
		    }
		  else if (strategy == LAST_FOUND)
		    {
		      from = candidates.back();
		    }
		  else if (strategy == LEAST_CONTENTION)
		    {
		      double outbound_best = 0;
		      forall (candidate,candidates)
			{
			  int outbound_sample =  outbound_capacity[candidate] - outbound_bandwidth[candidate];
			  double outbound_freedom = outbound_sample /( double)  node_colors[ candidate] .size();
			  if (outbound_freedom > outbound_best)
			    {
			      outbound_best = outbound_freedom;
			      from = candidate;
			    }
			}
		    }
		  else if (strategy == RANDOM)
		    {
		      int which = random_integer2(candidates.size());
		      list_item wi = candidates.get_item(which);
		      from = candidates[wi];
		    }
		  else if (strategy == RANDOM_LIMITED)
		    {
		      int which = random_integer2(candidates.size());
		      list_item wi = candidates.get_item(which);
		      from = candidates[wi];
		    }
		  else if (strategy == FIRST_AFFINITY)
		    {
		      int outbound_best = 0;
		      forall (candidate,candidates)
			{
			  if  ( node_receivers[candidate].rank(n))
			    {
			    from = candidate;
			    break;
			    }
			}
		      if (!from)
			from = candidates.front();
		    }
		}  
	      if  (from)
		{
		  if (outbound_bandwidth[from] >=  outbound_capacity[from])
		    {
 		      printf("over capacitaty:  client%d\n", atoi (&G[from][6]) );
		      return 0;
		    }
		  color_mapping.  append ( new color_route(atoi (&G[from][6]), atoi (&G[n][6]), i));
		  if (outbound_bandwidth[n] < outbound_capacity[n])
		    {
		      accessible_color_locations[i].append (n);
		    }
		  color_locations[i].append (n);
		  node_colors[n].append (i);
		  outbound_bandwidth[from]++;
//  		  printf("route client%d client%d %d bw %d cap %d\n",  atoi (&G[from][6]), atoi (&G[n][6]), i, outbound_bandwidth[ from],  outbound_capacity[from]);
		  if (outbound_bandwidth[from] >=  outbound_capacity[from])
		    {
//  		      printf("incapacitated:  client%d\n", atoi (&G[from][6]) );
		      int c;
		      forall(c,node_colors[from]) 
			{
//  			  printf("inaccessible: %d at  client%d\n", c,atoi (&G[from][6]) );

			  accessible_color_locations[c]. remove ( from);
//  			  if (i != c) color_appearances.decrease_p(c, accessible_color_locations[c] .size() );
			}
		    }

		  inbound_bandwidth[n]++;
		  if  ( !node_receivers[from].rank(n))
		    node_receivers[from].append(n);
		  if  ( !node_senders[n].rank(from))
		    node_senders[n].append(from);
		  missing--;
		}
	      else
		{
		  printf("can't access color i: %d, %d\n", i, accessible_color_locations[i].size());
		  return 0;
		}

	    }
	}
      printf(" abw %d\n", inbound_bandwidth[n ]*COLOR_SIZE);
      incoming += inbound_bandwidth[n ]*COLOR_SIZE;
    }


 forall_rev_items(it,capacity_ratios) 
    {
      n = capacity_ratios.inf(it);
      cout << G[n ] <<  " : "  << capacity_ratios.key(it)
	   <<  " incap: "  << inbound_capacity[n]*COLOR_SIZE
	   <<  " inbw: "  << inbound_bandwidth[n]*COLOR_SIZE
	   <<  " outcap: "  << outbound_capacity[n]*COLOR_SIZE
	   <<  " outbw: "  << outbound_bandwidth[n]*COLOR_SIZE
	   <<  " util: "  << inbound_bandwidth[n]*COLOR_SIZE/(double)inbound_physical_capacity[n]
	   <<  " getters: "  << node_receivers[n].size()
	   <<  " givers: "  << node_senders[n].size()
	   << endl;
      
    }
 color_route* route;
 forall( route, color_mapping) 
    {
      printf("color_route client%d client%d %d\n",  route->source, route->destination, route->color);
    }
 printf("incoming/participants.size(): %f\n", incoming/(double)participants.size());
printf("strategy: %d\n", strategy);

 return result;
}


#endif

