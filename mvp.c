#include <string.h>
#include "src/base_sp1.c"
#include "src/draw_wing.c"
#include "src/bonus.c"
#include "src/cursor.c"
#include "src/inspect.c"
#include "src/utils.c"
#include "src/ship.c"
#include "src/wing.c"
#include "src/map.c"
#include "src/world.c"
#include "src/init_all.c"

// #define NO_MODS 0
// #define TORPEDO 1
// #define REMTECH 2

#define RETREAT 0
#define ATTACK 1
#define SPECIAL 2

#define NUM_OF_ACTIONS 3
#define NO_CHOICE 7

typedef enum State {
    IDLE,
    FLY,
    FIGHT,
    VICTORY,
    DEFEAT,
    EXIT,
} state;

// typedef enum Action {
//     RETREAT,
//     ATTACK,
//     SPECIAL,
// } action;

typedef enum FightResults {
    CONTINUE,
    THEY,
    WE,
} fight_result;

extern uch bullet1[];

state state;
wing BOTH_WINGS[2];
#define PLAYER_WING BOTH_WINGS[0]
#define ENEMY_WING BOTH_WINGS[1]
uch COMBAT_ROUND;


uch ENEMY_CHOICE[3];   // action, most damaged enemy, most damaged player

void init_state() {
    state = IDLE;
    // state->current_world = 0;
    init_wing(&PLAYER_WING);
    // fill_wing_with_rand_ships(&state->player_wing, 3, 0, 0);
    fill_wing_with_rand_ships(&PLAYER_WING, 5, 0, 0);
    COMBAT_ROUND = 0;
    MONEY = 15;
    // MONEY = 0;
}

void shoot_bullet_r(int x1, int y1, int x2, int y2)
{
    uch i;
    int dx, dy;
    dx = (x2 - x1) / 10;
    dy = (y2 - y1) / 10;
    for (i = 0; i < 9; ++i){
        sp1_MoveSprPix(
            cursor_sprites[0], 
            &full_screen, 
            bullet1, 
            x1 + i * dx, 
            y1 + i * dy
        );
        sp1_UpdateNow();
    }
    vsync(3000);
    sp1_MoveSprPix(
        cursor_sprites[0], 
        &full_screen, 
        bullet1, 
        x2, 
        y2
    );
    sp1_UpdateNow();
    vsync(3000);
}

void shoot_bullet(uch f, uch t, uch side) {
    if (side == OUR_SIDE) {
        shoot_bullet_r(
            our_wing_pos_x[f] + SHIP_SIZE / 2, our_wing_pos_y[f],  
            their_wing_pos_x[t] + SHIP_SIZE / 2, their_wing_pos_y[t] + SHIP_SIZE
        );
    } else {
        shoot_bullet_r(
            their_wing_pos_x[f] + SHIP_SIZE / 2, their_wing_pos_y[f] + SHIP_SIZE,
            our_wing_pos_x[t] + SHIP_SIZE / 2, our_wing_pos_y[t]
        );
    }
    // sp1_PrintAtInv(l_t_y, l_t_x, BACK_COLOR, ' ');
}

void flash(ship *ship, uch i, uch side) {
    uch x, y, c;
    if (side == OUR_SIDE) {
        x = our_wing_pos_x[i] / 8;
        y = our_wing_pos_y[i] / 8;
    } else {
        x = their_wing_pos_x[i] / 8;
        y = their_wing_pos_y[i] / 8;
    }
    for (c = 0; c < 11; c++) {
        print_ship_at_inv(y, x, ship, c % 2, side == THEIR_SIDE);
        vsync(1500);
    }
}



// // READ
uch read_action() {
    select_from_battle_options();
    return CURSOR_POS;
}

uch read_ship_i(wing *wing, uch side) {
    select_from_wing(wing, side);
    return CURSOR_POS;
}


void gen_ai() {
    ship *leader = get_leader(&ENEMY_WING);
    if (leader->type == BOSS && leader->tier == 1) {
        ENEMY_CHOICE[0] = RETREAT;
        ENEMY_CHOICE[1] = NO_CHOICE;
        ENEMY_CHOICE[2] = NO_CHOICE;
    } else if (leader->attack > get_leader(&PLAYER_WING)->health) {
        ENEMY_CHOICE[0] = ATTACK;
        ENEMY_CHOICE[1] = NO_CHOICE;
        ENEMY_CHOICE[2] = NO_CHOICE;
    } else if (
        leader->health < leader->max_health / 2
        && ENEMY_WING.size > 0 
        && get_ship(&ENEMY_WING, 1)->health > (get_ship(&ENEMY_WING, 1)->max_health / 2)
    ) {
        ENEMY_CHOICE[0] = RETREAT;
        ENEMY_CHOICE[1] = NO_CHOICE;
        ENEMY_CHOICE[2] = NO_CHOICE;
    } else {
        if (leader->type == DESTROYER && ENEMY_WING.size <= 1) {
            ENEMY_CHOICE[0] = ATTACK;
        } else {
            ENEMY_CHOICE[0] = ((uch) rand() % 2) + 1;
        }
        ENEMY_CHOICE[1] = get_most_damaged_ship_i(&ENEMY_WING);
        ENEMY_CHOICE[2] = get_most_damaged_ship_i(&PLAYER_WING);
    }
}

// ACTIONS

fight_result check_fight_result() {
    if (PLAYER_WING.size == 0) {
        return THEY;
    } else if (ENEMY_WING.size == 0) {
        return WE;
    } else {
        return CONTINUE;
    }
}

void perform_action(uch action, uch choice, uch damage_multiplier, uch side) {
    wing *you;
    wing *oppose;
    ship *you_leader;
    ship *oppose_leader;

    if (side == OUR_SIDE) {
        you = &PLAYER_WING;
        oppose = &ENEMY_WING;
    } else {
        oppose = &PLAYER_WING;
        you = &ENEMY_WING;
    }
    you_leader = get_leader(you);
    oppose_leader = get_leader(oppose);
    
    switch (action) {
        case ATTACK:
            if (oppose->protector == NO_SLOT) {
                shoot_bullet(0, 0, side);
                take_damage(
                    oppose_leader, 
                    you_leader->attack, 
                    damage_multiplier,
                    you_leader->type
                );
                flash(oppose_leader, 0, 1 - side);
            } else {
                shoot_bullet(0, oppose->protector, side);
                take_damage(
                    get_ship(oppose, oppose->protector),
                    you_leader->attack, 
                    damage_multiplier,
                    you_leader->type
                );
                flash(get_ship(oppose, oppose->protector), oppose->protector, 1 - side);
                oppose->protector = NO_SLOT;
            }
            break;
        case RETREAT:
            cycle_ships(you);
            // leader_retreat(OUR_SIDE, you->size - 1);
            if (get_leader(you)->type == INTERCEPTOR) {
                take_damage(
                    oppose_leader, 
                    get_leader(you)->special, 
                    damage_multiplier,
                    INTERCEPTOR
                );
                flash(oppose_leader, 0, 1 - side);
            }
            break;
        case SPECIAL:
            switch (you_leader->type) {
                case INTERCEPTOR:
                    // swap_ships_with_anim(oppose, 0, choice, !side);
                    swap_ships(oppose, 0, choice);
                    break;
                case BOMBER:
                    take_damage(
                        get_ship(oppose, choice), 
                        you_leader->special, 
                        damage_multiplier,
                        BOMBER
                    );
                    shoot_bullet(0, choice, side);
                    flash(get_ship(oppose, choice), choice, 1 - side);
                    break;
                case DESTROYER:
                    if (you->size > 1) {
                        // swap_ships_with_anim(you, 0, 1, side);
                        swap_ships(you, 0, 1);
                    }
                    you->protector = you->arrange[1];
                    if (you_leader->type == INTERCEPTOR) {
                        take_damage(
                            oppose_leader, 
                            you_leader->special, 
                            damage_multiplier,
                            INTERCEPTOR
                        );
                        flash(oppose_leader, 0, 1 - side);
                    }
                    break;  
                case SUPPORT:
                    flash(you_leader, 0, side);
                    heal(
                        get_ship(you, choice),
                        you_leader->special
                    );
                    flash(get_ship(you, choice), choice, side);
                    break;
                case BOSS:
                    take_damage(
                        get_ship(oppose, choice),
                        get_ship(oppose, choice)->attack,
                        damage_multiplier,
                        get_ship(oppose, choice)->type
                    );
                    flash(get_ship(oppose, choice), choice, 1 - side);
                    break;
                default:
                    // assert(0);
                    break;
            }
            break;
        default:
            // assert(0);
            break;
    }
}

void perform_summoner_boss_action(uch damage_mulipler) {
    if (ENEMY_WING.size <= MAX_WING_SIZE - 2) {
        add_ship(&ENEMY_WING, INTERCEPTOR);
        fill_wing_with_rand_ships(&ENEMY_WING, 1, 0, 0);
    } else if (ENEMY_WING.size == MAX_WING_SIZE - 1) {
        add_ship(&ENEMY_WING, INTERCEPTOR);
        fill_wing_with_rand_ships(&ENEMY_WING, 0, 0, 1);
    } else {
        fill_wing_with_rand_ships(&ENEMY_WING, 0, 0, 2);
    }
    ENEMY_CHOICE[0] = RETREAT;
    ENEMY_CHOICE[1] = NO_CHOICE;
    ENEMY_CHOICE[2] = NO_CHOICE;
}

void perform_round(
    // wing *player, 
    // wing *enemy, 
    uch damage_multiplier,
    uch player_action,
    uch player_choice,
    uch enemy_action,
    uch enemy_choice
) {
    uch i, side, wing_size;
    ship *tmp;
    // int result_effect;
    // ship *player_leader, *enemy_leader;

    // wing_size = PLAYER_WING.size;
    // for (i = 1; i < wing_size; i++) {
    //     tmp = get_ship(&PLAYER_WING, i);
    //     heal(
    //         get_ship(tmp, i), 
    //         PLAYER_WING.heal
    //     );
    // }
    // wing_size = ENEMY_WING.size;
    // for (i = 1; i < wing_size; i++) {
    //     tmp = get_ship(&ENEMY_WING, i);
    //     heal(
    //         tmp, 
    //         ENEMY_WING.heal
    //     );
    // }

    for (side = OUR_SIDE; side < 2; side++) {
        wing_size = BOTH_WINGS[side].size;
        for (i = 1; i < wing_size; i++) {
            heal(
                get_ship(&BOTH_WINGS[side], i), 
                BOTH_WINGS[side].heal
            );
        }
    }

    for (side = OUR_SIDE; side < 2; side++) {
        wing_size = BOTH_WINGS[side].size;
        for (i = 0; i < wing_size; i++) {
            tmp = get_ship(&BOTH_WINGS[side], i);
            if (tmp->mods & TORPEDO || tmp->type == BOMBER) {
                shoot_bullet(i, 0, side);
                render_scene();
            }
            if (tmp->mods & REMTECH || tmp->type == SUPPORT) {
                flash(tmp, i, side);
                render_scene();
            }
        }
    }

    // result_effect = PLAYER_WING.heal - (ENEMY_WING.missile);
    for (side = OUR_SIDE; side < 2; side++) {
        if(BOTH_WINGS[side].heal > BOTH_WINGS[1 - side].missile) {
            heal(
                get_leader(&BOTH_WINGS[side]), 
                BOTH_WINGS[side].heal - BOTH_WINGS[1 - side].missile
            ); 
        } else if (BOTH_WINGS[side].heal < BOTH_WINGS[1 - side].missile) {
            take_damage(
                get_leader(&BOTH_WINGS[side]), 
                BOTH_WINGS[1 - side].missile - BOTH_WINGS[side].heal,
                damage_multiplier,
                BOMBER
            );
        }
    }
    // if (PLAYER_WING.heal > ENEMY_WING.missile) {
    //     heal(
    //         get_leader(&PLAYER_WING), 
    //         PLAYER_WING.heal - ENEMY_WING.missile
    //     );
    // } else if (PLAYER_WING.heal < ENEMY_WING.missile) {
    //     take_damage(
    //         get_leader(&PLAYER_WING), 
    //         ENEMY_WING.missile - PLAYER_WING.heal,
    //         damage_multiplier,
    //         BOMBER
    //     );
    // }

    // // result_effect = ENEMY_WING.heal - (PLAYER_WING.missile);
    // if (ENEMY_WING.heal > (PLAYER_WING.missile)) {
    //     heal(
    //         get_leader(&ENEMY_WING), 
    //         ENEMY_WING.heal - PLAYER_WING.missile
    //     );
    // } else if (ENEMY_WING.heal < (PLAYER_WING.missile)) {
    //     take_damage(
    //         get_leader(&ENEMY_WING), 
    //         PLAYER_WING.missile - ENEMY_WING.heal,
    //         damage_multiplier,
    //         BOMBER
    //     );
    // }
    // player_leader = get_leader(&PLAYER_WING);
    // enemy_leader = get_leader(&ENEMY_WING);
    render_scene();
    perform_action(player_action, player_choice, damage_multiplier, OUR_SIDE);
    if (get_leader(&ENEMY_WING)->type == BOSS && get_leader(&ENEMY_WING)->tier == 1) {
        perform_summoner_boss_action(damage_multiplier);
    }
    perform_action(enemy_action, enemy_choice, damage_multiplier, THEIR_SIDE);
    render_scene();
    // TODO COMPLETE PERFORMER
}

void render_ours() {
    render_wing(&PLAYER_WING, OUR_SIDE);
    inspect_wing(&PLAYER_WING, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_money(MONEY);
    sp1_UpdateNow();
}

void render_scene() {
    render_wing(&ENEMY_WING, THEIR_SIDE);
    inspect_wing(&ENEMY_WING, &target_inspect_wing_rect, &target_inspect_ship_rect);
    render_ours();
}

void prepare() {
    uch ship_a, ship_b;

    for (;;) {
        render_scene();
        select_from_prepare_options();
        if (CURSOR_POS == SWAP_OPTION) {
            ship_a = read_ship_i(&PLAYER_WING, OUR_SIDE);
            ship_b = read_ship_i(&PLAYER_WING, OUR_SIDE);
            // swap_ships_with_anim(&player_wing, ship_a, ship_b, OUR_SIDE);
            swap_ships(&PLAYER_WING, ship_a, ship_b);
        } else if (CURSOR_POS == SPY_OPTION) {
            read_ship_i(&ENEMY_WING, THEIR_SIDE);
        } else if (CURSOR_POS == CURSOR_CANCEL) {
            state = EXIT;
            break;
        } else {
            break;
        }
    }
}

void steal_player_strongest() {
    uch strongest_i;
    ship *strongest_ship, *new_enemy;
    uch mods, i;
    
    strongest_i = get_strongest_ship(&PLAYER_WING);
    strongest_ship = get_ship(&PLAYER_WING, strongest_i);
    mods = strongest_ship->mods;

    add_ship(&ENEMY_WING, INTERCEPTOR);
    swap_ships(&ENEMY_WING, 0, ENEMY_WING.size - 1);
    new_enemy = get_leader(&ENEMY_WING);
    new_enemy->name = strongest_ship->name;
    new_enemy->attack = strongest_ship->attack;
    new_enemy->health = strongest_ship->health;
    new_enemy->max_health = strongest_ship->max_health;
    new_enemy->special = strongest_ship->special;
    new_enemy->type = strongest_ship->type;
    new_enemy->tier = strongest_ship->tier;
    for (i = 0; i < 7; i++) {
        if (!(ALL_MODS[i] & mods)) {
            install_mod(&ENEMY_WING, new_enemy, ALL_MODS[i]);
        }
    }
    remove_ship(&PLAYER_WING, strongest_i);

}

void perform_fight() {
    uch player_action, enemy_action; 
    uch player_choice, enemy_choice;
    fight_result fight_result;
    
    init_enemy_wing(
        &ENEMY_WING, 
        node_args[current_world]
    );
    COMBAT_ROUND = 0;

    render_scene();
    prepare();
    if (state == EXIT) {
        return;
    }

    if (get_leader(&ENEMY_WING)->type == BOSS && get_leader(&ENEMY_WING)->tier == 2) {
        steal_player_strongest();
    }

    for (;;) {

        render_scene();

        player_action = read_action();
        if (state == EXIT) {
            return;
        }
        if (player_action == SPECIAL) {
            switch (get_leader(&PLAYER_WING)->type) {
                case BOMBER:
                case INTERCEPTOR:
                    player_choice = read_ship_i(&ENEMY_WING, THEIR_SIDE);
                    break;
                case SUPPORT:
                    player_choice = read_ship_i(&PLAYER_WING, OUR_SIDE);
                    break;
                default:
                    player_choice = NO_CHOICE;
                    break;
            }
        } else {
            player_choice = NO_CHOICE;
        }

        gen_ai();   // expand this where boss added
        enemy_action = ENEMY_CHOICE[0];
        if (enemy_action == SPECIAL) {
            switch (get_leader(&PLAYER_WING)->type) {
                case BOMBER:
                case INTERCEPTOR:
                    enemy_choice = ENEMY_CHOICE[2];
                    break;
                case SUPPORT:
                    enemy_choice = ENEMY_CHOICE[1];
                    break;
                default:
                    enemy_choice = NO_CHOICE;
                    break;
            }
        } else {
            enemy_choice = NO_CHOICE;
        }

        perform_round(
            (COMBAT_ROUND / 10) + 1,
            player_action,
            player_choice,
            enemy_action,
            enemy_choice
        );
        scrap_dead_ships(&PLAYER_WING);
        scrap_dead_ships(&ENEMY_WING);
        fight_result = check_fight_result();
        render_scene();
        sp1_UpdateNow();
        
        if (fight_result == CONTINUE) {
            COMBAT_ROUND++;
        } else if (fight_result == WE) {
            state = IDLE;
            PLAYER_WING.protector = NO_SLOT;
            MONEY = MONEY + node_args[current_world] + 1;
            inspect_money(MONEY);
            get_bonuses(collect_scrap() / 20, &PLAYER_WING);
            break;
        } else if (fight_result == THEY) {
            state = DEFEAT;
            break;
        }
    }
    clear_screen_from_wing(THEIR_SIDE);
    render_ours();
}

uch read_next_node() {
    select_destination();
    return world[current_world].next_worlds[CURSOR_POS];
}

void perform_flight() {
    draw_map();
    sp1_UpdateNow();
    current_world = read_next_node();
    if (CURSOR_POS != CURSOR_CANCEL) {
        world_x_offset = nodes_x[current_world];
        clear_screen_from_map();
        draw_stars();
        // into_the_hyperspace(player_wing.size);
        sp1_UpdateNow();
    } else {
        state = EXIT;
    }
}

bonus shop[4];
void perform_shopping() {
    uch i;
    shop[0] = HEAL;
    shop[1] = ATK;
    render_ours();
    // for (i = 0; i < 3; i++) {
    //     shop[i + 1] = (bonus) (rand() % (NUM_OF_MOD_TYPES - 1));
    // }
    // collect_bonuses(shop, 4, &state->player_wing, True);
    collect_bonuses(shop, 2, &PLAYER_WING, True);
    sp1_ClearRectInv(&costs_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    CURSOR_POS = 0;
    render_ours();
}

extern uch launch_text[];
void launch_screen() {
    ps0.attr = INK_GREEN | PAPER_BLACK;
    // sp1_PrintString(&ps0, "\x14\x50");
    sp1_SetPrintPos(&ps0, 0, 0);
    sp1_PrintString(&ps0, launch_text);
    sp1_UpdateNow();
    select_seed();
    sp1_ClearRectInv(&full_screen, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);

}

// MAIN
void main() {
    init_all();

    launch_screen();

    init_state();
    select_map(rand() % MAPS_COUNT);
    generate_world();
    render_ours();

    // for(;;) {
    //     perform_fight();
    //     if (PLAYER_WING.size == 0) {
    //         break;
    //     }
    // }
    while (world[current_world].num_of_next_worlds > 0 && state != EXIT) {
        
        perform_flight();
        heal_wing(&PLAYER_WING, 4);

        if (state == EXIT) {
            break;
        } else if (nodes_content[current_world] == ENEMY) {
            perform_fight();
            if (state == DEFEAT || state == EXIT) {
                break;
            }
        } else if (nodes_content[current_world] == SHOP) {
            perform_shopping();
        }   
    }
    sp1_ClearRectInv(&full_screen, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR | SP1_RFLAG_SPRITE);
    sp1_SetPrintPos(&ps0, 10, 10);
    if (state == DEFEAT) {
        sp1_PrintString(&ps0, "GAME OVER :o(");
    } else if (state == VICTORY) {
        sp1_PrintString(&ps0, "YOU WIN :o)");
    } else {
        sp1_PrintString(&ps0, "GAME OVER");
    }
    sp1_UpdateNow();

}

