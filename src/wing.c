#ifndef __SUP_SP_WING__
#define __SUP_SP_WING__

#define True 1
#define False 0

#define OK 0
#define ERROR 1

#define MAX_WING_SIZE 5
#define NO_SLOT 5

#include <assert.h>
#include "ship.c"

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
    init_wing(wing);
    add_ship(wing, "KEK", INTERCEPTOR);
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
#endif
