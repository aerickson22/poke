#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "Constants.h"
#include "map_generation.h"

int main(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR_BOULDER,    COLOR_WHITE,   COLOR_BLACK);
    init_pair(COLOR_TREE,       COLOR_GREEN,   COLOR_BLACK);
    init_pair(COLOR_POKE_CENTER, COLOR_RED,     COLOR_BLACK);
    init_pair(COLOR_POKE_MART,  COLOR_BLUE,    COLOR_BLACK);
    init_pair(COLOR_LONG_GRASS, COLOR_GREEN,   COLOR_BLACK);
    init_pair(COLOR_CLEARING,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(COLOR_PATH,       COLOR_YELLOW,  COLOR_BLACK);
    init_pair(COLOR_TILDES,     COLOR_CYAN,    COLOR_BLACK);
    srand(time(NULL));

    map_t* map;
    if(!(map = map_init())){
        fprintf(stderr, "ERROR: MAP INIT FAILED\n");
        return ERROR;
    }
    map_generation(map);
    map_display(map);
    while(getch() != 27);
    map_destroy(map);
    endwin();
    return 0;
}
