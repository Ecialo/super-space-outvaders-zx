
#define WORLD_SIZE_0 21
char max_map_depth_0 = 6;
char nodes_x_0[]   = {0, 0, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6} ;
char nodes_y_0[]   = {0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2} ;
char nodes_out_0[] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1} ;
char nodes_in_0[]  = {0, 0, 0, 2, 1, 1, 0, 2, 0, 2, 2, 6, 7, 0, 5, 2, 6, 4, 0, 2, 0} ;
char num_of_links_0 = 18;
char links_0[] = {1, 3, 1, 9, 1, 16, 3, 5, 4, 5, 5, 12, 7, 14, 9, 10, 9, 16, 10, 11, 11, 12, 12, 7, 12, 19, 15, 16, 16, 11, 16, 17, 17, 12, 19, 14} ;
content_type nodes_content_0[] = {NO_NODE, ENEMY, NO_NODE, ENEMY, SKIP_NODE, SHOP, NO_NODE, ENEMY, NO_NODE, ENEMY, ENEMY, ENEMY, ENEMY, NO_NODE, ENEMY, SKIP_NODE, ENEMY, ENEMY, NO_NODE, SHOP, NO_NODE} ;
char node_args_0[] = {0, 0, 0, 2, 0, 0, 0, 6, 0, 3, 4, 1, 0, 0, 9, 0, 8, 5, 0, 0, 0} ;

#define MAX_WORLD_SIZE 21 

#define MAPS_COUNT 1 

char world_size;
char max_map_depth;
char *nodes_x;
char *nodes_y;
char *nodes_out;
char *nodes_in;
char num_of_links;
char *links;
content_type *nodes_content;
char *node_args;
 

void select_pointers(char id) {
if (id ==  0 ) {

    world_size = WORLD_SIZE_0;
    max_map_depth = max_map_depth_0;
    nodes_x = nodes_x_0;
    nodes_y = nodes_y_0;
    nodes_out = nodes_out_0;
    nodes_in = nodes_in_0;
    num_of_links = num_of_links_0;
    links  = links_0;
    nodes_content = nodes_content_0;
    node_args = node_args_0;

}
}
