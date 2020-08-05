#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"

// Array fires;

uint8_t firesAlive;


void initFire() {
    const unsigned int width = curLevel.width;
    const unsigned int height = curLevel.height;
    unsigned int i;
    
    firesAlive = 0;

    for(i = 0; i < width * height; i++) {
        tile_t tile = curLevel.data[i];

        if(IS_FIRE(tile))
            firesAlive++;
    }
}


void createFire(tile_t *tile) {
    fire_t *f = &tile->data.fire;
    
    f->isLit = true;
    f->isPersistent = tile->id == TILE_FIRE_1 ? false : true;
    tile->type = TYPE_FIRE;
}


uint8_t putOutFire(tile_t *t, uint8_t x, uint8_t y) {
    uint8_t i;

    firesAlive--;
    dbg_sprintf(dbgerr, "Fire put out: (%d, %d), %d\n", x, y, firesAlive);

    if(t->data.fire.isPersistent) {

        t->data.fire.isLit = false;
        t->id = TILE_WATER;
        
    } else {
        // replace current tile with water
        t->id = TILE_WATER;
        t->flags = t->type = 0;
    }
    
    

    return firesAlive;
}