#ifndef FIRE_H
#define FIRE_H

#include "main.h"

#define IS_FIRE(t) ((t).id == TILE_FIRE_1 || (t).id == TILE_BLUE_FIRE_1)


extern uint8_t firesAlive;

// extern Array fires;


void initFire();
void createFire(tile_t *);
uint8_t putOutFire(tile_t *t, uint8_t x, uint8_t y);

#endif