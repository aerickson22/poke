#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "map_generation.h"
#include "Constants.h"
#include "helper_display_funcs.h"

struct world_t{
    map_t*** data;
    map_t* curr;
    int curr_x;
    int curr_y;
};

void world_destroy(struct world_t* in){
    for(int i = 0; i < WORLD_MAX_Y; i++){
        for(int j = 0; j < WORLD_MAX_X; j++){
            if(in->data[i][j]){
                map_destroy(in->data[i][j]);
            }
        }
    }
    for(int i = 0; i < WORLD_MAX_Y; i++){
        free(in->data[i]);
    }
    free(in->data);
    free(in);
}

struct world_t* world_init(){
    struct world_t* out;
    if (!(out = malloc(sizeof(*out)))) {
        return NULL;
    }
    if (!(out->data = malloc(sizeof(map_t**) * WORLD_MAX_Y))) {
        free(out);
        return NULL;
    }
    for (int i = 0; i < WORLD_MAX_Y; i++) {
        if (!(out->data[i] = malloc(sizeof(map_t*) * WORLD_MAX_X))) {
            for (int k = 0; k < i; k++) free(out->data[k]);
            free(out->data);
            free(out);
            return NULL;
        }
        for(int j = 0; j < WORLD_MAX_X; j++){
            out->data[i][j] = NULL;
        }
    }

    int start_x = WORLD_MAX_X / 2;
    int start_y = WORLD_MAX_Y / 2;

    if(!(out->data[start_y][start_x] = map_init())){
        world_destroy(out);
        return NULL;
    }
    if(map_generation(out->data[start_y][start_x], NULL, NULL) < 0){
        world_destroy(out);
        return NULL;
    }
    out->curr = out->data[start_y][start_x];
    out->curr_x = start_x;
    out->curr_y = start_y;
    return out;
}

int world_move_map(struct world_t* in, int choice){
    switch(choice){
        case KEY_UP:
            if(in->curr_y > 0){
                in->curr_y--;
            }else{
                display_ui_error("OUT OF BOUNDS");
                return 0;
            }
            if(!in->data[in->curr_y][in->curr_x]){
                in->data[in->curr_y][in->curr_x] = map_init();
                map_generation(in->data[in->curr_y][in->curr_x], NULL, map_get_exit(in->curr, 'n'));
            }
            break;

        case KEY_DOWN:
            if(in->curr_y < WORLD_MAX_Y - 1){
                in->curr_y++;
            }else{
                display_ui_error("OUT OF BOUNDS");
                return 0;
            }
            if(!in->data[in->curr_y][in->curr_x]){
                in->data[in->curr_y][in->curr_x] = map_init();
                map_generation(in->data[in->curr_y][in->curr_x], NULL, map_get_exit(in->curr, 's'));
            }
            break;

        case KEY_LEFT:
            if(in->curr_x > 0){
                in->curr_x--;
            }else{
                display_ui_error("OUT OF BOUNDS");
                return 0;
            }
            if(!in->data[in->curr_y][in->curr_x]){
                in->data[in->curr_y][in->curr_x] = map_init();
                map_generation(in->data[in->curr_y][in->curr_x], map_get_exit(in->curr, 'w'), NULL);
            }
            break;

        case KEY_RIGHT:
            if(in->curr_x < WORLD_MAX_X - 1){
                in->curr_x++;
            }else{
                display_ui_error("OUT OF BOUNDS");
                return 0;
            }
            if(!in->data[in->curr_y][in->curr_x]){
                in->data[in->curr_y][in->curr_x] = map_init();
                map_generation(in->data[in->curr_y][in->curr_x], map_get_exit(in->curr, 'e'), NULL);
            }
            break;

        case 'f': {
            int to_x, to_y;
            get_flight_coordinates(&to_x, &to_y, MAP_MAX_Y + 1);

            if(to_x < 0 || to_x > WORLD_MAX_X - 1 || to_y < 0 || to_y > WORLD_MAX_Y - 1){
                display_ui_error("OUT OF BOUNDS");
                return 0;
            }

            in->curr_x = to_x;
            in->curr_y = to_y;

            if(!in->data[in->curr_y][in->curr_x]){
                in->data[in->curr_y][in->curr_x] = map_init();
                map_generation(in->data[in->curr_y][in->curr_x], NULL, NULL);
            }
            break;
        }

        case 27:
        case 'Q':
            return -1;
    }

    in->curr = in->data[in->curr_y][in->curr_x];
    return 0;
}

void world_display_current_map(struct world_t* in, char direction){
    map_display(in->curr);
    move(0, 0);
    printw("Player at (%d, %d): Moving %c", in->curr_x, in->curr_y, direction);
}

