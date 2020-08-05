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


void drainWater() {
    const uint8_t width = curLevel.width;
    const uint8_t height = curLevel.height;
    uint8_t y, x;

    for(y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            tile_t tile = getTile(x, y);

            // find spout
            if(IS_PIPE(tile) && tile.data.hasWater)
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
                removeAnimation(x, y);
            } else {
                continue;
            }

            setTile(x, y, &tile);
        }
        
    }

    player.isWaterFlowing = false;
}

void rotateTile(uint8_t x, uint8_t y) {
    uint8_t i;
    uint8_t ogTile[] = {TILE_PIPE_VERTICAL, TILE_PIPE_HORIZONTAL, TILE_PIPE_TOP_LEFT, TILE_PIPE_TOP_RIGHT, TILE_PIPE_BOT_RIGHT, TILE_PIPE_BOT_LEFT};
    uint8_t newTile[] = {TILE_PIPE_HORIZONTAL, TILE_PIPE_VERTICAL, TILE_PIPE_TOP_RIGHT, TILE_PIPE_BOT_RIGHT, TILE_PIPE_BOT_LEFT, TILE_PIPE_TOP_LEFT};
    tile_t *tile = getTilePointer(x, y);

    for(i = 0; i < sizeof(ogTile); i++)
    {
        if(tile->id == ogTile[i])
        {
            
            tile->id = newTile[i];
            setTile(x, y, tile);
            return;
        }
    }

}

void initFlows() {
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
                createFlow(x, y, pipeFromIndex(tile.id)->directions);
                keepWaterTick = true;
            } else if(tile.type == TYPE_ROTATE)
            {
                rotateTile(x, y);
            }
        }
        
    }

    tickFlows();
}


bool createFlow(uint8_t x, uint8_t y, Direction dir) {
    tile_t *tile = getTilePointer(x, y);
    flow_t f;

    createPosition(position, x, y);
    f.position = position;
    f.spreadDirection = dir;

    Array_Append(&flows, &f);
    
    //dbg_sprintf(dbgerr, "createflow dir: %d, array dir: %d, (%d,%d)\n", f.spreadDirection, newF->spreadDirection, newF->position.x, newF->position.y);
    
    tile->data.hasWater = true;
    setTile(x, y, tile);
    
    return true;
}


void tickFlows() {
    uint8_t i;

    if(!keepWaterTick)
        return;

    // set to false, but if we get a flow that is still going, set to true
    keepWaterTick = false;

    foreach(flows, i) {
        const flow_t *f = (flow_t*)Array_Get(&flows, i);

        if(doFlow(f))
            keepWaterTick = true;
        else
            i--;
    }
}


bool doFlow(flow_t *self) {
    const Direction d = self->spreadDirection;
    Direction directions[4];
    uint8_t i;
    bool hasMoved = false;

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
            bool directionsMatch;
            addPosition(&pos, self->position);
            tile = getTilePointerSafely(pos.x, pos.y);

            directionsMatch = (1<<(uint8_t)getOppositeDirection(directions[i])) & pipeFromIndex(tile->id)->directions;
            if(tile->type == TYPE_PIPE && !tile->data.hasWater && directionsMatch )
            {
                flow_t f;

                f.spreadDirection = pipeFromIndex(tile->id)->directions;
                f.position = pos;
                Array_Append(&flows, &f);
            
                tile->data.hasWater = true;
                setTile(pos.x, pos.y, tile);
                dbg_sprintf(dbgerr, "found flow at (%d, %d)\n", pos.x, pos.y);
                
                hasMoved = true;
            } else if (tile->type == TYPE_FIRE && tile->data.fire.isLit)
            {
                
                // win if no fires remain
                if(!putOutFire(tile, pos.x, pos.y))
                {
                    completeLevel();
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

    deleteFlow(self);

    if(hasMoved)
        return true;
    else
        return false;
    
}


void deleteFlow(flow_t *self) {
    // must be able to look up stuff
    uint8_t i;

    // Array_Remove(&flows, self);
    foreach(flows, i)
    {
        if((flow_t *)Array_Get(&flows, i) == self)
            Array_RemoveAt(&flows, i);
            // Array_Remove(&flows, self);
    }

}

void animateWater() {
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