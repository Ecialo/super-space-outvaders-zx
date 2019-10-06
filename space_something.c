#include <stdio.h>
#include <string.h>
#include <assert.h>

#define True 1
#define False 0

#define OK 0
#define ERROR 1

#define MAX_WING_SIZE 5
#define NO_SLOT 5

#define MAX_NEXT_WORLDS 3
#define WORLD_SIZE 5

#define NO_MODS 0
#define TORPEDO 1
#define REMTECH 2

#define NUM_OF_ACTIONS 3
#define NO_CHOICE 7

typedef enum State {
    IDLE,
    FLY,
    FIGHT,
    VICTORY,
    DEFEAT,
} state;

typedef enum ShipType {
    INTERCEPTOR,
    BOMBER,
    DESTROYER,
    SUPPORT,
    BOSS,
    NOT_A_SHIP,
} ship_type;

typedef enum ContentType {
    JUST,
} content_type;

typedef enum Action {
    ATTACK,
    RETREAT,
    SPECIAL,
} action;

typedef enum FightResults {
    CONTINUE,
    THEY,
    WE,
} fight_result;


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
typedef struct SpaceShip
{
    char name[11];
    ship_type type;

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
    ship_type type, 
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
    ship->type = type;
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

int take_damage(ship *ship, char amount, char multiplier, ship_type source_type) {
    char total_damage;
    ship_type type = ship->type;
    total_damage = amount * multiplier;
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
typedef union ShipSlot
{
    ship ship;
    char next_empty_slot;
} ship_slot;


typedef struct Wing
{
    ship_slot ships[MAX_WING_SIZE];
    char arrange[MAX_WING_SIZE];
    char size;
    char first_empty_slot;

    char head;
    char heal;
    char missile;
} wing;

int init_wing(wing *wing) {
    wing->ships[0].next_empty_slot = 1;
    wing->ships[1].next_empty_slot = 2;
    wing->ships[2].next_empty_slot = 3;
    wing->ships[3].next_empty_slot = 4;
    wing->ships[4].next_empty_slot = NO_SLOT;
    wing->size = 0;
    wing->first_empty_slot = 0;

    wing->head = 0;
    wing->heal = 0;
    wing->missile = 0;
    return OK;
}

ship* get_ship(wing *wing, char inwing_pos) {
    assert(wing->size > inwing_pos);
    return &wing->ships[wing->arrange[inwing_pos]].ship;
}

ship* get_leader(wing *wing) {
    return get_ship(wing, 0);
}

int add_ship(wing *wing, char *name, ship_type ship_type) {
    char slot;
    char old_size = wing->size;
    ship *ship;
    assert(old_size < MAX_WING_SIZE);
    
    // size
    wing->size = old_size + 1;
    
    // ship slot
    slot = wing->first_empty_slot;
    wing->first_empty_slot = wing->ships[slot].next_empty_slot;

    // arrange
    wing->arrange[old_size] = slot;

    // ship init
    ship = &(wing->ships[slot].ship);
    if (ship_type == INTERCEPTOR) { init_interceptor(ship, name); }
    else if (ship_type == BOMBER) { init_bomber(ship, name); }
    else if (ship_type == DESTROYER) { init_destroyer(ship, name); }
    else if (ship_type == SUPPORT) { init_support(ship, name); }
    else { return ERROR; }
    
    return OK;
}

int remove_ship(wing *wing, char inwing_pos) {
    char slot, i, size;
    char ship_i;
    char old_size = wing->size;
    assert(wing->size > inwing_pos);

    ship_i = wing->arrange[inwing_pos];

    // size
    size = old_size - 1;
    wing->size = size;

    // ship slot
    slot = wing->first_empty_slot;
    wing->ships[ship_i].next_empty_slot = slot;
    wing->first_empty_slot = ship_i;

    // arrange
    for (i = inwing_pos; i < size; i++) {
        wing->arrange[i] =wing->arrange[i + 1];
    }

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

int scrap_dead_ships(wing *wing) {
    char i = 0;
    char size = wing->size;
    while (i < wing->size)
    {
        if (get_ship(wing, i)->is_alive) {
            i++;
        } else {
            remove_ship(wing, i);
        }
    }
}

// GLOBAL GAME STATE
typedef struct GameState
{
    char state;
    char current_world;
    wing player_wing;
    char combat_round;
} game_state;

int init_state(game_state *state) {
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

int render_ship(ship *ship) {
    ship_type type;
    printf("SHIP\n");
    printf("NAME: %s\n", ship->name);
    printf("TYPE: ");
    type = ship->type;
    // printf("RAW TYPE %d\n", (int)type);
    if (type == INTERCEPTOR) { printf("ITERCEPTOR\n"); }
    else if (type == BOMBER) { printf("BOMBER\n"); }
    else if (type == DESTROYER) { printf("DESTROYER\n"); }
    else if (type == SUPPORT) { printf("SUPPORT\n"); }
    else { printf("UNKNOWN TYPE\n"); }
    printf("ATTACK %d\n", ship->attack);
    printf("HEALTH %d/%d\n", ship->health, ship->max_health);
    printf("---\n");
    return OK;
}

int render_wing(wing *wing) {
    char i;
    printf("WING\n");
    printf("Size %d fempty %d\n", wing->size, wing->first_empty_slot);
    for (i = 0; i < wing->size; i++) {
        printf("Ship #%d\n", i);
        render_ship(get_ship(wing, i));
    }
    printf("-----\n");
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

char read_ship_i(wing *wing) {
    int a;
    printf("Choose ship from \n");
    render_wing(wing);
    scanf("%d", &a);
    return a;
}

// ACTIONS
int perform_fly(game_state *state) {

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

fight_result check_fight_result(wing *player, wing *enemy) {
    if (player->size == 0) {
        return THEY;
    } else if (enemy->size == 0) {
        return WE;
    } else {
        return CONTINUE;
    }
}

int perform_action(wing *you, wing *oppose, action action, char choice, char damage_multiplier) {
    ship *you_leader = get_leader(you);
    ship *oppose_leader = get_leader(oppose);
    
    switch (action) {
        case ATTACK:
            take_damage(
                oppose_leader, 
                you_leader->attack, 
                damage_multiplier,
                you_leader->type
            );
            break;
        case RETREAT:
            cycle_ships(you);
            if (you_leader->type == INTERCEPTOR) {
                take_damage(
                    oppose_leader, 
                    you_leader->special, 
                    damage_multiplier,
                    NOT_A_SHIP
                );  
            }
            break;
        case SPECIAL:
            switch (you_leader->type) {
                case INTERCEPTOR:
                    swap_ships(oppose, 0, choice);
                    break;
                case BOMBER:
                    take_damage(
                        get_ship(oppose, choice), 
                        you_leader->special, 
                        damage_multiplier,
                        NOT_A_SHIP
                    );
                    break;
                case DESTROYER:
                    swap_ships(you, 0, 1);
                    // TODO RAISE DESTROYER SHIELD
                    break;  
                case SUPPORT:
                    heal(
                        get_ship(you, choice),
                        you_leader->special
                    );
                    break;
                default:
                    assert(0);
                    break;
            }
            break;
        default:
            assert(0);
            break;
    }
}

int perform_round(
    wing *player, 
    wing *enemy, 
    char damage_multiplier,
    action player_action,
    char player_choice,
    action enemy_action,
    char enemy_choice
) {
    char i, wing_size, result_effect;
    ship player_leader, enemy_leader;

    wing_size = player->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            // &(player->ships[player->arrange[i]]),
            get_ship(player, i), 
            player->heal
        );
    }
    wing_size = enemy->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            // &(enemy->ships[enemy->arrange[i]]), 
            get_ship(enemy, i), 
            enemy->heal
        );
    }

    result_effect = player->heal - (enemy->missile) * 2;
    if (result_effect > 0) {
        heal(
            // &(player->ships[player->arrange[0]]), 
            get_leader(player), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            // &(player->ships[player->arrange[0]]), 
            get_leader(player), 
            result_effect,
            damage_multiplier,
            NOT_A_SHIP
        );
    }

    result_effect = enemy->heal - (player->missile) * 2;
    if (result_effect > 0) {
        heal(
            // &(enemy->ships[enemy->arrange[0]]), 
            get_leader(enemy), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            // &(enemy->ships[enemy->arrange[0]]), 
            get_leader(enemy), 
            result_effect,
            damage_multiplier,
            NOT_A_SHIP
        );
    }
    player_leader = *get_leader(player);
    enemy_leader = *get_leader(enemy);

    perform_action(player, enemy, player_action, player_choice, damage_multiplier);
    perform_action(enemy, player, enemy_action, enemy_choice, damage_multiplier);
    // TODO COMPLETE PERFORMER
}

int perform_fight(game_state *state) {

    action player_action, enemy_action; 
    char player_choice, enemy_choice;
    fight_result fight_result;
    wing player_wing = state->player_wing;
    
    wing enemy_wing;
    init_enemy_wing(&enemy_wing);

    for(;;) {

        printf("You wing");
        render_wing(&player_wing);
        printf("Enemy wing");
        render_wing(&enemy_wing);

        player_action = read_action();
        if (player_action == SPECIAL) {
            switch (get_leader(&player_wing)->type) {
                case BOMBER:
                case INTERCEPTOR:
                    player_choice = read_ship_i(&enemy_wing);
                    break;
                case SUPPORT:
                    player_choice = read_ship_i(&player_wing);
                    break;
                default:
                    player_choice = NO_CHOICE;
                    break;
            }
        } else {
            player_choice = NO_CHOICE;
        }

        enemy_action = (action) (rnd() % NUM_OF_ACTIONS);
        if (enemy_action == SPECIAL) {
            switch (get_leader(&player_wing)->type) {
                case BOMBER:
                    player_choice = (char) (rnd() % player_wing.size);
                    break;
                case SUPPORT:
                case INTERCEPTOR:
                    enemy_choice = (char) (rnd() % enemy_wing.size);
                    break;
                default:
                    enemy_choice = NO_CHOICE;
                    break;
            }
        } else {
            enemy_choice = NO_CHOICE;
        }

        perform_round(
            &player_wing,
            &enemy_wing,
            (state->combat_round / 10) + 1,
            player_action,
            player_choice,
            enemy_action,
            enemy_choice
        );
        scrap_dead_ships(&player_wing);
        scrap_dead_ships(&enemy_wing);
        fight_result = check_fight_result(&player_wing, &enemy_wing);
        if (fight_result == CONTINUE) {
            state->combat_round++;
        } else if (fight_result == WE) {
            state->state = IDLE;
            break;
        } else if (fight_result == THEY) {
            state->state = DEFEAT;
            break;
        }
    }
    return OK;
}

int perform_idle(game_state *state) {
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

int test_wing_ship_add() {
    wing wing1;
    ship s0, s1, s2, s3, s4;
    char a0, a1, a2, a3, a4;
    init_wing(&wing1);

    // render_wing(&wing1);    // [[*], [], [], [], []]
    assert(wing1.size == 0);
    assert(wing1.first_empty_slot == 0);
    
    add_ship(&wing1, "OLOLO", INTERCEPTOR); // [O, [*], [], [], []]
    // render_wing(&wing1);
    assert(wing1.size == 1);
    assert(wing1.first_empty_slot == 1);

    add_ship(&wing1, "KEKEKE", BOMBER); // [O, B, [*], [], []]
    // render_wing(&wing1);
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 2);

    add_ship(&wing1, "1488", DESTROYER); // [O, B, 1, [*], []]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 3);

    add_ship(&wing1, "666", SUPPORT); // [O, B, 1, 6, [*]]
    // render_wing(&wing1);
    assert(wing1.size == 4);
    assert(wing1.first_empty_slot == 4);

    add_ship(&wing1, "HITLER", INTERCEPTOR); // [O, B, 1, 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 5);
    assert(wing1.first_empty_slot == NO_SLOT);

    remove_ship(&wing1, 1); // [O, [*], 1, 6, H] [O, 1, 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 4);
    assert(wing1.first_empty_slot == 1);
    
    remove_ship(&wing1, 1); // [O, [], [*], 6, H] [O 6, H]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 2);
    assert(wing1.ships[2].next_empty_slot == 1);

    remove_ship(&wing1, 2); // [O, [], [], 6, [*]] [O, 6]
    // render_wing(&wing1);
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 4);
    assert(wing1.ships[4].next_empty_slot == 2);
    assert(wing1.ships[2].next_empty_slot == 1);
    assert(wing1.ships[1].next_empty_slot == NO_SLOT);

    add_ship(&wing1, "1488", DESTROYER); // [O, [], [*], 6, 1] [O, 6, 1]
    // render_wing(&wing1);
    assert(wing1.size == 3);
    assert(wing1.first_empty_slot == 2);

    remove_ship(&wing1, 0); // [[*], [], [], 6, 1] [6, 1]
    assert(wing1.size == 2);
    assert(wing1.first_empty_slot == 0);
    assert(wing1.ships[0].next_empty_slot == 2);
    
    remove_ship(&wing1, 0); // [[], [], [], [*], 1] [1]
    assert(wing1.size == 1);
    assert(wing1.first_empty_slot == 3);
    assert(wing1.ships[3].next_empty_slot == 0);

    remove_ship(&wing1, 0); // [[], [], [], [], [*]]
    assert(wing1.size == 0);
    assert(wing1.first_empty_slot == 4);
    assert(wing1.ships[4].next_empty_slot == 3);
    
}

int test_wing_swap_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);

    swap_ships(&wing, 0, 1); // [B, I, D, S]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);

    swap_ships(&wing, 0, 0);
    assert(get_ship(&wing, 0)->type == BOMBER);

    swap_ships(&wing, 3, 0); // [S, I, D, B]
    assert(get_ship(&wing, 0)->type == SUPPORT);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);
    assert(get_ship(&wing, 2)->type == DESTROYER);
    assert(get_ship(&wing, 3)->type == BOMBER);
}

int test_wing_cycle_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);
    // [I, B, D, S]

    cycle_ships(&wing); // [B, D, S, I]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == DESTROYER);
    assert(get_ship(&wing, 2)->type == SUPPORT);
    assert(get_ship(&wing, 3)->type == INTERCEPTOR);

    cycle_ships(&wing); // [D, S, I, B]
    assert(get_ship(&wing, 0)->type == DESTROYER);
    assert(get_ship(&wing, 1)->type == SUPPORT);
    assert(get_ship(&wing, 2)->type == INTERCEPTOR);
    assert(get_ship(&wing, 3)->type == BOMBER);

    cycle_ships(&wing); // [S, I, B, D]
    assert(get_ship(&wing, 0)->type == SUPPORT);
    assert(get_ship(&wing, 1)->type == INTERCEPTOR);
    assert(get_ship(&wing, 2)->type == BOMBER);
    assert(get_ship(&wing, 3)->type == DESTROYER);

    cycle_ships(&wing); // [I, B, D, S]
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == BOMBER);
    assert(get_ship(&wing, 2)->type == DESTROYER);
    assert(get_ship(&wing, 3)->type == SUPPORT);

    cycle_ships(&wing); // [B, D, S, I]
    assert(get_ship(&wing, 0)->type == BOMBER);
    assert(get_ship(&wing, 1)->type == DESTROYER);
    assert(get_ship(&wing, 2)->type == SUPPORT);
    assert(get_ship(&wing, 3)->type == INTERCEPTOR);
}

int test_scrap_ships() {
    wing wing;
    init_wing(&wing);
    add_ship(&wing, "OLOLO", INTERCEPTOR);
    add_ship(&wing, "KEKEKE", BOMBER);
    add_ship(&wing, "1488", DESTROYER);
    add_ship(&wing, "666", SUPPORT);

    get_ship(&wing, 1)->is_alive = False;
    get_ship(&wing, 3)->is_alive = False;

    scrap_dead_ships(&wing);

    assert(wing.size == 2);
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == DESTROYER);

    scrap_dead_ships(&wing);

    assert(wing.size == 2);
    assert(get_ship(&wing, 0)->type == INTERCEPTOR);
    assert(get_ship(&wing, 1)->type == DESTROYER);
}

int test_take_damage() {
    ship i, b, d, s;

    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, DESTROYER);
    assert(i.health == i.max_health - 2);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, INTERCEPTOR);
    assert(i.health == i.max_health - 1);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, BOMBER);
    assert(i.health == i.max_health - 1);
    init_interceptor(&i, "Int");
    take_damage(&i, 1, 1, SUPPORT);
    assert(i.health == i.max_health - 1);

    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, DESTROYER);
    assert(b.health == b.max_health - 1);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, INTERCEPTOR);
    assert(b.health == b.max_health - 2);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, BOMBER);
    assert(b.health == b.max_health - 1);
    init_bomber(&b, "Bom");
    take_damage(&b, 1, 1, SUPPORT);
    assert(b.health == b.max_health - 1);

    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, DESTROYER);
    assert(d.health == d.max_health - (3 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, INTERCEPTOR);
    assert(d.health == d.max_health - (3 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, BOMBER);
    assert(d.health == d.max_health - (6 - d.special));
    init_destroyer(&d, "Des");
    take_damage(&d, 3, 1, SUPPORT);
    assert(d.health == d.max_health - (3 - d.special));

    init_support(&s, "Sup");
    take_damage(&s, 1, 1, DESTROYER);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, INTERCEPTOR);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, BOMBER);
    assert(s.health == s.max_health - 1);
    init_support(&s, "Sup");
    take_damage(&s, 1, 1, SUPPORT);
    assert(s.health == s.max_health - 1);

    init_bomber(&b, "Bom");
    take_damage(&b, 2, 2, DESTROYER);
    assert(b.health == b.max_health - 4);
    init_bomber(&b, "Bom");
    take_damage(&b, 2, 2, INTERCEPTOR);
    assert(b.health == b.max_health - 6);

    init_bomber(&b, "Bom");
    assert(b.is_alive);
    take_damage(&b, 100, 1, DESTROYER);
    assert(b.health == 0);
    assert(!b.is_alive);

}

int test_heal() {
    ship a;
    init_interceptor(&a, "Int");
    take_damage(&a, 3, 1, INTERCEPTOR);
    heal(&a, 1);
    assert(a.health == a.max_health - 2);
    heal(&a, 5);
    assert(a.health == a.max_health);
}

int run_tests() {
    test_random();
    test_wing_ship_add();
    test_wing_swap_ships();
    test_wing_cycle_ships();
    test_scrap_ships();
    test_take_damage();
    test_heal();
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
    // state state;
    // init_state(&state);
    // generate_world();

    // // main game loop
    // for (a = 0; a < 10; a++) {
    //     if (state.state == FLY) { perform_fly(&state); }
    //     else if (state.state == FIGHT) { perform_fight(&state); }
    //     else if (state.state == IDLE) { perform_idle(&state); }
    //     else if (state.state = VICTORY) {
    //         render_victory();
    //         break;
    //     }
    //     printf("\n");
    // }
}