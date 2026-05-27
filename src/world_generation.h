#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

/**
 * @file world_generation.h
 * @brief Interface for regional management, map tracking, and world navigation traversal.
 */

/**
 * @brief Opaque structure representing the global game world.
 *
 * Contains a grid layout mapping coordinates to individual map instances,
 * as well as tracker pointers for the player's current map grid coordinates.
 */
typedef struct world_t world_t;

/**
 * @brief Allocates and initialises a new game world tracking environment.
 *
 * Instantiates grid buffers and sets up the starting map instance coordinates.
 *
 * @return Pointer to the newly allocated world environment, or NULL on allocation failure.
 */
world_t* world_init();

/**
 * @brief Frees all memory allocations associated with the world and its sub-maps.
 *
 * @param world Pointer to the world instance to destroy.
 */
void world_destroy(world_t* world);

/**
 * @brief Moves the player's active focus across maps using a specific cardinal action.
 *
 * Evaluates target boundary constraints and links or instantiates maps dynamically
 * when cross-border movement occurs.
 *
 * @param world     Pointer to the active world container.
 * @param direction Integer representation of the direction to travel.
 * @return          SUCCESS (0) on successful navigation, ERROR (-1) if movement is illegal.
 */
int world_move_map(world_t* world, int direction);

/**
 * @brief Requests the rendering sequence for the world's currently focused map.
 *
 * Acts as a tracking pass-through wrapper for individual rendering routines.
 *
 * @param world        Pointer to the active world context.
 * @param command_mode A character identifier flag tracking active user display modes.
 */
void world_display_current_map(world_t* world, char command_mode);

#endif /* WORLD_GENERATION_H */
