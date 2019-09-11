#include <stdio.h>
#include <string.h>

enum State {
    FLY,
    FIGHT,
};

enum Result {
    OK,
    ERROR,
};

char MAX_SIZE = 5;


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
    return 0;
}

// WING
typedef struct Wing
{
    ship ships[5];
    char arrange[5];
    char size;
    char head;
} wing;

int init_wing(wing *wing) {
    wing->size = 0;
    wing->head = 0;
    return 0;
}

int add_ship(wing *wing, char *name, char health, char attack) {
    char old_size = wing->size;
    if (old_size == MAX_SIZE) {
        return 1;
    }
    wing->size = old_size + 1;
    wing->arrange[old_size] = old_size;
    
    init_ship(
        &(wing->ships[old_size]), 
        name, 
        health, 
        attack
    );
    return 0;
}


// TESTS
int run_tests() {
    wing wing1;
    
    init_wing(&wing1);
    
    return OK;
}

// WORLD

typedef struct WorldNode {
    char next_words[3];
} world_node;

world_node world[3];
int generate_world() {
    world[0].next_words[0] = 1;
    world[0].next_words[1] = 2;
}

// RENDER

// MAIN
int main() {
    int a;
    int tests_result = run_tests();
    if (tests_result == ERROR) {
        return ERROR;
    }

    // init
    wing player_wing;
    init_wing(&player_wing);
    generate_world();

    // main game loop
    for (a = 0; a < 3; a++) {

    }
}