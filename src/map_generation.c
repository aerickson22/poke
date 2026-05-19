#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#include "map_generation.h"
#include "Constants.h"
#include "queue.h"

struct vec{
    double x;
    double y;
};

struct map_t{
    Tile** data;
    Tile* exits;
};

struct map_t* map_init() {
    struct map_t* out;
    if (!(out = malloc(sizeof(*out)))) {
        return NULL;
    }
    if (!(out->data = malloc(sizeof(Tile*) * MAP_MAX_Y))) {
        free(out);
        return NULL;
    }
    for (int i = 0; i < MAP_MAX_Y; i++) {
        if (!(out->data[i] = malloc(sizeof(Tile) * MAP_MAX_X))) {
            for (int k = 0; k < i; k++) free(out->data[k]);
            free(out->data);
            free(out);
            return NULL;
        }
    }
    if (!(out->exits = malloc(sizeof(Tile) * NUMBER_OF_EXITS))) {
        for (int i = 0; i < MAP_MAX_Y; i++) free(out->data[i]);
        free(out->data);
        free(out);
        return NULL;
    }
    for (int i = 0; i < MAP_MAX_Y; i++) {
        for (int j = 0; j < MAP_MAX_X; j++) {
            out->data[i][j].x       = j;
            out->data[i][j].y       = i;
            out->data[i][j].terrian = BLANK;
        }
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
            switch(in->data[i][j].terrian){
                case BOULDER:
                    attron(COLOR_PAIR(COLOR_BOULDER));
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_BOULDER));
                    break;
                case TREE:
                    attron(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    break;
                case POKEMON_CENTER:
                    attron(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    break;
                case POKEMARTS:
                    attron(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    break;
                case TALL_GRASS:
                    attron(COLOR_PAIR(COLOR_LONG_GRASS));
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_LONG_GRASS));
                    break;
                case SHORT_GRASS:
                    attron(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    break;
                case PATH:
                    attron(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    break;
                case WATER:
                    attron(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    attroff(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    break;
                default:
                    mvaddch(i + 1, j, in->data[i][j].terrian);
                    break;
            }
        }
    }
}

void _shuffle(int target[], int length) {
    for (int i = length - 1; i > 0; i--) {
        int index = rand() % (i + 1);
        int temp = target[i];
        target[i] = target[index];
        target[index] = temp;
    }
}

int* _permutations() {
    int* out;
    if (!(out = malloc(sizeof(*out) * NUM_PERMS * 2))) {
        return NULL;
    }
    for (int i = 0; i < NUM_PERMS; i++) {
        out[i] = i;
    }
    _shuffle(out, NUM_PERMS);
    for (int i = 0; i < NUM_PERMS; i++) {
        out[i + NUM_PERMS] = out[i];
    }
    return out;
}

struct vec _getConstantVec(int v) {
    int h = v & 3;
    struct vec out;
    if (h == 0)      { out.x =  1.0; out.y =  1.0; }
    else if (h == 1) { out.x = -1.0; out.y =  1.0; }
    else if (h == 2) { out.x = -1.0; out.y = -1.0; }
    else             { out.x =  1.0; out.y = -1.0; }
    return out;
}

double _fade(double t) {
    return ((6.0 * t - 15.0) * t + 10.0) * t * t * t;
}

double _lerp(double t, double a1, double a2) {
    return a1 + t * (a2 - a1);
}

double _dot_product(struct vec v1, struct vec v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

double _noise(int* perms, double x, double y) {
    int big_x = (int)floor(x) & 255;
    int big_y = (int)floor(y) & 255;

    double x_f = x - floor(x);
    double y_f = y - floor(y);

    struct vec top_right    = { x_f - 1.0, y_f - 1.0 };
    struct vec top_left     = { x_f,       y_f - 1.0 };
    struct vec bottom_right = { x_f - 1.0, y_f       };
    struct vec bottom_left  = { x_f,       y_f        };

    int val_top_right    = perms[perms[big_x + 1] + big_y + 1];
    int val_top_left     = perms[perms[big_x]     + big_y + 1];
    int val_bottom_right = perms[perms[big_x + 1] + big_y    ];
    int val_bottom_left  = perms[perms[big_x]     + big_y    ];

    double dot_top_right    = _dot_product(top_right,    _getConstantVec(val_top_right));
    double dot_top_left     = _dot_product(top_left,     _getConstantVec(val_top_left));
    double dot_bottom_right = _dot_product(bottom_right, _getConstantVec(val_bottom_right));
    double dot_bottom_left  = _dot_product(bottom_left,  _getConstantVec(val_bottom_left));

    double u = _fade(x_f);
    double v = _fade(y_f);

    return _lerp(u,
        _lerp(v, dot_bottom_left, dot_top_left),
        _lerp(v, dot_bottom_right, dot_top_right)
    );
}

char biome(double elevation, double moisture) {
    if (elevation < 0.35){
        return WATER;
    }
    if (elevation < 0.45){
        return SHORT_GRASS;
    }
    if (elevation < 0.75) {
        if (moisture < 0.4){
            return SHORT_GRASS;
        }
        if (moisture < 0.7){
            return TALL_GRASS;
        }else{
            return TREE;
        }
    }
    return BOULDER;
}

void _draw_borders(struct map_t* in){
    for (int i = 0; i < MAP_MAX_Y; i++) {
        for (int j = 0; j < MAP_MAX_X; j++) {
            if(!i || !j || i == (MAP_MAX_Y - 1) || j == (MAP_MAX_X - 1)){
                in->data[i][j].terrian = BOULDER;
            }
        }
    }
}

int map_generation(struct map_t* in) {
    int* perms;
    if (!(perms = _permutations())){
        return ERROR;
    }
    int* perms2;
    if (!(perms2 = _permutations())){
        return ERROR;
    }
    for (int i = 0; i < MAP_MAX_Y; i++) {
        for (int j = 0; j < MAP_MAX_X; j++) {
            double e = _noise(perms,  j * 0.075, i * 0.075)
                    + 0.5 * _noise(perms,  j * 0.15, i * 0.15);
            double m = _noise(perms2, j * 0.075, i * 0.075)
                    + 0.5 * _noise(perms2, j * 0.15, i * 0.15);
            e = (e + 1.5) / 3.0;
            m = (m + 1.5) / 3.0;
            in->data[i][j].terrian = biome(e, m);
        }
    }
    free(perms);
    free(perms2);
    _draw_borders(in);
    return SUCCESS;
}


