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

// #define FIGHT_CONTINUE 0;
// #define FIGHT_WE 2;
// #define FIGHT_THEY 1;

// GLOBAL GAME STATE
// typedef struct GameState
// {
//     uch state;
//     wing player_wing;
//     uch combat_round;
//     uch money;
// } game_state;

state state;
wing PLAYER_WING;
wing ENEMY_WING;
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

// // READ
uch read_action() {
    select_from_battle_options();
    return CURSOR_POS;
}

uch read_ship_i(wing *wing, uch side) {
    select_from_wing(wing, side);
    return CURSOR_POS;
}

// void swap_ships_with_anim(wing *wing, uch a, uch b, uch side) {
    // swap_ships_sprites(side, a, b);
    // swap_ships(wing, a, b);
// }

void gen_ai() {
    ship *leader = get_leader(&ENEMY_WING);
    if (leader->attack > get_leader(&PLAYER_WING)->health) {
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
            // shoot_bullet(0, 0, side);
            if (oppose->protector == NO_SLOT) {
                take_damage(
                    oppose_leader, 
                    you_leader->attack, 
                    damage_multiplier,
                    you_leader->type
                );
            } else {
                take_damage(
                    get_ship(oppose, oppose->protector),
                    you_leader->attack, 
                    damage_multiplier,
                    you_leader->type
                );
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
                    // shoot_bullet(0, choice, side);
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
                    }
                    break;  
                case SUPPORT:
                    heal(
                        get_ship(you, choice),
                        you_leader->special
                    );
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
        fill_wing_with_rand_ships(&ENEMY_WING, 2, 0, 0);
    } else if (ENEMY_WING.size == MAX_WING_SIZE - 1) {
        fill_wing_with_rand_ships(&ENEMY_WING, 1, 0, 1);
    } else {
        fill_wing_with_rand_ships(&ENEMY_WING, 0, 0, 2);
    }
    cycle_ships(&ENEMY_WING);
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
    uch i, wing_size;
    // int result_effect;
    // ship *player_leader, *enemy_leader;

    wing_size = PLAYER_WING.size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(&PLAYER_WING, i), 
            PLAYER_WING.heal
        );
    }
    wing_size = ENEMY_WING.size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(&ENEMY_WING, i), 
            ENEMY_WING.heal
        );
    }

    // result_effect = PLAYER_WING.heal - (ENEMY_WING.missile);
    if (PLAYER_WING.heal > ENEMY_WING.missile) {
        heal(
            get_leader(&PLAYER_WING), 
            PLAYER_WING.heal - ENEMY_WING.missile
        );
    } else if (PLAYER_WING.heal < ENEMY_WING.missile) {
        take_damage(
            get_leader(&PLAYER_WING), 
            ENEMY_WING.missile - PLAYER_WING.heal,
            damage_multiplier,
            BOMBER
        );
    }

    // result_effect = ENEMY_WING.heal - (PLAYER_WING.missile);
    if (ENEMY_WING.heal > (PLAYER_WING.missile)) {
        heal(
            get_leader(&ENEMY_WING), 
            ENEMY_WING.heal - PLAYER_WING.missile
        );
    } else if (ENEMY_WING.heal < (PLAYER_WING.missile)) {
        take_damage(
            get_leader(&ENEMY_WING), 
            PLAYER_WING.missile - ENEMY_WING.heal,
            damage_multiplier,
            BOMBER
        );
    }
    // player_leader = get_leader(&PLAYER_WING);
    // enemy_leader = get_leader(&ENEMY_WING);

    perform_action(player_action, player_choice, damage_multiplier, OUR_SIDE);
    perform_action(enemy_action, enemy_choice, damage_multiplier, THEIR_SIDE);
    // TODO COMPLETE PERFORMER
}

void render_ours() {
    render_wing(&PLAYER_WING, OUR_SIDE);
    inspect_wing(&PLAYER_WING, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_money(MONEY);
    update_screen();
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
        } else {
            break;
        }
    }
}

void perform_fight() {
    uch player_action, enemy_action; 
    uch player_choice, enemy_choice;
    fight_result fight_result;
    // wing player_wing;
    // wing enemy_wing;

    // player_wing = &state->player_wing;
    
    init_enemy_wing(
        &ENEMY_WING, 
        // node_args[current_world]
        1
    );
    COMBAT_ROUND = 0;

    render_scene();
    prepare();

    for (;;) {

        render_scene();

        player_action = read_action();
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
        update_screen();
        
        if (fight_result == CONTINUE) {
            COMBAT_ROUND++;
        } else if (fight_result == WE) {
            state = IDLE;
            PLAYER_WING.protector = NO_SLOT;
            MONEY = MONEY + node_args[current_world] + 1;
            inspect_money(MONEY);
            collect_scrap();
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
    clear_screen_from_map();
    // into_the_hyperspace(player_wing.size);
    sp1_UpdateNow();
}

void perform_shopping() {
    bonus shop[4];
    uch i;
    shop[0] = HEAL;
    shop[1] = ATK;
    render_wing(&PLAYER_WING, OUR_SIDE);
    // for (i = 0; i < 3; i++) {
    //     shop[i + 1] = (bonus) (rand() % (NUM_OF_MOD_TYPES - 1));
    // }
    // collect_bonuses(shop, 4, &state->player_wing, True);
    collect_bonuses(shop, 2, &PLAYER_WING, True);
    sp1_ClearRectInv(&costs_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
}

// MAIN
void main() {
    init_state();
    select_map(0);
    generate_world();

    init_all();
    render_ours();

    // for(;;) {
    //     perform_fight();
    //     if (PLAYER_WING.size == 0) {
    //         break;
    //     }
    // }
    while (world[current_world].num_of_next_worlds > 0) {
        
        perform_flight();
        heal_wing(&PLAYER_WING, 4);

        if (nodes_content[current_world] == ENEMY) {
            perform_fight();
            if (state == DEFEAT) {
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
    } else {
        sp1_PrintString(&ps0, "YOU WIN :o)");
    }
    update_screen();

}

