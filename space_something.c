#include <stdio.h>
#include <string.h>
#include <assert.h>

#define True 1
#define False 0
#define MAX_WING_SIZE 5
#define MAX_NEXT_WORLDS 3
#define WORLD_SIZE 5

enum State {
    IDLE,
    FLY,
    FIGHT,
    VICTORY,
};

enum Result {
    OK,
    ERROR,
};

enum ContentType {
    JUST,
};


// TOOLS

unsigned char RANDOM_HEAD = 255;
unsigned char RANDOM[] = {
    106, 162, 190, 251, 249, 243, 34, 246, 37, 215, 62, 
    147, 97, 176, 148, 150, 64, 80, 177, 52, 22, 247, 43, 
    133, 226, 125, 9, 104, 141, 77, 194, 248, 172, 241, 182, 90, 
    48, 38, 93, 238, 58, 38, 158, 249, 3, 154, 205, 72, 59, 222, 
    178, 223, 104, 188, 218, 34, 95, 160, 147, 253, 222, 184, 
    218, 147, 238, 155, 7, 41, 216, 29, 251, 200, 162, 14, 199, 
    184, 99, 176, 115, 67, 200, 7, 63, 208, 183, 115, 215, 96, 68, 
    184, 59, 125, 95, 209, 85, 35, 161, 51, 115, 5, 249, 197, 221, 
    151, 119, 74, 138, 164, 115, 114, 212, 53, 99, 46, 203, 199, 150, 68, 
    179, 50, 94, 134, 104, 254, 9, 76, 220, 32, 55, 37, 123, 31, 87, 40, 135, 
    122, 12, 133, 120, 27, 125, 151, 184, 146, 242, 149, 200, 46, 38, 1, 253, 
    15, 36, 232, 242, 203, 186, 36, 148, 241, 27, 41, 13, 34, 122, 63, 194, 
    198, 13, 174, 108, 45, 83, 154, 245, 74, 43, 30, 242, 160, 139, 138, 65, 
    4, 89, 159, 255, 78, 250, 204, 83, 89, 125, 218, 160, 98, 85, 232, 66, 
    72, 25, 220, 140, 191, 178, 53, 250, 240, 125, 195, 9, 79, 110, 147, 
    26, 193, 166, 0, 28, 132, 236, 45, 107, 222, 252, 206, 153, 236, 
    141, 141, 16, 81, 145, 213, 138, 4, 147, 6, 214, 116, 44, 210, 199, 15, 
    131, 111, 159, 194, 154, 56, 117, 117, 150, 3, 235
};
unsigned char rnd() {
    RANDOM_HEAD++;
    return RANDOM[RANDOM_HEAD];
}


// SHIP
typedef struct Spacehip
{
    char name[11];
    char max_health;
    char health;
    char attack;
} ship;

int init_ship(ship *ship, char *name, char health, char attack) {
    ship->attack = attack;
    ship->max_health = health;
    ship->health = health;
    strcpy(ship->name, name);
    return OK;
}


// WING
typedef struct Wing
{
    ship ships[MAX_WING_SIZE];
    char arrange[MAX_WING_SIZE];
    char size;
    char head;
} wing;

int init_wing(wing *wing) {
    wing->size = 0;
    wing->head = 0;
    return OK;
}

int add_ship(wing *wing, char *name, char health, char attack) {
    char old_size = wing->size;
    if (old_size == MAX_WING_SIZE) {
        return ERROR;
    }
    wing->size = old_size + 1;
    wing->arrange[old_size] = old_size;
    
    init_ship(
        &(wing->ships[old_size]), 
        name, 
        health, 
        attack
    );
    return OK;
}

// GLOBAL GAME STATE
typedef struct GameState
{
    char state;
    char current_world;
    wing player_wing;
} state;

int init_state(state *state) {
    state->state = FLY;
    state->current_world = 0;
    init_wing(&state->player_wing);
    return OK;
}

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

int render_just_content(just_content *content) {
    char v = content->value;
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

// RENDER
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

int render_wing(wing *wing) {
    printf("wing contain %d ships", wing->size);
    return OK;
}

int render_ship(ship *ship) {
    return OK;
}

int render_victory() {
    printf("HOORAY!\n");
    return OK;
}


// ACTIONS
int perform_fly(state *state) {

    world_node node = world[state->current_world];

    if (node.is_terminate) {
        state->state = VICTORY;
        return OK;
    }

    char i, num_of_next_worlds;
    int d;
    num_of_next_worlds = node.num_of_next_worlds;
    printf("Currently we are in ");
    render_world(&node);
    printf("We can fly to:\n");
    for (i = 0; i < num_of_next_worlds; i++) {
        printf("%d - ", i);
        render_world(&world[node.next_worlds[i]]);
    }
    printf("Choose\n");
    scanf("%i", &d);
    // d = d - '0';
    if (d >= 0 && d < num_of_next_worlds) {
        state->current_world = node.next_worlds[d];
    }
    return OK;
}

int perform_fight() {
    printf("BOOP\n");
    return OK;
}


// TESTS

int test_random() {
    int i;
    unsigned char rnd_result;
    assert(rnd() == 106);
    for (i = 0; i < 255; i++) {
        rnd();
    }
    assert(rnd() == 106);
    assert(rnd() == 162);
    return OK;
}

int run_tests() {
    test_random();
    // wing wing1;
    
    // init_wing(&wing1);
    
    return OK;
}


// MAIN
int main() {
    int a;
    int tests_result = run_tests();
    if (tests_result == ERROR) {
        return ERROR;
    }

    // init
    state state;
    init_state(&state);
    generate_world();

    // main game loop
    for (a = 0; a < 10; a++) {
        if (state.state == FLY) { perform_fly(&state); }
        else if (state.state == FIGHT) { perform_fight(&state); }
        else if (state.state = VICTORY) {
            render_victory();
            break;
        }
        printf("\n");
    }
}