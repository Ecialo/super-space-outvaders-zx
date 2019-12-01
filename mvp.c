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

// GLOBAL GAME STATE
// typedef struct GameState
// {
//     char state;
//     wing player_wing;
//     char combat_round;
//     unsigned char money;
// } game_state;

state state;
wing player_wing;
char COMBAT_ROUND;

char ENEMY_CHOICE[3];   // action, most damaged enemy, most damaged player

int init_state() {
    state = IDLE;
    // state->current_world = 0;
    init_wing(&player_wing);
    // fill_wing_with_rand_ships(&state->player_wing, 3, 0, 0);
    fill_wing_with_rand_ships(&player_wing, 5, 0, 0);
    COMBAT_ROUND = 0;
    // MONEY = 99;
    MONEY = 0;
    return OK;
}

// // READ
char read_action() {
    select_from_battle_options();
    return CURSOR_POS;
}

char read_ship_i(wing *wing, char side) {
    select_from_wing(wing, side);
    return CURSOR_POS;
}

void swap_ships_with_anim(wing *wing, char a, char b, char side) {
    swap_ships_sprites(side, a, b);
    swap_ships(wing, a, b);
}

void gen_ai(wing *enemy, wing *player) {
    ship *leader = get_leader(enemy);
    if (leader->attack > get_leader(player)->health) {
        ENEMY_CHOICE[0] = ATTACK;
        ENEMY_CHOICE[1] = NO_CHOICE;
        ENEMY_CHOICE[2] = NO_CHOICE;
    } else if (
        leader->health < leader->max_health / 2
        && enemy->size > 0 
        && get_ship(enemy, 1)->health > (get_ship(enemy, 1)->max_health / 2)
    ) {
        ENEMY_CHOICE[0] = RETREAT;
        ENEMY_CHOICE[1] = NO_CHOICE;
        ENEMY_CHOICE[2] = NO_CHOICE;
    } else {
        if (leader->type == DESTROYER && enemy->size <= 1) {
            ENEMY_CHOICE[0] = ATTACK;
        } else {
            ENEMY_CHOICE[0] = ((char) rand() % 2) + 1;
        }
        ENEMY_CHOICE[1] = get_most_damaged_ship_i(enemy);
        ENEMY_CHOICE[2] = get_most_damaged_ship_i(player);
    }
}

// ACTIONS

fight_result check_fight_result(wing *player, wing *enemy) {
    if (player->size == 0) {
        return THEY;
    } else if (enemy->size == 0) {
        return WE;
    } else {
        return CONTINUE;
    }
}

void perform_action(wing *you, wing *oppose, char action, char choice, char damage_multiplier, char side) {
    ship *you_leader = get_leader(you);
    ship *oppose_leader = get_leader(oppose);
    
    switch (action) {
        case ATTACK:
            shoot_bullet(0, 0, side);
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
            leader_retreat(OUR_SIDE, you->size - 1);
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
                    swap_ships_with_anim(oppose, 0, choice, !side);
                    // swap_ships(oppose, 0, choice);
                    break;
                case BOMBER:
                    take_damage(
                        get_ship(oppose, choice), 
                        you_leader->special, 
                        damage_multiplier,
                        BOMBER
                    );
                    shoot_bullet(0, choice, side);
                    break;
                case DESTROYER:
                    if (you->size > 1) {
                        swap_ships_with_anim(you, 0, 1, side);
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

void perform_summoner_boss_action(wing *boss_wing, wing *player, char damage_mulipler) {
    if (boss_wing->size <= MAX_WING_SIZE - 2) {
        fill_wing_with_rand_ships(boss_wing, 2, 0, 0);
    } else if (boss_wing->size == MAX_WING_SIZE - 1) {
        fill_wing_with_rand_ships(boss_wing, 1, 0, 1);
    } else {
        fill_wing_with_rand_ships(boss_wing, 0, 0, 2);
    }
    cycle_ships(boss_wing);
}

int perform_round(
    wing *player, 
    wing *enemy, 
    char damage_multiplier,
    char player_action,
    char player_choice,
    char enemy_action,
    char enemy_choice
) {
    char i, wing_size;
    // int result_effect;
    ship *player_leader, *enemy_leader;

    wing_size = player->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(player, i), 
            player->heal
        );
    }
    wing_size = enemy->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            get_ship(enemy, i), 
            enemy->heal
        );
    }

    // result_effect = player->heal - (enemy->missile);
    if (player->heal > enemy->missile) {
        heal(
            get_leader(player), 
            player->heal - enemy->missile
        );
    } else if (player->heal < enemy->missile) {
        take_damage(
            get_leader(player), 
            enemy->missile - player->heal,
            damage_multiplier,
            BOMBER
        );
    }

    // result_effect = enemy->heal - (player->missile);
    if (enemy->heal > (player->missile)) {
        heal(
            get_leader(enemy), 
            enemy->heal - player->missile
        );
    } else if (enemy->heal < (player->missile)) {
        take_damage(
            get_leader(enemy), 
            player->missile - enemy->heal,
            damage_multiplier,
            BOMBER
        );
    }
    player_leader = get_leader(player);
    enemy_leader = get_leader(enemy);

    perform_action(player, enemy, player_action, player_choice, damage_multiplier, OUR_SIDE);
    perform_action(enemy, player, enemy_action, enemy_choice, damage_multiplier, THEIR_SIDE);
    // TODO COMPLETE PERFORMER
}

void render_scene(wing *enemy_wing) {
    render_wing(&player_wing, OUR_SIDE);
    render_wing(enemy_wing, THEIR_SIDE);
    inspect_wing(&player_wing, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_wing(enemy_wing, &target_inspect_wing_rect, &target_inspect_ship_rect);
}

void prepare(wing *enemy_wing) {
    char ship_a, ship_b;

    for (;;) {
        render_scene(enemy_wing);
        select_from_prepare_options();
        if (CURSOR_POS == SWAP_OPTION) {
            ship_a = read_ship_i(&player_wing, OUR_SIDE);
            ship_b = read_ship_i(&player_wing, OUR_SIDE);
            swap_ships_with_anim(&player_wing, ship_a, ship_b, OUR_SIDE);
        } else if (CURSOR_POS == SPY_OPTION) {
            read_ship_i(enemy_wing, THEIR_SIDE);
        } else {
            break;
        }
    }
}

void perform_fight() {
    char player_action, enemy_action; 
    char player_choice, enemy_choice;
    fight_result fight_result;
    // wing player_wing;
    wing enemy_wing;

    // player_wing = &state->player_wing;
    
    init_enemy_wing(&enemy_wing, node_args[current_world]);
    COMBAT_ROUND = 0;

    render_scene(&enemy_wing);

    prepare(&enemy_wing);

    for (;;) {

        render_scene(&enemy_wing);

        player_action = read_action();
        if (player_action == SPECIAL) {
            switch (get_leader(&player_wing)->type) {
                case BOMBER:
                case INTERCEPTOR:
                    player_choice = read_ship_i(&enemy_wing, THEIR_SIDE);
                    break;
                case SUPPORT:
                    player_choice = read_ship_i(&player_wing, OUR_SIDE);
                    break;
                default:
                    player_choice = NO_CHOICE;
                    break;
            }
        } else {
            player_choice = NO_CHOICE;
        }

        gen_ai(&enemy_wing, &player_wing);   // expand this where boss added
        enemy_action = ENEMY_CHOICE[0];
        if (enemy_action == SPECIAL) {
            switch (get_leader(&player_wing)->type) {
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
            &player_wing,
            &enemy_wing,
            (COMBAT_ROUND / 10) + 1,
            player_action,
            player_choice,
            enemy_action,
            enemy_choice
        );
        scrap_dead_ships(&player_wing);
        scrap_dead_ships(&enemy_wing);
        fight_result = check_fight_result(&player_wing, &enemy_wing);
        
        if (fight_result == CONTINUE) {
            COMBAT_ROUND++;
        } else if (fight_result == WE) {
            state = IDLE;
            player_wing.protector = NO_SLOT;
            MONEY = MONEY + node_args[current_world] + 1;
            inspect_money(MONEY);
            break;
        } else if (fight_result == THEY) {
            state = DEFEAT;
            break;
        }
    }
    clear_screen_from_wing(THEIR_SIDE);
}

char read_next_node() {
    select_destination();
    return world[current_world].next_worlds[CURSOR_POS];
}

void perform_flight() {
    draw_map();
    sp1_UpdateNow();
    current_world = read_next_node();
    clear_screen_from_map();
    into_the_hyperspace(player_wing.size);
    sp1_UpdateNow();
}

void perform_shopping() {
    bonus shop[4];
    char i;
    shop[0] = HEAL;
    shop[1] = ATK;
    // for (i = 0; i < 3; i++) {
    //     shop[i + 1] = (bonus) (rand() % (NUM_OF_MOD_TYPES - 1));
    // }
    // collect_bonuses(shop, 4, &state->player_wing, True);
    collect_bonuses(shop, 2, &player_wing, True);
    sp1_ClearRectInv(&costs_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
}

// MAIN
void main() {
    init_state();
    select_map(0);
    generate_world();

    init_all();

    render_wing(&player_wing, OUR_SIDE);
    inspect_wing(&player_wing, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_money(MONEY);
    update_screen();
    while (world[current_world].num_of_next_worlds > 0) {
        
        perform_flight();
        heal_wing(&player_wing, 4);

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

// int main() {
//     static game_state state;
//     int i;
//     volatile int v;
//     i = 0;
//     init_state(&state);
//     select_map(0);
//     generate_world();

//     init_all();
//     render_wing(&state.player_wing, OUR_SIDE);
//     while (1)
//     {
//         sp1_UpdateNow();
//     }
    
//     // while (1){
//     //     select_map(i);
//     //     i = 1 - i;
//     //     generate_world();
//     //     draw_map();
//     //     sp1_UpdateNow();
//     //     v = 5000;
//     //     while(v--);
//     // }
//     // select_destination();

//     sp1_UpdateNow();

//     while(1);
// }

