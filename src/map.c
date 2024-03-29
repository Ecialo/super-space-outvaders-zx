#ifndef __SUP_SP_MAP__
#define __SUP_SP_MAP__


#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <z80.h>               // for z80_bpoke(), z80_wpoke(), im2 functions
#include <string.h>            // for memset()
#include <stdlib.h>            // for rand()
#include <intrinsic.h>         // for intrinsic_di()
#include <sound.h>             // for bit_beepfx()

#include "world.c"
#include "base_sp1.c"
#include "tiles.c"
#include "cursor.c"

uch compute_node_x(uch node_i) {
    // return (nodes_x[node_i] - nodes_x[current_world]) * 3;
    return (nodes_x[node_i] - world_x_offset) * 3;
    // return (nodes_x[node_i] - 0) * 3;
}

uch compute_node_y(uch node_i) {
    return nodes_y[node_i] * 3;
}

void print_arr(uint16_t row, uint16_t col, uint16_t colour, uch in_arr, uch out_arr) {
    uch out = NO_OUT * (1 - out_arr);

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

void clear_screen_from_map() {
    sp1_ClearRectInv(&map_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR | SP1_RFLAG_SPRITE);
    // sp1_PutTilesInv(&map_rect, env_tiles);
}

void draw_map() {
    uch i;
    uch x, y, power;
    uch color;
    sp1_ClearRectInv(&map_rect, INK_BLACK | PAPER_RED, ' ', SP1_IFLAG_OVERWRITE_TILES);

    // sp1_GetTiles(&map_rect, env_tiles);
    for (i = 0; i < world_size; i++) {
        if (i == current_world) {
            color = INK_MAGENTA | PAPER_RED;
        } else {
            color = INK_BLACK | PAPER_RED;
        }
        if (nodes_x[i] < world_x_offset || (nodes_x[i] - world_x_offset) > 6) {
            continue;
        }
        x = compute_node_x(i);
        y = compute_node_y(i);
        switch (nodes_content[i]) {
            case ENEMY:
                power = node_args[i]; 
                if (power < 3) {
                    print_big_at_inv(y, x, color, EASY_NODE_TILES);
                } else if (power < 6) {
                    print_big_at_inv(y, x, color, NORM_NODE_TILES);
                } else if (power < 9) {
                    print_big_at_inv(y, x, color, HARD_NODE_TILES);
                } else {
                    print_big_at_inv(y, x, color, BOSS_NODE_TILES);
                }
                break;
            case SKIP_NODE:
                print_big_at_inv(y, x, color, SKIP_NODE_TILES);
                break;
            case SHOP:
                print_big_at_inv(y, x, color, CREDIT_TILES);
                break;
            default:
                print_big_empty(y, x, color);
                break;
        }
        if (nodes_y[i] == 1) {
            sp1_PrintAtInv(y - 1, x, color, ' ');
            sp1_PrintAtInv(y + 2, x, color, ' ');
            sp1_PrintAtInv(y - 1, x + 1, color, ' ');
            sp1_PrintAtInv(y + 2, x + 1, color, ' ');
        }
        if (nodes_x[i] - world_x_offset) {
            print_arr(y, x - 1, INK_BLACK | PAPER_RED, nodes_in[i], nodes_out[i]);
            if (nodes_y[i] == 1) {
                sp1_PrintAtInv(y - 1, x - 1, color, NO_BOT + NO_OUT);
                sp1_PrintAtInv(y + 2, x - 1, color, NO_BOT + NO_OUT);
            }
        }
    }

}


void select_destination() {
    uch s, nwi;
    uch x, y;
    world_node *cwn;
    
    CURSOR_POS = 0;
    cwn = &world[current_world];


    s = cwn->num_of_next_worlds - 1;
    nwi = cwn->next_worlds[CURSOR_POS];
    
    x = compute_node_x(nwi);
    y = compute_node_y(nwi);
    draw_ramka_at(y, x, 2, 2);
    sp1_UpdateNow();
    while(1) {
        in_wait_nokey();
        in_wait_key();
        if (in_key_pressed(IN_KEY_SCANCODE_a) && CURSOR_POS > 0) {
            CURSOR_POS--;
        } else if (in_key_pressed(IN_KEY_SCANCODE_d) && CURSOR_POS < s) {
            CURSOR_POS++;
        } else if (in_key_pressed(IN_KEY_SCANCODE_1)) {
            CURSOR_POS = CURSOR_CANCEL;
            break;
        } else if (in_key_pressed(IN_KEY_SCANCODE_SPACE)) {
            break;
        }
        nwi = cwn->next_worlds[CURSOR_POS];
        x = compute_node_x(nwi);
        y = compute_node_y(nwi);

        draw_ramka_at(y, x, 2, 2);
        sp1_UpdateNow();
    }
}
#endif