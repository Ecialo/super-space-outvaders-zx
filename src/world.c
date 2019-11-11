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
    JUST,
    NO_NODE,
    SKIP_NODE,
} content_type;


typedef struct JustContent
{
    char value;
} just_content;

void init_just_content(just_content *content, char value) {
    content->value = value;
    // return OK;
}

typedef union Content {
    just_content just;
} content;

typedef struct WorldContent
{
    char content_type;
    content content;
} world_content;


void init_world_content(world_content *content, char content_type, char param) {
    content->content_type = content_type;
    if (content_type == JUST) {
        init_just_content(&content->content.just, param);
        // return OK;
    } else {
        // return ERROR;
    }
}


// WORLD
typedef struct WorldNode {
    char id;
    
    char next_worlds[MAX_NEXT_WORLDS];
    char num_of_next_worlds;
    char is_terminate;
    
    world_content content;
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

// char max_map_depth = 2;
// //                  0  1  2  3  4  5  6  7  8
// char nodes_x[]   = {0, 1, 1, 2, 2, 0, 0, 1, 2};
// char nodes_y[]   = {1, 0, 2, 0, 1, 2, 0, 1, 2};
// char nodes_out[] = {0, 0, 0, 1, 1, 0, 0, 1, 0};
// char nodes_in[]  = {0, IN_MID, IN_MID, IN_UP, IN_MID | IN_DOWN, 0, 0, IN_MID, 0};
// #define num_of_links 666;
// char links[] = {};
// content_type nodes_content[] = {JUST, JUST, JUST, JUST, JUST, NO_NODE, NO_NODE, SKIP_NODE, NO_NODE};
#include "../data/world_0.h"


char world_x_offset = 0;
char current_world = 1;
world_node world[WORLD_SIZE];

void init_world_node(world_node *node, char id) {
    // char v = (id % 3) - 1; 
    node->id = id;
    node->num_of_next_worlds = 0;
    node->is_terminate = 0;
    init_world_content(
        &node->content,
        nodes_content[id],
        node_args[id]
    );
    // return OK;
}


void generate_world() {
    char i;
    if (WORLD_INITED) {
        // return ERROR;
    }

    for (i = 0; i < WORLD_SIZE; i++) {
        init_world_node(&world[i], i);
    }
    for (i = 0; i < num_of_links; i++) {
        link_nodes(
            &world[links[2*i]], 
            &world[links[2*i + 1]]
        );
    }
    /*    5 0 6
           /|\
          2 7 1
           \|/|
          8 4 3
    */
    // link_nodes(&world[0], &world[1]);
    // link_nodes(&world[0], &world[2]);
    // link_nodes(&world[0], &world[4]);
    // link_nodes(&world[1], &world[3]);
    // link_nodes(&world[1], &world[4]);
    // link_nodes(&world[2], &world[4]);
    
    WORLD_INITED = True;
}

// RENDER
// int render_just_content(just_content *content) {
//     char v;
//     v = content->value;
//     if (v < 0) { 
//         printf("(-)"); 
//     }
//     else if (v > 0) { 
//         printf("(+)"); 
//     }
//     else { 
//         printf("(0)"); 
//     }
//     return OK;
// }

// int render_world_content(world_content *content) {
//     if (content->content_type == JUST) {
//         render_just_content(&content->content.just);
//         return OK;
//     }
//     else {
//         return ERROR;
//     }
// }

// int render_world(world_node *node) {
//     printf("World{%d}", node->id);
//     if (node->is_terminate) {
//         printf("[x]");
//     } else {
//         printf("[>]");
//     }
//     render_world_content(&node->content);
//     printf("\n");
//     return OK;
// }
#endif