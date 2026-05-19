#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "map_generation.h"
#include "Constants.h"
#include "minheap.h"

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
            out->data[i][j].x = j;
            out->data[i][j].y = i;
            out->data[i][j].distances = INT_MAX;
            out->data[i][j].predcessors = NULL;
            out->data[i][j].terrain = BLANK;
            out->data[i][j].visted = 0;
        }
    }

    return out;
}

int map_destroy(struct map_t* in){
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
            switch(in->data[i][j].terrain){
                case BOULDER:
                    attron(COLOR_PAIR(COLOR_BOULDER));
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_BOULDER));
                    break;
                case TREE:
                    attron(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_TREE) | A_BOLD);
                    break;
                case POKEMON_CENTER:
                    attron(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_POKE_CENTER) | A_BOLD);
                    break;
                case POKEMARTS:
                    attron(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_POKE_MART) | A_BOLD);
                    break;
                case TALL_GRASS:
                    attron(COLOR_PAIR(COLOR_LONG_GRASS));
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_LONG_GRASS));
                    break;
                case SHORT_GRASS:
                    attron(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_CLEARING) | A_BOLD);
                    break;
                case PATH:
                    attron(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_PATH) | A_BOLD);
                    break;
                case WATER:
                    attron(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    mvaddch(i + 1, j, in->data[i][j].terrain);
                    attroff(COLOR_PAIR(COLOR_TILDES) | A_BOLD);
                    break;
                default:
                    mvaddch(i + 1, j, in->data[i][j].terrain);
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
                in->data[i][j].terrain = BOULDER;
            }
        }
    }
}

int _compare_tiles(void* t1, void* t2){
    return (*(Tile**)t1)->distances - (*(Tile**)t2)->distances;
}

int _distance_to(char dest){
    switch(dest){
        case PATH:
        case POKEMARTS:
        case POKEMON_CENTER:
        case SHORT_GRASS:
            return 10;
        case TALL_GRASS:
            return 20;
        case TREE:
            return 50;
        case BOULDER:
            return INT_MAX;
        case WATER:
            return 80;
        default:
            return 15;
    }
}

void _thicken_paths(struct map_t* in) {
    int is_path[MAP_MAX_Y][MAP_MAX_X] = {0};
    for (int y = 0; y < MAP_MAX_Y; y++) {
        for (int x = 0; x < MAP_MAX_X; x++) {
            if (in->data[y][x].terrain == PATH) {
                is_path[y][x] = 1;
            }
        }
    }
    for (int y = 1; y < MAP_MAX_Y - 1; y++) {
        for (int x = 1; x < MAP_MAX_X - 1; x++) {
            if (is_path[y][x]) {
                if (in->data[y+1][x].terrain != BOULDER && in->data[y+1][x].terrain != WATER)
                    in->data[y+1][x].terrain = PATH;
                if (in->data[y-1][x].terrain != BOULDER && in->data[y-1][x].terrain != WATER)
                    in->data[y-1][x].terrain = PATH;
                if (in->data[y][x+1].terrain != BOULDER && in->data[y][x+1].terrain != WATER)
                    in->data[y][x+1].terrain = PATH;
                if (in->data[y][x-1].terrain != BOULDER && in->data[y][x-1].terrain != WATER)
                    in->data[y][x-1].terrain = PATH;
            }
        }
    }
}

int  _draw_vertical(struct map_t* in){
    int start = (rand() % (MAP_MAX_X - 3)) + 3;
    Tile* src = &in->data[MAP_MAX_Y - 1][start];
    src->distances = 0;
    src->terrain = PATH;
    Tile* dest = &in->data[0][start];
    dest->terrain = PATH;
    minheap_t* tiles;
    int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    if(!(tiles = minheap_init(sizeof(src)))){
        return ERROR;
    }
    if(minheap_insert(&src, tiles, _compare_tiles) < 0){
        minheap_destroy(tiles);
        return ERROR;
    }
    while (!minheap_is_empty(tiles)) {
        if (!(src = *(Tile**)minheap_remove(tiles, _compare_tiles))) {
            minheap_destroy(tiles);
            return ERROR;
        }
        src->visted = 1;

        if (src->x == dest->x && src->y == dest->y) {
            while(src->predcessors){
                src->terrain = PATH;
                src = src->predcessors;
            }
            minheap_destroy(tiles);
            return SUCCESS;
        }
        for (int i = 0; i < 8; i++) {
            if (!(src->y + dy[i] < 0 || src->y + dy[i] >= MAP_MAX_Y ||
                src->x + dx[i] < 0 || src->x + dx[i] >= MAP_MAX_X ||
                in->data[src->y + dy[i]][src->x + dx[i]].visted)) {
                Tile* neighbor = &in->data[src->y + dy[i]][src->x + dx[i]];
                int dist = _distance_to(neighbor->terrain);
                if (dist != INT_MAX) {
                    int new_dist = src->distances + dist;
                    if (new_dist < neighbor->distances) {
                        neighbor->distances = new_dist;
                        neighbor->predcessors = src;
                        if (minheap_insert(&neighbor, tiles, _compare_tiles) < 0) {
                            minheap_destroy(tiles);
                            return ERROR;
                        }
                    }
                }
            }
        }
    }
    minheap_destroy(tiles);
    return SUCCESS;
}

int _draw_horizontal(struct map_t* in){
    int start = (rand() % (MAP_MAX_Y - 3)) + 3;
    Tile* src = &in->data[start][0];
    src->distances = 0;
    src->terrain = PATH;
    Tile* dest = &in->data[start][MAP_MAX_X - 1];
    dest->terrain = PATH;
    minheap_t* tiles;
    int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    if(!(tiles = minheap_init(sizeof(src)))){
        return ERROR;
    }
    if(minheap_insert(&src, tiles, _compare_tiles) < 0){
        minheap_destroy(tiles);
        return ERROR;
    }
    while (!minheap_is_empty(tiles)) {
        if (!(src = *(Tile**)minheap_remove(tiles, _compare_tiles))) {
            minheap_destroy(tiles);
            return ERROR;
        }
        src->visted = 1;

        if (src->x == dest->x && src->y == dest->y) {
            while(src->predcessors){
                src->terrain = PATH;
                src = src->predcessors;
            }
            minheap_destroy(tiles);
            return SUCCESS;
        }
        for (int i = 0; i < 8; i++) {
            if (!(src->y + dy[i] < 0 || src->y + dy[i] >= MAP_MAX_Y ||
                src->x + dx[i] < 0 || src->x + dx[i] >= MAP_MAX_X ||
                in->data[src->y + dy[i]][src->x + dx[i]].visted)) {
                Tile* neighbor = &in->data[src->y + dy[i]][src->x + dx[i]];
                int dist = _distance_to(neighbor->terrain);
                if (dist != INT_MAX) {
                    int new_dist = src->distances + dist;
                    if (new_dist < neighbor->distances) {
                        neighbor->distances = new_dist;
                        neighbor->predcessors = src;
                        if (minheap_insert(&neighbor, tiles, _compare_tiles) < 0) {
                            minheap_destroy(tiles);
                            return ERROR;
                        }
                    }
                }
            }
        }
    }
    minheap_destroy(tiles);
    return SUCCESS;
}

void _reset_tiles(struct map_t* in) {
    for (int y = 0; y < MAP_MAX_Y; y++) {
        for (int x = 0; x < MAP_MAX_X; x++) {
            in->data[y][x].visted = 0;
            in->data[y][x].distances = INT_MAX;
            in->data[y][x].predcessors = NULL;
        }
    }
}

int _draw_paths(struct map_t* in) {
    if (_draw_vertical(in) < 0){
        return ERROR;
    }
    _reset_tiles(in);
    if (_draw_horizontal(in) < 0){
        return ERROR;
    }
    _thicken_paths(in);
    return SUCCESS;
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
            in->data[i][j].terrain = biome(e, m);
        }
    }
    free(perms);
    free(perms2);
    _draw_borders(in);
    _draw_paths(in);
    return SUCCESS;
}


