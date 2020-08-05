#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "util.h"


// these are not required to be in a particular order
const pipe_t pipes[] = {
	{BIT_RIGHT | BIT_DOWN, TILE_PIPE_BOT_RIGHT},
	{BIT_LEFT | BIT_DOWN, TILE_PIPE_BOT_LEFT},
	{BIT_LEFT | BIT_RIGHT, TILE_PIPE_HORIZONTAL},
	{BIT_UP | BIT_RIGHT, TILE_PIPE_TOP_RIGHT},
	{BIT_DOWN | BIT_UP, TILE_PIPE_VERTICAL},
	{BIT_DOWN, TILE_SPOUT_BOT},
	{BIT_LEFT | BIT_UP, TILE_PIPE_TOP_LEFT},
	{BIT_LEFT | BIT_UP | BIT_DOWN | BIT_RIGHT, TILE_PIPE_ALL},
};


void loadMap(mapstore_t map) {
	unsigned int i;
	uint8_t *data = map.data;
	const unsigned int area = (unsigned int)map.width * (unsigned int)map.height;

	curLevel.width = map.width;
	curLevel.height = map.height;
	curLevel.px = map.sx;
	curLevel.py = map.sy;
	curLevel.ox = (SCREEN_WIDTH/2) - map.width * (8*SCALEBY/2);
	curLevel.oy = (SCREEN_HEIGHT/2) - map.height * (4*SCALEBY);
	curLevel.data = malloc(area * sizeof(tile_t));

	// if compressed, then we will expand it
	if(map.rleSize)
	{
		uint8_t *copy = map.data;
		data = malloc(area);
		assert(data);
		rleDecompress(copy, &data, map.rleSize);
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
			createFire(t);
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


	drawLevel();
	player.x = curLevel.px;
	player.y = curLevel.py;
	initFire();
	initAnimations();
	Array_Clear(&flows);
}


void freeLevel() {
	if(curLevel.data)
		free(curLevel.data);
}


void drawLevel() {
	uint8_t x, y;
	
	gfx_FillScreen(COLOR_YELLOW);
	for(y = 0; y < curLevel.height; y++)
	{
		for(x = 0; x < curLevel.width; x++)
		{
			tile_t tile = getTile(x, y);
			drawTile(tile, x, y);
		}
	}

}



/* Draws the map and initializes necessary variables.
 * Should call 'createMap' first
 * @param *level pointer to level_t to load
 * @returns none. */



/* Draws the UI that occurs after completing a level.
 * Should call 'createMap' first
 * @returns none. */
void completeLevel() {
	uint8_t key;
	
	drawLevel();

	centeredString("Level Complete!", SCREEN_HEIGHT - 8);

	drawPlayer();
	// add fireworks later??
	do {
		doAnimations();
		gfx_BlitBuffer();
	} while((key = os_GetCSC()) != sk_2nd);

	nextLevel();
}