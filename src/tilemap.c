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


void createMap(level_t *level, const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, uint8_t *data, const bool isRLE) {
	unsigned int i;
	const unsigned int area = (unsigned int)width * (unsigned int)height;

	level->width = width;
	level->height = height;
	level->px = x;
	level->py = y;
	level->ox = (SCREEN_WIDTH/2) - width * (8*SCALEBY/2);
	level->oy = (SCREEN_HEIGHT/2) - height * (4*SCALEBY);
	level->data = malloc(area * sizeof(tile_t));

	// if compressed, then we will expand it
	if(isRLE)
	{
		uint8_t *copy = data;
		data = malloc(isRLE);
		rleDecompress(copy, &data, isRLE);
	}

	assert(level->data);

	for(i = 0; i < area; i++)
	{
		tile_t *t = &level->data[i];
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
	if(isRLE)
	{
		free(data);
	}

	dbg_sprintf(dbgerr, "size: (%d, %d)\n", level->width, level->height);
}


void freeLevel(level_t *l) {
	uint8_t i;
	
	if(l->data)
		free(l->data);
}


void drawLevel(level_t level) {
	uint8_t x, y;
	
	gfx_FillScreen(COLOR_YELLOW);
	for(y = 0; y < level.height; y++)
	{
		for(x = 0; x < level.width; x++)
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
void loadMap(level_t *level) {
	uint8_t x, y;

	curLevel = level;
	drawLevel(*level);
	player.x = level->px;
	player.y = level->py;
	initFire();
	initAnimations();
	Array_Clear(&flows);
}


void completeLevel() {
	uint8_t key;
	
	drawLevel(*curLevel);

	centeredString("Level Complete!", SCREEN_HEIGHT - 8);

	drawPlayer();
	// add fireworks later??
	do {
		doAnimations();
		gfx_BlitBuffer();
	} while((key = os_GetCSC()) != sk_2nd);

	
	nextLevel();
}