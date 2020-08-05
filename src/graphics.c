#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "images/gfx.h"

Array animationTile;
uint8_t animationTimer;
static gfx_sprite_t *playerS = playerSprite;


/* Initializes the 'animationTiles' Array.
 * Should be called before a level is drawn
 * @returns none */
void initAnimations() {
    uint8_t x, y;

    Array_Clear(&animationTile);
    animationTimer = 0;

    for(y = 0; y < curLevel->height; y++)
    {
        for (x = 0; x < curLevel->width; x++)
        {
            tile_t tile = getTile(x, y);
            animation_t a;

            a.x = x;
            a.y = y;

            switch (tile.id)
            {
            case TILE_FIRE_1:
                a.item1 = TILE_FIRE_1;
                a.item2 = TILE_FIRE_2;
                break;
            case TILE_BLUE_FIRE_1:
                a.item1 = TILE_BLUE_FIRE_1;
                a.item2 = TILE_BLUE_FIRE_2;
                break;
            case TILE_WATER:
                break;
            default:
                continue;
            }
            
            Array_Append(&animationTile, &a);
        }
        
    }
}


void doAnimations() {
    uint8_t i;

    animationTimer++;
    animateWater();

    if((animationTimer % 15) != 0)
        return;
    
    
    foreach(animationTile, i) {
        animation_t *a = (animation_t*)Array_Get(&animationTile, i);
        tile_t tile = getTile(a->x, a->y);

        if(tile.id == TILE_WATER) {
            drawTile(tile, a->x, a->y);
        } else {
            tile.id = tile.id == a->item1 ? a->item2 : a->item1;
            setTile(a->x, a->y, &tile);
        }

    }
}


/* Stops a tile at (x, y) from animating
 * @param x X row of the tile
 * @param y Y column of the tile
 * @returns none */
void removeAnimation(const uint8_t x, const uint8_t y) {
    uint8_t i;

    foreach(animationTile, i)
    {
        animation_t *a = (animation_t*)Array_Get(&animationTile, i);

        if(a->x == x && a->y == y)
        {
            Array_RemoveAt(&animationTile, i);
            return;
        }
    }

}

/* draws a sprite and adds displacement from 'curLevel'
 * @param *sprite gfx_sprite_t to draw to the screen
 * @param x X coordinate of sprite in pixels
 * @param y Y coordinate of sprite in pixels
 * @returns none */
#define drawSpriteWithOffset(sprite, x, y) drawSprite(sprite, (unsigned int)(x) + curLevel->ox, (y) + curLevel->oy)
#define drawSpriteWithOffset_Transparent(sprite, x, y) drawSprite_Transparent(sprite, (unsigned int)(x) + curLevel->ox, (y) + curLevel->oy)


/* redraws a tile at a given (row, column)
 * @param tile tile_t to draw to the screen
 * @param row X coordinate
 * @param column Y coordinate
 * @returns none */
void drawTile(tile_t tile, uint8_t row, uint8_t column) {
    // const gfx_sprite_t *sprite = (tile.type == TYPE_FIRE && !tile.data.fire.isLit) ? (gfx_sprite_t*)floor_data : lookupTile(tile.id).data;
    const gfx_sprite_t *sprite = lookupTile(tile.id).data;
    
    const unsigned int x = row << SHIFTBY;
    const uint8_t y = column << SHIFTBY;

    // if is a pipe and is filled with water, fill blue
    if(IS_PIPE(tile) && tile.data.hasWater)
        drawFilledPipe(sprite, x, y);
    else
        drawSpriteWithOffset(sprite, x, y);
	
		
}


/* draws a sprite
 * @param *sprite gfx_sprite_t to draw to the screen
 * @param x X coordinate of sprite in pixels
 * @param y Y coordinate of sprite in pixels
 * @returns none */
void drawSprite(gfx_sprite_t *sprite, unsigned int x, uint8_t y) {
    gfx_ScaledSprite_NoClip(sprite, x, y, SCALEBY, SCALEBY);
}

/* draws a sprite
 * @param *sprite gfx_sprite_t to draw to the screen
 * @param x X coordinate of sprite in pixels
 * @param y Y coordinate of sprite in pixels
 * @returns none */
void drawSprite_Transparent(gfx_sprite_t *sprite, unsigned int x, uint8_t y) {
    gfx_ScaledTransparentSprite_NoClip(sprite, x, y, SCALEBY, SCALEBY);
}

/* Draws a string that is centered along the X-axis
 * @param *string pointer to the string
 * @param y Y coordinate
 * @returns none */
void centeredString(const char *string, uint8_t y) {
    const uint8_t pixelLen = gfx_GetStringWidth(string);

    gfx_PrintStringXY(string, SCREEN_WIDTH/2 - (pixelLen >> 1), y);
}

/* fills a pipe sprite with water and draws it onto the screen
 * @param *sprite source sprite pointer
 * @param row X coordinate of tile in rows/columns
 * @param column Y coordinate of tile in rows/columns
 * @returns none */
static void drawFilledPipe(const gfx_sprite_t *sprite, uint8_t x, uint8_t y) {
    uint8_t i;

    memcpy(pipeSpriteBuffer, sprite, 66);
    
    // search and replaced
    for(i = 2; i < 66; i++) {
        const uint8_t color = pipeSpriteBuffer[i];

        if(color == COLOR_GRAY)
            pipeSpriteBuffer[i] = gfx_RGBTo1555(0, 0, 255);
    }

    drawSpriteWithOffset((gfx_sprite_t*)pipeSpriteBuffer, x, y);
}


/* redraws a tile at a given coordinate
 * @param row X coordinate of tile in rows/columns
 * @param column Y coordinate of tile in rows/columns
 * @returns none */
void updateTile(uint8_t x, uint8_t y) 
{
    drawTile(getTile(x, y), x, y);
}

void erasePlayer() {
	const tile_t tile = getTile(player.x, player.y);
	drawTile(tile, player.x, player.y);
}


void drawPlayer() {
    Position pos = realPlayerPosition();

    

    if(player.isMoving)
    {
        if((animationTimer >> 2) & 1)
            playerS = (playerS == playerSpriteRun1) ? playerSpriteRun2 : playerSpriteRun1;
        drawSpriteWithOffset_Transparent(playerS, pos.x, pos.y);
    } else
    {
    	drawSpriteWithOffset_Transparent(playerSprite, pos.x, pos.y);
    }
}


/* returns the actual screen coords of the player
 * - normally stored as a tile location
 * @returns position struct */
Position realPlayerPosition() {
    Position pos = facingOffset(player.direction);
    pos.x = (player.x << SHIFTBY) + player.dx * (char)pos.x;
    pos.y = (player.y << SHIFTBY) + player.dy * (char)pos.y;

    return pos;
}