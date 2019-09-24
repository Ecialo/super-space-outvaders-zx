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

enum ShipType {
    INTERCEPTOR,
    BOMBER,
    DESTROYER,
    SUPPORT,
    BOSS,
    NOT_A_SHIP,
};

enum ShipMods {
    NO_MODS = 0,
    TORPEDO = 1,
    REMTECH = 2,
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

char monus(char a, char b) {
    if (a > b) {
        return a - b;
    } else {
        return 0;
    }
}


// SHIP
typedef struct Spacehip
{
    char name[11];
    char type;

    char max_health;
    char health;

    char attack;
    char special;

    char mods;
    char is_alive;
} ship;

int init_ship(
    ship *ship, 
    char *name,
    char type, 
    char health, 
    char attack,
    char special,
    char mods
) {
    strcpy(ship->name, name);
    ship->max_health = health;
    ship->health = health;
    ship->attack = attack;
    ship->special = special;
    ship->mods = mods;
    ship->is_alive = True;
    return OK;
}

int init_interceptor(ship *ship, char *name) {
    init_ship(
        ship,
        name,
        INTERCEPTOR,
        10,
        3,
        1,
        NO_MODS
    );
    return OK;
}

int init_bomber(ship *ship, char *name) {
    init_ship(
        ship,
        name,
        BOMBER,
        8,
        2,
        3,
        NO_MODS
    );
    return OK;
}

int init_destroyer(ship *ship, char *name) {
    init_ship(
        ship,
        name,
        DESTROYER,
        12,
        2,
        1,
        NO_MODS
    );
    return OK;
}

int init_support(ship *ship, char *name) {
    init_ship(
        ship,
        name,
        SUPPORT,
        8,
        1,
        3,
        NO_MODS
    );
    return OK;
}

int take_damage(ship *ship, char amount, char multipler, char source_type) {
    char total_damage;
    char type = ship->type;
    total_damage = amount * multipler;
    if (type == INTERCEPTOR && source_type == DESTROYER) {
        total_damage = total_damage + amount;
    } else if (type == BOMBER && source_type == INTERCEPTOR) {
        total_damage = total_damage + amount;
    } else if (type == DESTROYER) {
        if (source_type == BOMBER) {
            total_damage = total_damage + amount;
        } 
        total_damage = total_damage - ship->special;
    }
    ship->health = monus(ship->health, total_damage);
    if (ship->health == 0) {
        ship->is_alive = False;
    }
    return OK;
}

int heal(ship *ship, char amount) {
    char result_health;
    result_health = ship->health + amount;
    if (result_health > ship->max_health) {
        ship->health = ship->max_health;
    }
    else {
        ship->health = result_health;
    }
    return OK;
}


// WING
typedef struct Wing
{
    ship ships[MAX_WING_SIZE];
    char arrange[MAX_WING_SIZE];
    char size;
    char head;
    char heal;
    char missile;
} wing;

int init_wing(wing *wing) {
    wing->size = 0;
    wing->head = 0;
    wing->heal = 0;
    wing->missile = 0;
    return OK;
}

int add_ship(wing *wing, char *name, char type) {
    char old_size = wing->size;
    ship *ship;
    if (old_size == MAX_WING_SIZE) {
        return ERROR;
    }
    wing->size = old_size + 1;
    wing->arrange[old_size] = old_size;
    ship = &(wing->ships[old_size]);
    
    if (type == INTERCEPTOR) { init_interceptor(ship, name); }
    else if (type == BOMBER) { init_bomber(ship, name); }
    else if (type == DESTROYER) { init_destroyer(ship, name); }
    else if (type == SUPPORT) { init_support(ship, name); }
    else { return ERROR; }
    
    return OK;
}

int init_enemy_wing(wing *wing) {
    return OK;
}

int swap_ships(wing *wing, char left_ship_i, char right_ship_i) {
    char tmp;
    tmp = wing->arrange[left_ship_i];
    wing->arrange[left_ship_i] = wing->arrange[right_ship_i];
    wing->arrange[right_ship_i] = tmp;
    return OK;
}

int cycle_ships(wing *wing) {
    char tmp, i, wing_size;
    tmp = wing->arrange[0];
    wing_size = wing->size - 1;
    for (i = 0; i < wing_size; i++) {
        wing->arrange[i] = wing->arrange[i+1];
    }
    wing->arrange[wing_size] = tmp;
    return OK;
}

// GLOBAL GAME STATE
typedef struct GameState
{
    char state;
    char current_world;
    wing player_wing;
    char combat_round;
} state;

int init_state(state *state) {
    state->state = IDLE;
    state->current_world = 0;
    init_wing(&state->player_wing);
    state->combat_round = 0;
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

// RENDER
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


// READ
char read_action() {
    int a;
    printf("0 -- shoot, 1 -- retreat, 2 -- special");
    scanf("%d", &a);
    return (char) a;
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

int perform_round(
    wing *player, 
    wing *enemy, 
    char damage_multipler,
    char player_action,
    char enemy_action
) {
    char i, wing_size, result_effect;
    wing_size = player->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            &(player->ships[player->arrange[i]]), 
            player->heal
        );
    }
    wing_size = enemy->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            &(enemy->ships[enemy->arrange[i]]), 
            enemy->heal
        );
    }

    result_effect = player->heal - (enemy->missile) * 2;
    if (result_effect > 0) {
        heal(
            &(player->ships[player->arrange[0]]), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            &(player->ships[player->arrange[0]]), 
            result_effect,
            damage_multipler,
            NOT_A_SHIP
        );
    }

    result_effect = enemy->heal - (player->missile) * 2;
    if (result_effect > 0) {
        heal(
            &(enemy->ships[enemy->arrange[0]]), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            &(enemy->ships[enemy->arrange[0]]), 
            result_effect,
            damage_multipler,
            NOT_A_SHIP
        );
    }

    // TODO COMPLETE PERFORMER
}

int perform_fight(state *state) {

    char action;
    wing player_wing = state->player_wing;
    wing enemy_wing;
    init_enemy_wing(&enemy_wing);

    printf("You wing");
    render_wing(&player_wing);
    printf("Enemy wing");
    render_wing(&enemy_wing);

    action = read_action();


    return OK;
}

int perform_idle(state *state) {
    world_node node = world[state->current_world];
    render_world(&node);
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
        else if (state.state == IDLE) { perform_idle(&state); }
        else if (state.state = VICTORY) {
            render_victory();
            break;
        }
        printf("\n");
    }
}