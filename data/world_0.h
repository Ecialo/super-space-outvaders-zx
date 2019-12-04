
#define WORLD_SIZE_0 27
uch max_map_depth_0 = 8;
uch nodes_x_0[]   = {0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8} ;
uch nodes_y_0[]   = {0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2} ;
uch nodes_out_0[] = {0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0} ;
uch nodes_in_0[]  = {0, 0, 0, 2, 1, 1, 0, 2, 0, 0, 0, 2, 2, 6, 7, 0, 5, 2, 2, 2, 6, 4, 0, 2, 0, 0, 0} ;
uch num_of_links_0 = 20;
uch links_0[] = {1, 3, 1, 11, 1, 20, 3, 5, 4, 5, 5, 14, 7, 16, 11, 12, 11, 20, 12, 13, 13, 14, 14, 7, 14, 23, 16, 17, 17, 18, 19, 20, 20, 13, 20, 21, 21, 14, 23, 16} ;
uch nodes_content_0[] = {NO_NODE, ENEMY, NO_NODE, ENEMY, SKIP_NODE, SHOP, NO_NODE, ENEMY, NO_NODE, NO_NODE, NO_NODE, ENEMY, ENEMY, ENEMY, ENEMY, NO_NODE, ENEMY, ENEMY, ENEMY, SKIP_NODE, ENEMY, ENEMY, NO_NODE, SHOP, NO_NODE, NO_NODE, NO_NODE} ;
uch node_args_0[] = {0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 3, 4, 1, 0, 0, 9, 1, 1, 0, 8, 5, 0, 0, 0, 0, 0} ;

#define MAX_WORLD_SIZE 27 

#define MAPS_COUNT 1 

uch world_size;
uch max_map_depth;
uch *nodes_x;
uch *nodes_y;
uch *nodes_out;
uch *nodes_in;
uch num_of_links;
uch *links;
uch *nodes_content;
uch *node_args;
 

void select_pointers(uch id) {
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
