#ifndef __SUP_SP_SHIPS__
#define __SUP_SP_SHIPS__

#include "utils.c"

#define OK 0
#define ERROR 1

#define True 1
#define False 0

#define NO_MODS 0
#define EXTRA_SHIELD 1
#define EXTRA_GUNS 2
#define EXTRA_COMP 4
#define ARMOR 8
#define REBIRTH 16
#define TORPEDO 32
#define REMTECH 64

#define TIER_1_SCRAP 7
#define MOD_SCRAP 3

char ALL_MODS[] = {
    EXTRA_SHIELD, 
    EXTRA_GUNS, 
    EXTRA_COMP, 
    ARMOR, 
    REBIRTH, 
    TORPEDO, 
    REMTECH
};

typedef enum ShipType {
    INTERCEPTOR,
    BOMBER,
    DESTROYER,
    SUPPORT,
    BOSS,
    NOT_A_SHIP,
} ship_type;

typedef struct SpaceShip
{
    char name[11];
    ship_type type;
    char tier;

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
    // char tier, 
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
    ship->tier = 1;
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

void heal(ship *ship, char amount) {
    char result_health;
    result_health = ship->health + amount;
    if (result_health > ship->max_health) {
        ship->health = ship->max_health;
    }
    else {
        ship->health = result_health;
    }
}

char upgrade_ship(ship *ship) {
    // char old_mods;
    // old_mods = ship->mods;
    if (ship->tier > 1) {
        return ERROR;
    } else {
        return OK;
    }
}

#endif
