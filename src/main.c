#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "Constants.h"
#include "world_generation.h"

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

    world_t* world;
    if(!(world = world_init())){
        fprintf(stderr, "ERROR: WORLD INIT FAILED\n");
        return ERROR;
    }
    world_display_current_map(world, 'N');
    int choice;
    char direction = 'N';
    int running_game = 1;
    while(running_game){
        choice = getch();
        switch(choice){
            case KEY_UP:
                direction = 'N';
                break;
            case KEY_DOWN:
                direction = 'S';
                break;
            case KEY_LEFT:
                direction = 'W';
                break;
            case KEY_RIGHT:
                direction = 'E';
                break;
        }
        if(world_move_map(world, choice) >= 0){
            world_display_current_map(world, direction);
        }else{
            running_game = 0;
            world_destroy(world);
        }
    }
    endwin();
    return 0;
}
