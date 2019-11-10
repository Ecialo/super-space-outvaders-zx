#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "src/world.c"
// #include "src/icons.h"
#include "src/base_sp1.c"
#include "src/map_tiles.h"
// #include "src/draw_wing.c"
#include "src/cursor.c"

// extern unsigned char credit[];

void init_tileset(uint8_t *tileset, uint16_t offset, char size) {
    char i;
    for (i = 0; i < size; i++) {
        sp1_TileEntry(offset + i, tileset + i*8);
    }
}

void init_map_tiles() {
    init_tileset(credit, CREDIT_TILES, 4);
    init_tileset(compact_arr, COMPACT_ARR_TILES, 16);
    init_tileset(skip_node, SKIP_NODE_TILES, 4);
}

void print_big_at_inv(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile) {
    sp1_PrintAtInv(row, col, colour, tile + LEFT_TOP_TILE);
    sp1_PrintAtInv(row, col + 1, colour, tile + RIGHT_TOP_TILE);
    sp1_PrintAtInv(row + 1, col, colour, tile + LEFT_BOT_TILE);
    sp1_PrintAtInv(row + 1, col + 1, colour, tile + RIGHT_BOT_TILE);
}

void print_big_empty(uint16_t row, uint16_t col, uint16_t colour) {
    sp1_PrintAtInv(row, col, colour, ' ');
    sp1_PrintAtInv(row, col + 1, colour, ' ');
    sp1_PrintAtInv(row + 1, col, colour, ' ');
    sp1_PrintAtInv(row + 1, col + 1, colour, ' ');
}

void print_arr(uint16_t row, uint16_t col, uint16_t colour, char in_arr, char out_arr) {
    char out = NO_OUT * !out_arr;

    if (IN_UP & in_arr && IN_MID & in_arr) {
        sp1_PrintAtInv(row, col, colour, ALL_CON_TOP + out);
    } else if (IN_UP & in_arr) {
        sp1_PrintAtInv(row, col, colour, NO_MID_TOP + out);
    } else if (IN_MID & in_arr) {
        sp1_PrintAtInv(row, col, colour, ONLY_MID_TOP + out);
    } else {
        sp1_PrintAtInv(row, col, colour, NO_TOP + out);
    }

    if (IN_DOWN & in_arr && IN_MID & in_arr) {
        sp1_PrintAtInv(row + 1, col, colour, ALL_CON_BOT + out);
    } else if (IN_DOWN & in_arr) {
        sp1_PrintAtInv(row + 1, col, colour, NO_MID_BOT + out);
    } else if (IN_MID & in_arr) {
        sp1_PrintAtInv(row + 1, col, colour, ONLY_MID_BOT + out);
    } else {
        sp1_PrintAtInv(row + 1, col, colour, NO_BOT + out);
    }
}


void draw_map() {
    char i;
    char x, y;
    char color;
    for (i = 0; i < WORLD_SIZE; i++) {
        if (i == current_world) {
            color = INK_MAGENTA | PAPER_RED;
        } else {
            color = INK_BLACK | PAPER_RED;
        }
        x = nodes_x[i] * 3 + 3;
        y = nodes_y[i] * 2 + 3;
        switch (nodes_content[i]) {
            case JUST:
                print_big_at_inv(y, x, color, CREDIT_TILES);
                break;
            case SKIP_NODE:
                print_big_at_inv(y, x, color, SKIP_NODE_TILES);
                break;
            default:
                print_big_empty(y, x, color);
                break;
        }
        if (nodes_x[i]) {
            print_arr(y, x - 1, INK_BLACK | PAPER_RED, nodes_in[i], nodes_out[i]);
        }
    }

}

void select_destination() {
    char s, nwi;
    char x, y;
    world_node cwn;
    
    CURSOR_POS = 0;
    cwn = world[current_world];
    
    s = cwn.num_of_next_worlds - 1;
    nwi = cwn.next_worlds[CURSOR_POS];
    x = nodes_x[nwi] * 3 + 3;
    y = nodes_y[nwi] * 2 + 3;
    draw_ramka_at(y, x, 2, 2);
    sp1_UpdateNow();
    while(1) {
        in_wait_nokey();
        in_wait_key();
        if (in_key_pressed(IN_KEY_SCANCODE_a) && CURSOR_POS > 0) {
            CURSOR_POS--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && CURSOR_POS < s) {
            CURSOR_POS++;
        } else if (in_key_pressed(IN_KEY_SCANCODE_SPACE)) {
            break;
        }
        nwi = cwn.next_worlds[CURSOR_POS];
        x = nodes_x[nwi] * 3 + 3;
        y = nodes_y[nwi] * 2 + 3;
        draw_ramka_at(y, x, 2, 2);
        // // render_wing(wing, side);
        // // draw_ramka_at(wing_sprites[CURSOR_POS + offset]);
        sp1_UpdateNow();
    }
}


int main() {
    // char i;
    init_sp1();
    init_map_tiles();
    init_cursor();
    init_icons();
    generate_world();
    draw_map();
    // sp1_UpdateNow();
    select_destination();

    // sp1_UpdateNow();

    // while(1);
    return 0;
}
