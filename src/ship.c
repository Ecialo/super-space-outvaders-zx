#ifndef __SUP_SP_SHIPS__
#define __SUP_SP_SHIPS__

#include "types.h"
#include "utils.c"

#define OK 1
#define ERROR 0

#define True 1
#define False 0

#define NO_MODS 0
#define EXTRA_SHIELD 2
#define EXTRA_GUNS 1
#define EXTRA_COMP 4
#define ARMOR 32
#define REBIRTH 64
#define TORPEDO 8
#define REMTECH 16

#define TIER_1_SCRAP 7
#define TIER_2_SCRAP 13
#define MOD_SCRAP 3

#define ATTACK_MOD_BONUS 2
#define HEALTH_MOD_BONUS 3
#define SPECIAL_MOD_BONUS 1

#define TROLL_CLASS 1
#define TSUNAMI_CLASS 0

uch *intr_names[] = {
    "RAPTOR",
    "VIPER"
};

uch *bomb_names[] = {
    "SHOOTA",
    "SKORCHA"
};

uch *dest_names[] = {
    "TITAN",
    "DEMIGOD"
};

uch *sup_names[] = {
    "GEEK",
    "NERD"
};

uch *boss_names[] = {
    "TSUNAMI",
    "TROLL"
};

uch ALL_MODS[] = {
    EXTRA_GUNS,
    EXTRA_SHIELD,  
    EXTRA_COMP, 
    TORPEDO,
    REMTECH,
    ARMOR, 
    REBIRTH 
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
    uch *name;
    ship_type type;
    uch tier;

    uch max_health;
    uch health;

    uch attack;
    uch special;

    uch mods;
    uch is_alive;
} ship;

void init_ship(
    ship *ship, 
    uch *name,
    ship_type type,
    // uch tier, 
    uch health, 
    uch attack,
    uch special,
    uch mods
) {
    ship->name = name;
    ship->max_health = health;
    ship->health = health;
    ship->attack = attack;
    ship->special = special;
    ship->mods = mods;
    ship->is_alive = True;
    ship->type = type;
    ship->tier = 1;
}

void init_interceptor(ship *ship) {
    init_ship(
        ship,
        intr_names[0],
        INTERCEPTOR,
        10,
        3,
        1,
        NO_MODS
    );
}

void init_bomber(ship *ship) {
    init_ship(
        ship,
        bomb_names[0],
        BOMBER,
        9,
        2,
        3,
        NO_MODS
    );
}

void init_destroyer(ship *ship) {
    init_ship(
        ship,
        dest_names[0],
        DESTROYER,
        12,
        2,
        1,
        NO_MODS
    );
}

void init_support(ship *ship) {
    init_ship(
        ship,
        sup_names[0],
        SUPPORT,
        8,
        1,
        3,
        NO_MODS
    );
}

void init_boss(ship *ship, uch boss_class) {
    if (boss_class == TROLL_CLASS) {
        init_ship(
            ship,
            boss_names[1],
            BOSS,
            35,
            8,
            0,
            NO_MODS
        );
        ship->tier = 2;
    } else {
        init_ship(
            ship,
            boss_names[0],
            BOSS,
            40,
            0,
            0,
            NO_MODS
        );
    }
}

uch promote_ship(ship* ship) {
    if (ship->tier > 1) {
        return ERROR;
    }
    ship->tier = 2;
    switch (ship->type)
    {
    case INTERCEPTOR:
        ship->attack = 6;
        ship->health = 11;
        ship->max_health = 11;
        ship->special = 6;
        break;
    case BOMBER:
        ship->attack = 3;
        ship->health = 13;
        ship->max_health = 13;
        ship->special = 5;
        break;
    case SUPPORT:
        ship->attack = 1;
        ship->health = 16;
        ship->max_health = 16;
        ship->special = 7;
        break;
    case DESTROYER:
        ship->attack = 4;
        ship->health = 19;
        ship->max_health = 19;
        ship->special = 2;
        break;
    case BOSS:
        ship->attack = 8;
        ship->health = 35;
        ship->max_health = 35;
        break;
    }
    if (EXTRA_GUNS & ship->mods) {
        ship->attack += 2;
    }
    if (EXTRA_SHIELD & ship->mods) {
        ship->max_health += 3;
        ship->health += 3;
    }
    if (EXTRA_COMP & ship->mods) {
        ship->special += 1;
    }
    return OK;
}


void take_damage(ship *ship, uch amount, uch multiplier, ship_type source_type) {
    uch total_damage;
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
        total_damage = monus(total_damage, ship->special);
    }
    if (ARMOR & ship->mods) {
        total_damage = monus(total_damage, 1);
    }
    ship->health = monus(ship->health, total_damage);
    if (ship->health == 0) {
        ship->is_alive = False;
    }
}

void heal(ship *ship, uch amount) {
    uch result_health;
    result_health = ship->health + amount;
    if (result_health > ship->max_health) {
        ship->health = ship->max_health;
    }
    else {
        ship->health = result_health;
    }
}

// uch upgrade_ship(ship *ship) {
//     // uch old_mods;
//     // old_mods = ship->mods;
//     if (ship->tier > 1) {
//         return ERROR;
//     } else {
//         return OK;
//     }
// }

#endif
