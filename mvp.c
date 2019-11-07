#include <string.h>
#include "src/base_sp1.c"
#include "src/draw_wing.c"
#include "src/cursor.c"
#include "src/inspect.c"
#include "src/utils.c"
#include "src/ship.c"
#include "src/wing.c"


#define MAX_NEXT_WORLDS 3
#define WORLD_SIZE 5

#define NO_MODS 0
#define TORPEDO 1
#define REMTECH 2

#define NUM_OF_ACTIONS 3
#define NO_CHOICE 7

#define OUR_SIDE 0
#define THEIR_SIDE 1


typedef enum State {
    IDLE,
    FLY,
    FIGHT,
    VICTORY,
    DEFEAT,
} state;

typedef enum ContentType {
    JUST,
} content_type;

typedef enum Action {
    RETREAT,
    ATTACK,
    SPECIAL,
} action;

typedef enum FightResults {
    CONTINUE,
    THEY,
    WE,
} fight_result;

// GLOBAL GAME STATE
typedef struct GameState
{
    char state;
    char current_world;
    wing player_wing;
    char combat_round;
} game_state;

int init_state(game_state *state) {
    state->state = IDLE;
    state->current_world = 0;
    init_wing(&state->player_wing);
    state->combat_round = 0;
    return OK;
}

// int render_ship(ship *ship, char in_wing_pos) {
//     ship_type type;
//     char i, casted_type;
    
//     i = in_wing_pos;
//     type = ship->type;
//     // casted_type = (char) type;
//     // wing_sprites_i_to_render[i] = casted_type;
//     return OK;
// }

// void render_ship_stats(ship *ship, char side) {
    
// }

// int render_wing(wing *wing, char side) {
//     char i;
//     // printf("WING\n");
//     // printf("Size %d fempty %d\n", wing->size, wing->first_empty_slot);
    
    
//     for (i = 0 + side; i < wing->size + side; i++) {
//         // wing_sprites_i_to_render[i] = 1;
//         render_ship(get_ship(wing, i - side), i);
//     }
//     for (; i < MAX_WING_SIZE + side; i++) {
//         wing_sprites_i_to_render[i] = NOT_A_SHIP;
//     }
// }

// int render_victory() {
//     // printf("HOORAY!\n");
//     return OK;
// }


// // READ
char read_action() {
    select_from_battle_options();
    return CURSOR_POS;
}

char read_ship_i(wing *wing, char side) {
    select_from_wing(wing, side);
    return CURSOR_POS;
}

// // ACTIONS
// int perform_fly(game_state *state) {

//     world_node node;
//     node = world[state->current_world];

//     if (node.is_terminate) {
//         state->state = VICTORY;
//         return OK;
//     }

//     char i, num_of_next_worlds;
//     int d;
//     num_of_next_worlds = node.num_of_next_worlds;
//     printf("Currently we are in ");
//     render_world(&node);
//     printf("We can fly to:\n");
//     for (i = 0; i < num_of_next_worlds; i++) {
//         printf("%d - ", i);
//         render_world(&world[node.next_worlds[i]]);
//     }
//     printf("Choose\n");
//     scanf("%i", &d);
//     // d = d - '0';
//     if (d >= 0 && d < num_of_next_worlds) {
//         state->current_world = node.next_worlds[d];
//     }
//     return OK;
// }

fight_result check_fight_result(wing *player, wing *enemy) {
    if (player->size == 0) {
        return THEY;
    } else if (enemy->size == 0) {
        return WE;
    } else {
        return CONTINUE;
    }
}

int perform_action(wing *you, wing *oppose, action action, char choice, char damage_multiplier) {
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
                    swap_ships(you, 0, 1);
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
    action player_action,
    char player_choice,
    action enemy_action,
    char enemy_choice
) {
    char i, wing_size, result_effect;
    ship player_leader, enemy_leader;

    wing_size = player->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            // &(player->ships[player->arrange[i]]),
            get_ship(player, i), 
            player->heal
        );
    }
    wing_size = enemy->size;
    for (i = 1; i < wing_size; i++) {
        heal(
            // &(enemy->ships[enemy->arrange[i]]), 
            get_ship(enemy, i), 
            enemy->heal
        );
    }

    result_effect = player->heal - (enemy->missile) * 2;
    if (result_effect > 0) {
        heal(
            // &(player->ships[player->arrange[0]]), 
            get_leader(player), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            // &(player->ships[player->arrange[0]]), 
            get_leader(player), 
            result_effect,
            damage_multiplier,
            NOT_A_SHIP
        );
    }

    result_effect = enemy->heal - (player->missile) * 2;
    if (result_effect > 0) {
        heal(
            // &(enemy->ships[enemy->arrange[0]]), 
            get_leader(enemy), 
            result_effect
        );
    } else if (result_effect < 0) {
        take_damage(
            // &(enemy->ships[enemy->arrange[0]]), 
            get_leader(enemy), 
            result_effect,
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

void perform_fight(game_state *state) {
    action player_action, enemy_action; 
    char player_choice, enemy_choice;
    fight_result fight_result;
    wing player_wing;
    player_wing = state->player_wing;
    
    wing enemy_wing;
    init_enemy_wing(&enemy_wing);

    for(;;) {
        render_wing(&player_wing, OUR_SIDE);
        render_wing(&enemy_wing, THEIR_SIDE);
        inspect_ship(get_leader(&player_wing), 18, 10);
        inspect_ship(get_leader(&enemy_wing), 18, 3);

        player_action = (action) read_action();
//         printf("%d", (int) player_action);
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

        enemy_action = (action) (rnd() % NUM_OF_ACTIONS);
        if (enemy_action == SPECIAL) {
            switch (get_leader(&player_wing)->type) {
                case BOMBER:
                    enemy_choice = (char) (rnd() % player_wing.size);
                    break;
                case SUPPORT:
                case INTERCEPTOR:
                    enemy_choice = (char) (rnd() % enemy_wing.size);
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
            break;
        } else if (fight_result == THEY) {
            state->state = DEFEAT;
            break;
        }
    }
}

// int perform_idle(game_state *state) {
//     world_node node;
//     node = world[state->current_world];
//     render_world(&node);
//     return OK;
// }

// MAIN

void just_fight() {
    game_state state;
    init_state(&state);
    add_ship(&state.player_wing, "KUKAREK", DESTROYER);
    add_ship(&state.player_wing, "KUKARE2", BOMBER);
    perform_fight(&state);
}

int main() {
    init_sp1();
    init_icons();
    init_cursor();
    init_ship_sprites();
    init_inspector();

    just_fight();

    // init
    // state state;
    // init_state(&state);
    // generate_world();

    // // main game loop
    // for (a = 0; a < 10; a++) {
    //     if (state.state == FLY) { perform_fly(&state); }
    //     else if (state.state == FIGHT) { perform_fight(&state); }
    //     else if (state.state == IDLE) { perform_idle(&state); }
    //     else if (state.state = VICTORY) {
    //         render_victory();
    //         break;
    //     }
    //     printf("\n");
    // }
}