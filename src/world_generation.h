#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

    typedef struct world_t world_t;

    world_t* world_init();
    void world_destroy(world_t*);
    int world_move_map(world_t*);
    void world_display_current_map(world_t*);

#endif
