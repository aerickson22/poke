#ifndef HELPER_DISPLAY_FUNCS_H
#define HELPER_DISPLAY_FUNCS_H

/**
 * @file helper_display_funcs.h
 * @brief User interface utility functions for terminal error signaling and coordinates retrieval.
 */

/**
 * @brief Displays an error message inside the dedicated UI overlay/line.
 *
 * @param error_message String containing the error message text to render. Must not be NULL.
 */
void display_ui_error(const char *error_message);

/**
 * @brief Prompts the user via ncurses UI and parses flight coordinates.
 *
 * Safe-guards inputs to ensure target coordinates correspond to valid game locations.
 * Modifies the provided integer references on successful user entry.
 *
 * @param[out] x          Pointer to assign the targeted X world coordinate.
 * @param[out] y          Pointer to assign the targeted Y world coordinate.
 * @param[in]  prompt_row The screen row index where the prompt should be drawn.
 */
void get_flight_coordinates(int *x, int *y, int prompt_row);

#endif /* HELPER_DISPLAY_FUNCS_H */
