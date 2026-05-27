#include <ncurses.h>
#include <unistd.h>

#include "Constants.h"
#include "helper_display_funcs.h"

void display_ui_error(const char *error_message) {
    move(MAP_MAX_Y + 1, 0);
    clrtoeol();
    attron(A_BOLD);
    printw("ERROR: %s (Press any key)", error_message);
    attroff(A_BOLD);
    refresh();
    getch();
    move(MAP_MAX_Y + 1, 0);
    clrtoeol();
    refresh();
}

void get_flight_coordinates(int *x, int *y, int prompt_row) {
    move(prompt_row, 0);
    clrtoeol();
    printw("Fly to (X Y)? ");
    refresh();
    echo();
    scanw("%d %d", x, y);
    noecho();
    move(prompt_row, 0);
    clrtoeol();
    refresh();
}
