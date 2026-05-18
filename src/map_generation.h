#ifndef MAP_GENERATION_H
#define MAP_GENERATION_H

//Opadque Map Type
typedef struct map_t map_t;

//Displays a map_t
void map_display(map_t*);
//initalize a map_t and returns pointer to it
map_t* map_init();
//destory a map_t
int map_destory(map_t*);

#endif
