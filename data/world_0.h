
#define WORLD_SIZE 12
char max_map_depth = 3;
char nodes_x[]   = {0, 0, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3} ;
char nodes_y[]   = {0, 1, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2} ;
char nodes_out[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0} ;
char nodes_in[]  = {0, 0, 0, 2, 1, 0, 2, 7, 3, 2, 0, 0} ;
char num_of_links = 9;
char links[] = {1, 3, 1, 7, 1, 9, 3, 4, 3, 7, 4, 8, 6, 7, 7, 8, 9, 7} ;
content_type nodes_content[] = {NO_NODE, JUST, NO_NODE, JUST, JUST, NO_NODE, SKIP_NODE, JUST, JUST, JUST, NO_NODE, NO_NODE} ;
char node_args[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;

