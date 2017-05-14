
//  evaluate.h

void LEDA_init(int address, int source = 0, int topo=0, int topo_nodes=0, int num_nodes=0);
void LEDA_experiment_started ( double);
void LEDA_experiment_stopped  (double);
void LEDA_experiment_parameters  (double,double,double, double);
void LEDA_tree_status(int address, int = 0);
void LEDA_node_status(int address, int = 0);
int LEDA_get_join_target ( int = -1);
void LEDA_set_link_delay (int from, int to, double value);
void LEDA_change_link_cost (int from, int to);
void LEDA_change_random_link_costs ();
void LEDA_set_link_down (int from, int to);
void LEDA_set_link_up (int from, int to);
void LEDA_add_overlay_link (int from, int to);
void LEDA_remove_overlay_link (int from, int to);
double LEDA_get_overlay_cost (int from, int to);
double LEDA_get_overlay_hops (int from, int to);
double LEDA_get_overlay_cost (const  char *, const  char * );
double LEDA_dump_tree(double,char * name, double = -1.0);
double LEDA_dump_stats(double, double);
double LEDA_cost_ratio();
double LEDA_MST_cost();
double LEDA_SPT_delay();
void LEDA_measure_set (int*target, int from, int count);
void LEDA_parent_died(int address,double when);
void LEDA_reconnected(int address,double when);
double LEDA_get_overlay_delay (int from, int to);
double LEDA_get_ns_overlay_delay (int from, int to);
int LEDA_get_simulator_ID(int number_);
void LEDA_initialize_ModelNet(int topology,int topology_nodes, int participants, int seed);
