#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "map_generation.h"
#include "Constants.h"

struct world_t{
    map_t*** data;
    map_t* curr;
};

void world_destroy(struct world_t* in){
    for(int i = 0; i < WORLD_MAX_Y; i++){
        for(int j = 0; j < WORLD_MAX_X; j++){
            if(in->data[i][j]){
                map_destroy(in->data[i][j]);
            }
        }
    }
    for(int i = 0; i < WORLD_MAX_X; i++){
        free(in->data[i]);
    }
    free(in->data);
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
    if(!(out->data[201][201] = map_init())){
        world_destroy(out);
        return NULL;
    }
    if(map_generation(out->data[201][201]) < 0){
        world_destroy(out);
        return NULL;
    }
    out->curr = out->data[201][201];
    return out;
}

int world_move_map(struct world_t* in){
    in->curr++;
    return 0;
}

void world_display_current_map(struct world_t* in){
    map_display(in->curr);
}

