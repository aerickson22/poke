#ifndef CONSTANTS_H
#define CONSTANTS_H

//STATES and ERRORS
#define ERROR -1
#define SUCCESS 0

//Consts numbers for map gen
#define MAP_MAX_X 80
#define MAP_MAX_Y 21
#define POKEMON_STORE_X_SIZE 2
#define POKEMON_STORE_Y_SIZE 2
#define NUMBER_OF_EXITS 4

//Rendering characters for screen
#define PATH '#'
#define POKEMON_CENTER 'C'
#define POKEMARTS 'M'
#define TALL_GRASS ':'
#define WATER '~'
#define BOULDER '%'
#define SHORT_GRASS '.'
#define TREE '^'

//Ncurses
#define COLOR_BOULDER 1
#define COLOR_TREE 2
#define COLOR_POKE_CENTER 3
#define COLOR_POKE_MART 4
#define COLOR_LONG_GRASS 5
#define COLOR_CLEARING 6
#define COLOR_PATH 7
#define COLOR_TILDES 8

//data structures
typedef struct map_point_t{
    int x;
    int y;
}map_point_t;

#endif
