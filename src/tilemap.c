#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "util.h"

tile_t *levelPointers[TOTAL_LEVELS];

// these are not required to be in a particular order
const pipe_t pipes[] = {
	{BIT_RIGHT | BIT_DOWN, TILE_PIPE_BOT_RIGHT},
	{BIT_LEFT | BIT_DOWN, TILE_PIPE_BOT_LEFT},
	{BIT_UP | BIT_RIGHT, TILE_PIPE_TOP_RIGHT},
	{BIT_LEFT | BIT_UP, TILE_PIPE_TOP_LEFT},
	{BIT_LEFT | BIT_RIGHT, TILE_PIPE_HORIZONTAL},
	{BIT_DOWN | BIT_UP, TILE_PIPE_VERTICAL},
	{BIT_DOWN, TILE_SPOUT_BOT},
	{BIT_UP, TILE_SPOUT_TOP},
	{BIT_LEFT, TILE_SPOUT_LEFT},
	{BIT_RIGHT, TILE_SPOUT_RIGHT},
	{BIT_LEFT | BIT_UP | BIT_DOWN | BIT_RIGHT, TILE_PIPE_ALL},
};

// index = level number + 1. index = 0 if map is the world map
void lvl_load(mapstore_t map, uint8_t index) {
	unsigned int i;
	uint8_t *data = map.data;
	const unsigned int area = (unsigned int)(map.width) * (unsigned int)(map.height);

	Array_Clear(&flows);
	Array_Clear(&animationTile);

	curLevel.width = map.width;
	curLevel.height = map.height;
	curLevel.px = map.sx;
	curLevel.py = map.sy;
	curLevel.ox = (SCREEN_WIDTH/2) - map.width * (8*SCALEBY/2);
	curLevel.oy = (SCREEN_HEIGHT/2) - map.height * (4*SCALEBY);

	// if we have allocated memory for the level, don't malloc again
	// instead use the pointer we kept track of
	if(index == 0 || levelPointers[index-1] == NULL)
	{
		curLevel.data = malloc(area * sizeof(tile_t));

		if(index)
			levelPointers[index-1] = curLevel.data;
	}
	else {
		curLevel.data = levelPointers[index-1];
	}
	

	dbg_sprintf(dbgerr, "benchmark 1: %p\n", curLevel.data);


	// if compressed, then we will expand it
	if(map.rleSize)
	{
		uint8_t *copy = map.data;
		dbg_sprintf(dbgerr, "benchmark 2\n");
		data = malloc(area);
		assert(data);
		utl_rleDecompress(copy, &data, map.rleSize);
	}

	assert(curLevel.data);

	for(i = 0; i < area; i++)
	{
		tile_t *t = &curLevel.data[i];
		tilesprite_t tiledata = lookupTile(data[i]);

		t->id = tiledata.id;
		t->flags = tiledata.flags;
		
		if(tiledata.id == TILE_FIRE_1 || tiledata.id == TILE_BLUE_FIRE_1)
		{
			fre_create(t);
		} else if (tiledata.id == TILE_ROTATION)
		{
			t->type = TYPE_ROTATE;
		} else
		{	
			t->data.hasWater = false;
			t->type = pipeFromIndex(tiledata.id) ? TYPE_PIPE : TYPE_BLOCK;
		}

	}

	// free alloted memory
	if(map.rleSize)
	{
		free(data);
	}

	lvl_draw();
	player.x = curLevel.px;
	player.y = curLevel.py;
	fre_init();
	scrn_initAnimations();
	Array_Clear(&flows);
}



/* Frees all loaded levels
 * @returns none. */
void lvl_free()
{
	uint8_t i;

	for(i = 0; i < sizeof(levelPointers); i++)
	{
		// if malloced, then free and set to null
		if(levelPointers[i])
		{
			free(levelPointers[i]);
			levelPointers[i] = NULL;
		}
	}
}

/* Draws the level in 'curLevel'
 * @returns none. */
void lvl_draw() {
	uint8_t x, y;
	
	gfx_FillScreen(COLOR_YELLOW);
	for(y = 0; y < curLevel.height; y++)
	{
		for(x = 0; x < curLevel.width; x++)
		{
			tile_t tile = getTile(x, y);
			scrn_drawTile(tile, x, y);
		}
	}

}



/* Safely rotates a pipe at (x, y).
 * @param X row
 * @param Y column
 * @returns none. */
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


/* Draws the UI that occurs after completing a level.
 * @returns none. */
void lvl_complete()
{
	uint8_t key;
	
	lvl_draw();

	scrn_centeredString("Level Complete!", SCREEN_HEIGHT - 8);

	scrn_drawPlayer();
	// add fireworks later??
	do {
		scrn_doAnimations();
		gfx_BlitBuffer();
	} while((key = os_GetCSC()) != sk_2nd);

	nextLevel();
}