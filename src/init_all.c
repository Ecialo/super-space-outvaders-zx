#include "base_sp1.c"
#include "bonus.c"
#include "cursor.c"
#include "draw_wing.c"
#include "inspect.c"
#include "tiles.c"

void init_all() {
    init_sp1();
    init_bonus_wing();
    // init_bullet();
    // init_ship_sprites();
    // init_shield_sprites();
    init_cursor();
    init_inspector();
    init_all_tilesets();
}