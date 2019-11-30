#ifndef __SUP_SP_WING__
#define __SUP_SP_WING__

#define True 1
#define False 0

#define OK 1
#define ERROR 0

#define MAX_WING_SIZE 5
#define NO_SLOT 5

#define OUR_SIDE 0
#define THEIR_SIDE 1

#include "ship.c"
#include "utils.c"

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
    char protector;

    char head;
    char heal;
    char missile;
} wing;

void init_wing(wing *wing) {
    char i;
    for (i = 0; i < 4; i++) {
        wing->ships[i].next_empty_slot = i + 1;
    }
    wing->ships[4].next_empty_slot = NO_SLOT;
    wing->size = 0;
    wing->first_empty_slot = 0;

    wing->protector = NO_SLOT;
    wing->head = 0;
    wing->heal = 0;
    wing->missile = 0;
}

ship* get_ship(wing *wing, char inwing_pos) {
    // assert(wing->size > inwing_pos);
    return &wing->ships[wing->arrange[inwing_pos]].ship;
}

ship* get_leader(wing *wing) {
    return get_ship(wing, 0);
}

char get_most_damaged_ship_i(wing *wing) {
    char i, min_hp, most_damaged;
    ship *current;
    min_hp = 50;
    for (i = 0; i < wing->size; i++) {
        current = get_ship(wing, i);
        if (current->health < min_hp) {
            min_hp = current->health;
            most_damaged = i;
        }
    }
    return most_damaged;
}

// DANGER!!! CAST SCRAP TO ZERO. NOT INTENDED TO USE MID COMBAT
char get_strongest_ship(wing *wing) {
    char max_scrap = 0;
    char i, cur_scrap, p;
    SCRAP = 0; // Badbadnotgood, but i'm lazy
    for (i = 0; i < wing->size; i++) {
        scrap_ship(get_ship(wing, i));
        cur_scrap = collect_scrap();
        if (cur_scrap > max_scrap) {
            p = i;
            max_scrap = cur_scrap;
        }
    }
    return p;
}

void add_ship(wing *wing, char *name, ship_type ship_type) {
    char slot;
    char old_size = wing->size;
    ship *ship;
    // assert(old_size < MAX_WING_SIZE);
    
    // size
    wing->size = old_size + 1;
    
    // ship slot
    slot = wing->first_empty_slot;
    wing->first_empty_slot = wing->ships[slot].next_empty_slot;

    // arrange
    wing->arrange[old_size] = slot;

    // ship init
    ship = &(wing->ships[slot].ship);
    switch (ship_type) {
        case INTERCEPTOR:
            init_interceptor(ship, name);
            break;
        case BOMBER:
            init_bomber(ship, name);
            wing->missile++;
            break;
        case DESTROYER:
            init_destroyer(ship, name);
            break;
        case SUPPORT:
            init_support(ship, name);
            wing->heal++;
            break;
        default:
            break;
    }
}

char install_mod(wing *wing, ship *ship, char mod) {
    if (mod & ship->mods) {
        return ERROR;
    } else {
        switch (mod) {
            case EXTRA_GUNS:
                ship->attack = ship->attack + ATTACK_MOD_BONUS;
                break;
            case EXTRA_SHIELD:
                ship->health = ship->health + HEALTH_MOD_BONUS;
                ship->max_health = ship->max_health + HEALTH_MOD_BONUS;
                break;
            case EXTRA_COMP:
                ship->special = ship->special + SPECIAL_MOD_BONUS;
                break;
            case TORPEDO:
                wing->missile++;
                break;
            case REMTECH:
                wing->heal++;
                break;
            default:
                break;
        }
        // if (mod == TORPEDO) {
            // wing->missile++;
        // } else if (mod == REMTECH) {
            // wing->heal++;
        // }
        ship->mods = ship->mods | mod;
        return OK;
    }
}


void fill_wing_with_rand_ships(wing *wing, char tier1, char tier2, char mods) {
    ship_type t;
    unsigned char i;
    char mod;
    for (i = 0; i < tier1; i++) {
        t = (ship_type) (rand() % 4);
        add_ship(wing, "LOL", t);
    }
    while (mods > 0)
    {
        i = rand() % 7;  // 7 types of mods
        mod = ALL_MODS[i];
        i = rand() % wing->size;
        if (install_mod(wing, get_ship(wing, i), mod) == OK) {
            mods--;
        }
    }
    
}


void remove_ship(wing *wing, char inwing_pos) {
    char slot, i, size;
    char ship_i;
    char old_size = wing->size;
    char m;
    m = get_ship(wing, inwing_pos)->mods;
    if (REMTECH & m) {
        wing->heal--;
    } 
    if (TORPEDO & m) {
        wing->missile--;
    }
    switch (get_ship(wing, inwing_pos)->type) {
        case SUPPORT:
            wing->heal--;
            break;
        case BOMBER:
            wing->missile--;
            break;
        default:
            break;
    }
    // assert(wing->size > inwing_pos);

    ship_i = wing->arrange[inwing_pos];

    // size
    size = old_size - 1;
    wing->size = size;

    // ship slot
    slot = wing->first_empty_slot;
    wing->ships[ship_i].next_empty_slot = slot;
    wing->first_empty_slot = ship_i;

    // protector
    if (ship_i == wing->protector) {
        wing->protector = NO_SLOT;
    }

    // arrange
    for (i = inwing_pos; i < size; i++) {
        wing->arrange[i] =wing->arrange[i + 1];
    }

    // TODO remove bonuses from mods
}

void init_enemy_wing(wing *wing, char power) {
    init_wing(wing);
    switch (power) {
        case 0:
            // fill_wing_with_rand_ships(wing, 2, 0, 0);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 1:
            // fill_wing_with_rand_ships(wing, 2, 0, 2);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 2:
            // fill_wing_with_rand_ships(wing, 3, 0, 2);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 3:
            // fill_wing_with_rand_ships(wing, 0, 2, 3);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 4:
            // fill_wing_with_rand_ships(wing, 4, 1, 0);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 5:
            // fill_wing_with_rand_ships(wing, 5, 0, 4);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 6:
            // fill_wing_with_rand_ships(wing, 5, 0, 7);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 7:
            // fill_wing_with_rand_ships(wing, 3, 2, 5);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 8:
            // fill_wing_with_rand_ships(wing, 0, 4, 5);
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        case 9:
            fill_wing_with_rand_ships(wing, 1, 0, 0);
            break;
        default:
            break;
    }
}

void swap_ships(wing *wing, char left_ship_i, char right_ship_i) {
    char tmp;
    tmp = wing->arrange[left_ship_i];
    wing->arrange[left_ship_i] = wing->arrange[right_ship_i];
    wing->arrange[right_ship_i] = tmp;
}

void cycle_ships(wing *wing) {
    char tmp, i, wing_size;
    tmp = wing->arrange[0];
    wing_size = wing->size - 1;
    for (i = 0; i < wing_size; i++) {
        wing->arrange[i] = wing->arrange[i+1];
    }
    wing->arrange[wing_size] = tmp;
}

char SCRAP = 0;
char collect_scrap() {
    char scrap = SCRAP;
    SCRAP = 0;
    return scrap;
}

void scrap_ship(ship *ship) {
    char i;
    if (ship->tier == 1) {
        SCRAP = SCRAP + TIER_1_SCRAP;
    } else {
        SCRAP = SCRAP + TIER_2_SCRAP;
    }
    for (i = 1; i <= REMTECH; i = i*2) {
        if (i & ship->mods) {
            SCRAP = SCRAP + MOD_SCRAP;
        }
    }
}

void scrap_dead_ships(wing *wing) {
    char i = 0;
    char size = wing->size;
    ship *ship;
    while (i < wing->size)
    {   
        ship = get_ship(wing, i);
        if (ship->is_alive) {
            i++;
        } else if (ship->mods & REBIRTH) {
            SCRAP = SCRAP + MOD_SCRAP;
            ship->health = ship->max_health;
            ship->mods = ship->mods ^ REBIRTH;
            ship->is_alive = True;
        } else {
            scrap_ship(get_ship(wing, i));
            remove_ship(wing, i);
        }
    }
}

void heal_wing(wing *wing, char portion) {
    // heal max_health/portion
    char i;
    ship *ship;
    for (i = 0; i < wing->size; i++) {
        ship = get_ship(wing, i);
        heal(ship, ship->max_health / portion);
    }
}
#endif
