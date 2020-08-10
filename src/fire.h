#ifndef FIRE_H
#define FIRE_H

#include "main.h"

#define IS_FIRE(t) ((t).id == TILE_FIRE_1 || (t).id == TILE_BLUE_FIRE_1)


extern uint8_t firesAlive;

// extern Array fires;


void fre_init();
void fre_create(tile_t *);
uint8_t fre_extinguish(tile_t *t, uint8_t x, uint8_t y);

#endif