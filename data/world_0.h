
#define WORLD_SIZE 21
char max_map_depth = 6;
char nodes_x[]   = {0, 0, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6} ;
char nodes_y[]   = {0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2} ;
char nodes_out[] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1} ;
char nodes_in[]  = {0, 0, 0, 2, 1, 1, 0, 2, 0, 2, 2, 6, 7, 0, 5, 2, 6, 4, 0, 2, 0} ;
char num_of_links = 18;
char links[] = {1, 3, 1, 9, 1, 16, 3, 5, 4, 5, 5, 12, 7, 14, 9, 10, 9, 16, 10, 11, 11, 12, 12, 7, 12, 19, 15, 16, 16, 11, 16, 17, 17, 12, 19, 14} ;
content_type nodes_content[] = {NO_NODE, ENEMY, NO_NODE, ENEMY, SKIP_NODE, SHOP, NO_NODE, ENEMY, NO_NODE, ENEMY, ENEMY, ENEMY, ENEMY, NO_NODE, ENEMY, SKIP_NODE, ENEMY, ENEMY, NO_NODE, SHOP, NO_NODE} ;
char node_args[] = {0, 0, 0, 2, 0, 0, 0, 6, 0, 3, 4, 1, 0, 0, 9, 0, 8, 5, 0, 0, 0} ;

