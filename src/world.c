// WORLD CONTENT
#ifndef __SUP_SP_WORLD__
#define __SUP_SP_WORLD__

#define MAX_NEXT_WORLDS 5
// #define WORLD_SIZE 9

#define True 1
#define False 0

# define IN_UP 1
# define IN_MID 2
# define IN_DOWN 4

typedef enum ContentType {
    ENEMY,
    SHOP,
    NO_NODE,
    SKIP_NODE,
} content_type;

// WORLD
typedef struct WorldNode {
    char id;
    
    char next_worlds[MAX_NEXT_WORLDS];
    char num_of_next_worlds;
    char is_terminate;
    
} world_node;


void link_nodes(world_node *from, world_node *to) {
    char old_num_of_next_worlds = from->num_of_next_worlds;
    if (old_num_of_next_worlds == MAX_NEXT_WORLDS) {
        // return ERROR;
    }
    from->next_worlds[from->num_of_next_worlds] = to->id;
    from->num_of_next_worlds = old_num_of_next_worlds + 1;
    from->is_terminate = False;
    // return OK;
}

// WORLD INIT
char WORLD_INITED = False;

#include "../data/world_0.h"

char world_x_offset = 0;
char current_world = 1;
world_node world[MAX_WORLD_SIZE];

void init_world_node(world_node *node, char id) {
    node->id = id;
    node->num_of_next_worlds = 0;
    node->is_terminate = 0;
}

void select_map(char id) {
    memset(world, 0, sizeof(world_node) * MAX_WORLD_SIZE);
    select_pointers(id);
    WORLD_INITED = False;
}


void generate_world() {
    char i;
    if (WORLD_INITED) {
        return;
    }

    for (i = 0; i < world_size; i++) {
        init_world_node(&world[i], i);
    }
    for (i = 0; i < num_of_links; i++) {
        link_nodes(
            &world[links[2*i]], 
            &world[links[2*i + 1]]
        );
    }
    
    WORLD_INITED = True;
}

char collect_money() {
    if (nodes_content[current_world] == ENEMY) {
        return node_args[current_world] + 1;
    } else {
        return 0;
    }
}

#endif