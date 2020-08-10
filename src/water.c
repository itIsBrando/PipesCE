#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "images/water_1.h"


#define createPosition(p, x, y) Position p; p.x = x; p.y = y;
#define IS_SPOUT(tile) ((tile.flags & 0x4) != 0)

Array flows;
uint8_t waterSpriteBuffer[66];
static bool keepWaterTick = false;
static bool levelComplete;



/* Removes all flows and redraws the water-logged pipes. 
 * Reignites blue fires. 
 * Removes water tiles.
 * @returns none.*/
void wtr_drainWater() {
    const uint8_t width = curLevel.width;
    const uint8_t height = curLevel.height;
    uint8_t y, x;

    for(y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            tile_t tile = getTile(x, y);

            // find spout
            if(pipeFromIndex(tile.id) && tile.data.hasWater)
            {
                tile.data.hasWater = false;
            // reignite blue fires
            } else if(tile.type == TYPE_FIRE && tile.data.fire.isPersistent && !tile.data.fire.isLit)
            {
                tile.data.fire.isLit = true;
                tile.id = TILE_BLUE_FIRE_1;
                firesAlive++;
            // remove any water tiles
            } else if(tile.id == TILE_WATER)
            {
                tile.id = 0;
                scrn_removeAnimation(x, y);
            } else {
                continue;
            }

            setTile(x, y, &tile);
        }
        
    }

    player.isWaterFlowing = false;
}



/* Starts the water flowing out of a spout.
 * Also rotates pipes
 * @returns none.*/
void wtr_initFlows() {
    const uint8_t width = curLevel.width;
    const uint8_t height = curLevel.height;
    uint8_t y, x;

    player.isWaterFlowing = true;
    keepWaterTick = false;

    Array_Clear(&flows);

    for(y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            // find spout
            tile_t tile = chkCollision(x, y);

            if(IS_SPOUT(tile))
            {
                wtr_createFlow(x, y, pipeFromIndex(tile.id)->directions);
                keepWaterTick = true;
            } else if(tile.type == TYPE_ROTATE)
            {
                rotateTile(x, y);
            }

        }
        
    }

    wtr_tickFlows();
}



/* Creates a new flow at (x, y)
 * @param x row
 * @param y column
 * @param dir direction the pipe is facing (can be multiple)
 * @returns none.*/
void wtr_createFlow(uint8_t x, uint8_t y, Direction dir) {
    tile_t *tile = getTilePointer(x, y);
    flow_t f;

    createPosition(position, x, y);
    f.position = position;
    f.spreadDirection = dir;

    Array_Append(&flows, &f);
        
    tile->data.hasWater = true;
    setTile(x, y, tile);
    
}



/* Loops through each flow and moves the water.
 * Will return if no flow is moving
 * @returns none.*/
void wtr_tickFlows() {
    uint8_t i;

    if(!keepWaterTick)
        return;

    // set to false, but if we get a flow that is still going, set to true
    keepWaterTick = false;

    foreach(flows, i)
    {
        const flow_t *f = (flow_t*)Array_Get(&flows, i);

        if(wtr_doFlow(f))
            keepWaterTick = true;
        else
            i--;
        
        if(levelComplete)
            return;
    }
}

/* Checks surrounding pipes to continue the movement of water
 * @param *self pointer to flow_t
 * @returns true if the flow has continued to another pipe.*/
bool wtr_doFlow(flow_t *self) {
    const Direction d = self->spreadDirection;
    Direction directions[4];
    uint8_t i;
    bool hasMoved = false;

    levelComplete = false;
    directions[0] = (d & BIT_RIGHT) ? RIGHT : -1;
    directions[1] = (d & BIT_LEFT) ? LEFT : -1;
    directions[2] = (d & BIT_UP) ? UP : -1;
    directions[3] = (d & BIT_DOWN) ? DOWN : -1;

    for(i = 0; i < 4; i++)
    {
        if(directions[i] != -1)
        {
            Position pos = facingOffset(directions[i]);
            tile_t *tile;
            pipe_t *pipe;
            bool directionsMatch, isPipeOnRotation;

            addPosition(&pos, self->position);
            tile = getTilePointerSafely(pos.x, pos.y);
            pipe = pipeFromIndex(tile->id);

            // ensure that the pipe is facing the proper direction
            directionsMatch = (1<<(uint8_t)getOppositeDirection(directions[i])) & pipe->directions;
            // if there is a pipe on a rotation tile, then force water flow
            isPipeOnRotation = tile->type == TYPE_ROTATE && pipe;

            if((tile->type == TYPE_PIPE || isPipeOnRotation) && !tile->data.hasWater && directionsMatch )
            {
                flow_t f;

                // create a new flow node
                f.spreadDirection = pipe->directions;
                f.position = pos;
                Array_Append(&flows, &f);

                // redraw tile with water
                tile->data.hasWater = true;
                setTile(pos.x, pos.y, tile);
                
                hasMoved = true;
            } else if (tile->type == TYPE_FIRE && tile->data.fire.isLit)
            {
                
                // win if no fires remain
                if(!fre_extinguish(tile, pos.x, pos.y))
                {
                    lvl_complete();
                    levelComplete = true;
                    // when we complete level, any active flows do not get destroyed
                    return false;
                }

                setTile(pos.x, pos.y, tile);
            } else if(tile->id == 0)
            {
                // if we found neither a fire or a pipe, then spill water
                animation_t a;
                
                // create the water animation
                a.x = pos.x;
                a.y = pos.y;
                Array_Append(&animationTile, &a);
                
                // set physical type
                tile->id = TILE_WATER;
                
            }
            
        } 
    }

    wtr_deleteFlow(self);

    if(hasMoved)
        return true;
    else
        return false;
    
}


/* Deletes a flow from the flow Array.
 * inline???
 * @param *self pointer to flow_t.
 * @returns none.*/
void wtr_deleteFlow(flow_t *self) {
    Array_Remove(&flows, self);
}


/* Modifies the water tile to look animated.
 * Probably quite inefficient.
 * @returns none.*/
void wtr_animateWater() {
    uint8_t x, y;
    const uint8_t i = animationTimer;

    for(y = 0; y < 8; y++)
    {
        const uint8_t yWrap = ((y + i) & 7) << 3;
        const uint8_t yPos = y << 3;

        for(x = 0; x < 8; x++)
        {
            waterSpriteBuffer[2 + ((x - i) & 7) + yWrap] = water_1_data[2 + x + yPos];
        }
    }

}