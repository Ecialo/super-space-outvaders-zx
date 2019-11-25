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
typedef struct GameState
{
    char state;
    wing player_wing;
    char combat_round;
    unsigned char money;
} game_state;

char ENEMY_CHOICE[3];   // action, most damaged enemy, most damaged player

int init_state(game_state *state) {
    state->state = IDLE;
    // state->current_world = 0;
    init_wing(&state->player_wing);
    // fill_wing_with_rand_ships(&state->player_wing, 3, 0, 0);
    fill_wing_with_rand_ships(&state->player_wing, 5, 0, 0);
    state->combat_round = 0;
    state->money = 0;
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

void perform_action(wing *you, wing *oppose, char action, char choice, char damage_multiplier) {
    ship *you_leader = get_leader(you);
    ship *oppose_leader = get_leader(oppose);
    
    switch (action) {
        case ATTACK:
            take_damage(
                oppose_leader, 
                you_leader->attack, 
                damage_multiplier,
                you_leader->type
            );
            break;
        case RETREAT:
            cycle_ships(you);
            if (you_leader->type == INTERCEPTOR) {
                take_damage(
                    oppose_leader, 
                    you_leader->special, 
                    damage_multiplier,
                    NOT_A_SHIP
                );  
            }
            break;
        case SPECIAL:
            switch (you_leader->type) {
                case INTERCEPTOR:
                    swap_ships(oppose, 0, choice);
                    break;
                case BOMBER:
                    take_damage(
                        get_ship(oppose, choice), 
                        you_leader->special, 
                        damage_multiplier,
                        NOT_A_SHIP
                    );
                    break;
                case DESTROYER:
                    if (you->size > 1) {
                        swap_ships(you, 0, 1);
                    }
                    // TODO RAISE DESTROYER SHIELD
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
    ship player_leader, enemy_leader;

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
            NOT_A_SHIP
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
            NOT_A_SHIP
        );
    }
    player_leader = *get_leader(player);
    enemy_leader = *get_leader(enemy);

    perform_action(player, enemy, player_action, player_choice, damage_multiplier);
    perform_action(enemy, player, enemy_action, enemy_choice, damage_multiplier);
    // TODO COMPLETE PERFORMER
}

void render_scene(wing *player_wing, wing *enemy_wing) {
    render_wing(player_wing, OUR_SIDE);
    render_wing(enemy_wing, THEIR_SIDE);
    inspect_wing(player_wing, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_wing(enemy_wing, &target_inspect_wing_rect, &target_inspect_ship_rect);
}

void prepare(wing *player_wing, wing *enemy_wing) {
    char ship_a, ship_b;

    for (;;) {
        render_scene(player_wing, enemy_wing);
        select_from_prepare_options();
        if (CURSOR_POS == SWAP_OPTION) {
            ship_a = read_ship_i(player_wing, OUR_SIDE);
            ship_b = read_ship_i(player_wing, OUR_SIDE);
            swap_ships(player_wing, ship_a, ship_b);
        } else if (CURSOR_POS == SPY_OPTION) {
            read_ship_i(enemy_wing, THEIR_SIDE);
        } else {
            break;
        }
    }
}

void perform_fight(game_state *state) {
    char player_action, enemy_action; 
    char player_choice, enemy_choice;
    fight_result fight_result;
    wing player_wing;
    player_wing = state->player_wing;
    
    wing enemy_wing;
    init_enemy_wing(&enemy_wing, node_args[current_world]);
    state->combat_round = 0;

    render_scene(&player_wing, &enemy_wing);

    prepare(&player_wing, &enemy_wing);

    for (;;) {

        render_scene(&player_wing, &enemy_wing);

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
            (state->combat_round / 10) + 1,
            player_action,
            player_choice,
            enemy_action,
            enemy_choice
        );
        scrap_dead_ships(&player_wing);
        scrap_dead_ships(&enemy_wing);
        fight_result = check_fight_result(&player_wing, &enemy_wing);
        
        if (fight_result == CONTINUE) {
            state->combat_round++;
        } else if (fight_result == WE) {
            state->state = IDLE;
            MONEY = MONEY + node_args[current_world] + 1;
            inspect_money(MONEY);
            break;
        } else if (fight_result == THEY) {
            state->state = DEFEAT;
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
    sp1_UpdateNow();
}

void perform_shopping(game_state *state) {
    bonus shop[4];
    char i;
    shop[0] = HEAL;
    shop[1] = ATK;
    // for (i = 0; i < 3; i++) {
    //     shop[i + 1] = (bonus) (rand() % (NUM_OF_MOD_TYPES - 1));
    // }
    // collect_bonuses(shop, 4, &state->player_wing, True);
    collect_bonuses(shop, 2, &state->player_wing, True);
}

// MAIN

int main() {
    game_state state;
    init_state(&state);
    generate_world();

    init_all();

    render_wing(&state.player_wing, OUR_SIDE);
    inspect_wing(&state.player_wing, &our_inspect_wing_rect, &our_inspect_ship_rect);
    inspect_money(MONEY);
    while (!world[current_world].is_terminate && world[current_world].num_of_next_worlds > 0) {
        
        perform_flight();
        heal_wing(&state.player_wing, 4);

        if (nodes_content[current_world] == ENEMY) {
            perform_fight(&state);
            if (state.state == DEFEAT) {
                break;
            }
        } else if (nodes_content[current_world] == SHOP) {
            perform_shopping(&state);
        }   
    }
    sp1_ClearRectInv(&full_screen, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR | SP1_RFLAG_SPRITE);
    sp1_SetPrintPos(&ps0, 10, 10);
    if (state.state == DEFEAT) {
        sp1_PrintString(&ps0, "GAME OVER :o(");
    }
    else {
        sp1_PrintString(&ps0, "YOU WIN :o)");
    }
    update_screen();

}