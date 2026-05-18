#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "map_generation.h"
#include "Constants.h"

struct map_t{
    char** data;
    map_point_t* exits;
};

struct map_t* map_init(){
    struct map_t* out;
    if(!(out = malloc(sizeof(*out)))){
        return NULL;
    }
    if(!(out->data = malloc(sizeof((**(out->data))) * MAP_MAX_Y))){
        return NULL;
    }
    for(int i = 0; i < MAP_MAX_X; i++){
        if(!(out->data[i] = malloc(sizeof(*(out->data)) * MAP_MAX_X))){
            return NULL;
        }
    }
    if(!(out->exits = malloc(sizeof((*(out->exits))) * NUMBER_OF_EXITS))){
        return NULL;
    }
    return out;
}

int map_destory(struct map_t* in){
    for(int i = 0; i < MAP_MAX_Y; i++){
        free(in->data[i]);
    }
    free(in->data);
    free(in->exits);
    free(in);
    return SUCCESS;
}

void map_display(struct map_t* in){
    for(int i = 0; i < MAP_MAX_Y; i++){
        for(int j = 0; j < MAP_MAX_X; j++){
            switch(in->data[i][j]){
                case BOULDER:
                    attron(COLOR_PAIR(COLOR_BOULDER));
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_BOULDER));
                    break;
                case TREE:
                    attron(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    break;
                case POKEMON_CENTER:
                    attron(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    break;
                case POKEMARTS:
                    attron(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    break;
                case TALL_GRASS:
                    attron(COLOR_PAIR(COLOR_LONG_GRASS));
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_LONG_GRASS));
                    break;
                case SHORT_GRASS:
                    attron(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    break;
                case PATH:
                    attron(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    break;
                case WATER:
                    attron(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j]);
                    attroff(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    break;
                default:
                    mvaddch(i + 1, j, in->data[i][j]);
                    break;
            }
        }
    }
}


