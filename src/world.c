// WORLD CONTENT

typedef struct JustContent
{
    char value;
} just_content;

int init_just_content(just_content *content, char value) {
    content->value = value;
    return OK;
}

typedef union Content {
    just_content just;
} content;

typedef struct WorldContent
{
    char content_type;
    content content;
} world_content;


int init_world_content(world_content *content, char content_type, void *param) {
    content->content_type = content_type;
    if (content_type == JUST) {
        init_just_content(&content->content.just, *(char*)param);
        return OK;
    } else {
        return ERROR;
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

int init_world_node(world_node *node, char id) {
    char v = (id % 3) - 1; 
    node->id = id;
    node->num_of_next_worlds = 0;
    node->is_terminate = True;
    init_world_content(
        &node->content,
        JUST,
        &v
    );
    return OK;
}

int link_nodes(world_node *from, world_node *to) {
    char old_num_of_next_worlds = from->num_of_next_worlds;
    if (old_num_of_next_worlds == MAX_NEXT_WORLDS) {
        return ERROR;
    }
    from->next_worlds[from->num_of_next_worlds] = to->id;
    from->num_of_next_worlds = old_num_of_next_worlds + 1;
    from->is_terminate = False;
    return OK;
}

// WORLD INIT
char WORLD_INITED = False;
world_node world[WORLD_SIZE];
int generate_world() {
    char i;
    if (WORLD_INITED) {
        return ERROR;
    }

    for (i = 0; i < WORLD_SIZE; i++) {
        init_world_node(&world[i], i);
    }
    link_nodes(&world[0], &world[1]);
    link_nodes(&world[0], &world[2]);
    link_nodes(&world[1], &world[3]);
    link_nodes(&world[1], &world[4]);
    link_nodes(&world[2], &world[4]);
    
    WORLD_INITED = True;

    return OK;
}

// RENDER
int render_just_content(just_content *content) {
    char v;
    v = content->value;
    if (v < 0) { 
        printf("(-)"); 
    }
    else if (v > 0) { 
        printf("(+)"); 
    }
    else { 
        printf("(0)"); 
    }
    return OK;
}

int render_world_content(world_content *content) {
    if (content->content_type == JUST) {
        render_just_content(&content->content.just);
        return OK;
    }
    else {
        return ERROR;
    }
}

int render_world(world_node *node) {
    printf("World{%d}", node->id);
    if (node->is_terminate) {
        printf("[x]");
    } else {
        printf("[>]");
    }
    render_world_content(&node->content);
    printf("\n");
    return OK;
}
