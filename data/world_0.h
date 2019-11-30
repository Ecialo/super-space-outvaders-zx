#ifndef qqqqq
#define qqqqq

#define WORLD_SIZE_0 12
char max_map_depth_0 = 3;
char nodes_x_0[]   = {0, 0, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3} ;
char nodes_y_0[]   = {0, 1, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2} ;
char nodes_out_0[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0} ;
char nodes_in_0[]  = {0, 0, 0, 2, 1, 0, 2, 7, 3, 2, 0, 0} ;
char num_of_links_0 = 9;
char links_0[] = {1, 3, 1, 7, 1, 9, 3, 4, 3, 7, 4, 8, 6, 7, 7, 8, 9, 7} ;
content_type nodes_content_0[] = {NO_NODE, ENEMY, NO_NODE, ENEMY, ENEMY, NO_NODE, SKIP_NODE, ENEMY, ENEMY, ENEMY, NO_NODE, NO_NODE} ;
char node_args_0[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;

#define WORLD_SIZE_1 21
char max_map_depth_1 = 6;
char nodes_x_1[]   = {0, 0, 0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6} ;
char nodes_y_1[]   = {0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2} ;
char nodes_out_1[] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1} ;
char nodes_in_1[]  = {0, 0, 0, 2, 1, 1, 0, 2, 0, 2, 2, 6, 7, 0, 5, 2, 6, 4, 0, 2, 0} ;
char num_of_links_1 = 18;
char links_1[] = {1, 3, 1, 9, 1, 16, 3, 5, 4, 5, 5, 12, 7, 14, 9, 10, 9, 16, 10, 11, 11, 12, 12, 7, 12, 19, 15, 16, 16, 11, 16, 17, 17, 12, 19, 14} ;
content_type nodes_content_1[] = {NO_NODE, ENEMY, NO_NODE, ENEMY, SKIP_NODE, SHOP, NO_NODE, ENEMY, NO_NODE, ENEMY, ENEMY, ENEMY, ENEMY, NO_NODE, ENEMY, SKIP_NODE, ENEMY, ENEMY, NO_NODE, SHOP, NO_NODE} ;
char node_args_1[] = {0, 0, 0, 2, 0, 0, 0, 6, 0, 3, 4, 1, 0, 0, 9, 0, 8, 5, 0, 0, 0} ;

#define MAX_WORLD_SIZE  21
#define MAPS_COUNT 2
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
if (id ==  1 ) {

    world_size = WORLD_SIZE_1;
    max_map_depth = max_map_depth_1;
    nodes_x = nodes_x_1;
    nodes_y = nodes_y_1;
    nodes_out = nodes_out_1;
    nodes_in = nodes_in_1;
    num_of_links = num_of_links_1;
    links  = links_1;
    nodes_content = nodes_content_1;
    node_args = node_args_1;

}
}

#endif