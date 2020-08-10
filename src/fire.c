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


/* Called when a level is loaded.  
 * Determines how many fires need to be destroyed for level completion.
 * @returns none.*/
void fre_init() {
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


/* Creates a fire. 
 * Only called in 'loadMap'
 * @param *tile tile_t pointer to change into a fire
 * @returns none*/
void fre_create(tile_t *tile) {
    fire_t *f = &tile->data.fire;
    
    f->isLit = true;
    f->isPersistent = tile->id != TILE_FIRE_1;
    tile->type = TYPE_FIRE;
}



/* Deletes a fire. 
 * Creates an animated water tile.
 * @param *t tile at the location (x, y)
 * @param x row
 * @param y column
 * @returns none.*/
uint8_t fre_extinguish(tile_t *t, uint8_t x, uint8_t y) {
    uint8_t i;

    firesAlive--;

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