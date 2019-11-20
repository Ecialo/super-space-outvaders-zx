#ifndef __SUP_SP_INSPECT__
#define __SUP_SP_INSPECT__

#include "ship.c"
#include "wing.c"
#include "base_sp1.c"

struct sp1_pss ps0;

void init_inspector() {
    ps0.bounds = &full_screen;
    ps0.flags = SP1_PSSFLAG_INVALIDATE;
    ps0.visit = 0;
}

void inspect_ship(ship *ship, char x, char y) {
    char num_holder[3];
    num_holder[2] = '\0';
    sp1_SetPrintPos(&ps0, y, x);

    // INIT
    sp1_PrintString(&ps0, "\x14\x50");

    // NAME
    sp1_PrintString(&ps0, ship->name);

    // TYPE

    // ATTACK
    sp1_SetPrintPos(&ps0, y + 1, x);
    to_string(ship->attack, num_holder);
    sp1_PrintString(&ps0, "ATK:");
        sp1_PrintString(&ps0, num_holder);

    // SPECIAL
    to_string(ship->special, num_holder);
    sp1_SetPrintPos(&ps0, y + 2, x);
    sp1_PrintString(&ps0, "SPC:"); sp1_PrintString(&ps0, num_holder);
    
    // HP
    to_string(ship->health, num_holder);
    sp1_SetPrintPos(&ps0, y + 3, x);
    sp1_PrintString(&ps0, "SHD:"); 
        sp1_PrintString(&ps0, num_holder);
        to_string(ship->max_health, num_holder);
        sp1_PrintString(&ps0, "/");
        sp1_PrintString(&ps0, num_holder);

}
#endif