#ifndef __SUP_SP_SHIPS__
#define __SUP_SP_SHIPS__

#define OK 0
#define ERROR 1

#define True 1
#define False 0

#define NO_MODS 0
#define TORPEDO 1
#define REMTECH 2

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
#endif
