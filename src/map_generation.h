#ifndef MAP_GENERATION_H
#define MAP_GENERATION_H

/**
 * @file map_generation.h
 * @brief Public interface for map creation, terrain generation, and rendering.
 */

/**
 * @brief Opaque handle representing a game map.
 *
 * The internal layout of map_t is hidden from callers. Interact with it
 * exclusively through the functions declared in this header.
 */
typedef struct map_t map_t;

/**
 * @brief Renders the map to the terminal using ncurses.
 *
 * Iterates over every tile in the map and draws the appropriate character
 * and color pair to the ncurses window.
 *
 * @param map  Pointer to the map to display. Must not be NULL.
 */
void map_display(map_t* map);

/**
 * @brief Allocates and initialises a new, empty map.
 *
 * The caller is responsible for releasing the returned map with map_destroy()
 * when it is no longer needed.
 *
 * @return Pointer to the newly allocated map, or NULL if allocation fails.
 */
map_t* map_init();

/**
 * @brief Frees all resources owned by a map and invalidates the pointer.
 *
 * @param map  Pointer to the map to destroy. Must not be NULL.
 * @return     SUCCESS (0) on success, ERROR (-1) if @p map is NULL or
 *             an internal error occurs.
 */
int map_destroy(map_t* map);

/**
 * @brief Procedurally generates terrain for an initialised map.
 *
 * Populates every tile in @p in with a terrain type (grass, water, boulders,
 * paths, etc.) using the map-generation algorithm. The map must have been
 * created with map_init() before calling this function.
 *
 * @param in   Pointer to an initialised map to populate. Must not be NULL.
 * @return     SUCCESS (0) on success, ERROR (-1) on failure.
 */
int map_generation(map_t* in);

#endif /* MAP_GENERATION_H */
