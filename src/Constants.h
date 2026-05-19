#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file constants.h
 * @brief Project-wide constants, render characters, ncurses color IDs, and
 *        shared data structures.
 */

/** @brief Returned by functions to signal an unrecoverable error. */
#define ERROR   -1

/** @brief Returned by functions to signal successful completion. */
#define SUCCESS  0

/** @brief Maximum number of columns in the map grid. */
#define MAP_MAX_X             80

/** @brief Maximum number of rows in the map grid. */
#define MAP_MAX_Y             21

/** @brief Width in tiles of a Pokémon Center or Poké Mart building. */
#define POKEMON_STORE_X_SIZE   2

/** @brief Height in tiles of a Pokémon Center or Poké Mart building. */
#define POKEMON_STORE_Y_SIZE   2

/** @brief Number of directional exits on the map border (N, S, E, W). */
#define NUMBER_OF_EXITS        4

/** @brief Total number of distinct terrain types used during generation. */
#define NUMBER_OF_TERRAINS     7

/** @brief Size of the permutation table used by the noise function (2^8). */
#define NUM_PERMS            256

#define NUM_NEIGHBORS 8

/** @brief Path / road tile. */
#define PATH            '#'

/** @brief Pokémon Center building tile. */
#define POKEMON_CENTER  'C'

/** @brief Poké Mart building tile. */
#define POKEMARTS       'M'

/** @brief Tall grass tile (wild Pokémon encounters). */
#define TALL_GRASS      ':'

/** @brief Water tile. */
#define WATER           '~'

/** @brief Boulder / impassable rock tile. */
#define BOULDER         '%'

/** @brief Short grass / clearing tile. */
#define SHORT_GRASS     '.'

/** @brief Tree tile. */
#define TREE            '^'

/** @brief Empty / void tile. */
#define BLANK           ' '

/** @brief Color pair for boulder tiles. */
#define COLOR_BOULDER     1

/** @brief Color pair for tree tiles. */
#define COLOR_TREE        2

/** @brief Color pair for Pokémon Center tiles. */
#define COLOR_POKE_CENTER 3

/** @brief Color pair for Poké Mart tiles. */
#define COLOR_POKE_MART   4

/** @brief Color pair for tall-grass tiles. */
#define COLOR_LONG_GRASS  5

/** @brief Color pair for short-grass / clearing tiles. */
#define COLOR_CLEARING    6

/** @brief Color pair for path tiles. */
#define COLOR_PATH        7

/** @brief Color pair for water tiles. */
#define COLOR_TILDES      8

/**
 * @brief Represents a single tile on the map.
 *
 * Stores the tile's grid position and the terrain character used to render it.
 */
typedef struct Tile {
    int  x;        /**< Column index of the tile (0-based). */
    int  y;        /**< Row index of the tile (0-based).    */
    char terrain;  /**< Terrain character (see render-character constants). */
} Tile;

#endif /* CONSTANTS_H */
