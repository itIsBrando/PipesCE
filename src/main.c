/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/

#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "util.h"
#include "images/mypalette.h"


struct player_t player;

// RLE = -4 bytes
const uint8_t lvl1[] = {
	1, 1,  1, 1, 1, 1, 4, 1,
	1, 10, 0, 0, 0, 2, 5, 1,
	1, 5,  0, 0, 0, 0, 5, 1,
	1, 0,  5, 0, 0, 5, 0, 1,
	1, 5,  0, 8, 0, 0, 5, 1,
	1, 3,  8, 0, 8, 8, 9, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

// RLE = 17 bytes
const uint8_t lvl2[] = {
	9, 1, 6, 0, 2, 1, 5, 0, 1, 2, 2, 1, 1, 10, 3, 0, 2, 8, 1, 12, 1, 1, 1, 0, 3, 8, 2, 0, 8, 1
};

// RLE = 7 bytes
const uint8_t lvl3[] = {
    9, 1, 2, 0, 1, 10, 2, 0, 1, 2, 2, 1, 4, 0, 1, 5, 1, 10, 1, 1, 1, 13, 2, 8, 1, 15, 2, 8, 1, 9, 8, 1
};

// RLE = 13 bytes
const uint8_t lvl4[] = {
        1, 1, 1, 4, 7, 1, 1, 5, 1, 2, 4, 0, 2, 1, 1, 5, 5, 0, 2, 1, 1, 5, 5, 0, 2, 1, 1, 5, 7, 1, 
1, 5, 3, 0, 1, 10, 1, 0, 2, 1, 1, 5, 2, 0, 2, 5, 1, 0, 2, 1, 1, 5, 5, 0, 2, 1, 1, 3, 3, 8, 1, 9, 1, 0, 8, 1
};

// RLE = 0 bytes (not compressed)
const uint8_t lvl5[] = {
	1,  1,  4,  1,  1,  1,  1,
	1,  3,  5,  0,  0,  0,  1,
	1,  1,  1,  8,  0,  0,  1,
	1,  6,  9,  5,  0,  0,  1,
	1,  3,  7,  0,  0,  0,  1,
	1,  0, 10,  0,  0,  2,  1,
	1,  1,  1,  1,  1,  1,  1,
};

// RLE = 21 bytes
const uint8_t lvl6[] = {
	1, 1, 1, 4, 9, 1, 1, 5, 2, 0, 1, 2, 4, 0, 2, 1, 1, 5, 6, 1, 1, 15, 2, 1, 1, 3, 3, 8, 1, 16, 3, 0, 2, 1, 4, 0, 1, 3, 3, 0, 2, 1, 1, 5, 1, 7, 6, 0, 2, 1, 1, 10, 1, 9, 6, 0, 10, 1
};

// RLE = 17 bytes
const uint8_t lvl7[] = {
	1, 1, 1, 4, 9, 1, 1, 5, 6, 0, 1, 2, 2, 1, 1, 3, 1, 8, 3, 16, 3, 0, 2, 1, 1, 0, 1, 3, 3, 15, 3, 0, 2, 1, 1, 0, 1, 7, 6, 0, 10, 1
};

// RLE = 9 bytes
const uint8_t lvl8[] = {
	3, 1, 1, 4, 7, 1, 2, 0, 1, 5, 4, 0, 1, 2, 2, 1, 1, 16, 1, 1, 1, 15, 1, 8, 1, 7, 3, 0, 2, 1, 2, 0, 1, 5, 1, 6, 1, 9, 3, 0, 2, 1, 2, 0, 2, 5, 4, 0, 2, 1, 2, 0, 1, 16, 1, 5, 1, 16, 1, 7, 2, 0, 2, 1, 3, 0, 1, 3, 1, 8, 1, 9, 2, 0, 2, 1, 8, 0, 10, 1
};

// RLE = 15 bytes
const uint8_t lvl9[] = {
	11, 1, 1, 2, 7, 0, 2, 1, 3, 0, 2, 8, 1, 7, 2, 0, 2, 1, 2, 0, 1, 15, 1, 8, 1, 7, 1, 16, 2, 0, 2, 1, 2, 0, 1, 5, 1, 1, 2, 5, 2, 0, 2, 1, 2, 0, 1, 5, 1, 13, 1, 9, 1, 5, 2, 0, 2, 1, 2, 0, 1, 5, 1, 1, 1, 16, 1, 5, 2, 0, 2, 1, 1, 0, 1, 16, 1, 3, 2, 8, 1, 9, 2, 0, 2, 1, 8, 0, 2, 1, 8, 0, 10, 1
};


static const struct Map_t worldMaps[] = {
	// 1, 2, 3, 4, 5
	{"FIRST PUSHES", 1, 1, 0, 2, 0, 0}, {"PULL IT", 4, 1, 1, 3, 0, 0}, {"MORE FIRE", 7, 1, 2, 4, 0, 0}, {"LONG PIPE", 10, 1, 3, 5, 0, 0}, {"SNAKE PIPE", 12, 1, 4, 0, 0, 6},
	// 6, 7
	{"BLUE FIRE", 12, 4, 7, 0, 5, 8}, {"BLUE FIRE 3x", 9, 4, 0, 6, 0, 9},
	// 8, 9, 10, 11
	{"ROUND THE CORNER", 12, 7, 9, 0, 6, 0}, {"SPIRAL", 9, 7, 10, 8, 7, 12}, {"ROTATION", 6, 7, 11, 9, 0, 0}, {"OUT OF THE BOX", 3, 7, 0, 10, 0, 0},
	// 12
	{"SNAKE PIPE", 7, 10, 0, 0, 9, 0}
};

level_t *curLevel;
static uint8_t levelNumber = 0;

static level_t lev[9];

#define SIZEOF_LEV (sizeof(lev) / sizeof(lev[0]))
#define SIZEOF_POSITIONS (sizeof(worldMaps) / sizeof(worldMaps[0]))

// RLE = 8 bytes
static const uint8_t WorldMap[] = {
	/* 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	1, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  8, 10,  1,
	1,  0,  0,  0,  0,  0, 20,  5,  0,  0,  0,  0,  5,  1,
	1,  0,  0,  0,  0,  0,  0,  5,  0,  0, 20,  0,  5,  1,
	1, 10,  8,  8, 10,  7, 20, 10, 20, 10,  8,  8, 10,  1,
	1,  5, 18, 18, 18,  5, 20, 20, 20,  5, 18, 18,  5,  1,
	1,  5, 18, 18, 18,  3,  7,  0,  0,  5, 18, 18,  5,  1,
	1, 10, 18, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  1,
	1,  5, 18,  5,  0,  0,  0,  0,  6, 9,   0, 20,  0,  1,
	1,  5, 18,  3,  7,  0, 20,  6,  9,  0,  0,  0,  0,  1,
	1, 10,  8,  8, 10,  8,  8, 10,  8,  8, 10,  8, 10,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  7,
	5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5
	3,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9,
*/
	15, 1, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 1, 8, 1, 10, 2, 1, 5, 0, 1, 20, 1, 5, 4, 0, 1, 5, 2, 1, 6, 0, 1, 5, 2, 0, 1, 20, 1, 0, 1, 5, 2, 1, 1, 10, 2, 8, 1, 10, 1, 7, 1, 20, 1, 10, 1, 20, 1, 10, 2, 8, 1, 10, 2, 1, 1, 5, 3, 18, 1, 5, 3, 20, 1, 5, 2, 18, 1, 5, 2, 1, 1, 5, 3, 18, 1, 3, 1, 7, 2, 0, 1, 5, 2, 18, 1, 5, 2, 1, 1, 10, 1, 18, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 2, 1, 1, 5, 1, 18, 1, 5, 4, 0, 1, 6, 1, 9, 1, 0, 1, 20, 1, 0, 2, 1, 1, 5, 1, 18, 1, 3, 1, 7, 1, 0, 1, 20, 1, 6, 1, 9, 4, 0, 2, 1, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 2, 8, 1, 10, 1, 8, 1, 10, 15, 1, 1, 6, 12, 8, 1, 7, 1, 5, 12, 0, 1, 5, 1, 3, 12, 8, 1, 9
};

void main(void) {
	uint8_t kGroup6;

	/* 
	uint8_t i;
	Array array; */

	*(char*)0xFD0000 = 1;
	
	/* Array_New(&array, 10, sizeof(uint8_t));

	for(i = 0; i < Array_Len(&array); i++) {
		uint8_t *ii;
		uint8_t write = ((i+1)<<2);
		Array_Set(&array, &write, i);
		ii = (uint8_t*)Array_Get(&array, i);
		dbg_sprintf(dbgerr, "before %d: %d\n", i, *ii);
	}

	//((flow_t*)Array_Get(&array, 2))->position.x = 23;
	Array_RemoveAt(&array, 1);
	
	for(i = 0; i < Array_Len(&array); i++) {
		uint8_t *ii = Array_Get(&array, i);
		dbg_sprintf(dbgerr, "after %d: %d\n", i, *ii);
	}

	Array_Destroy(&array);
 */
    Array_New(&flows, 30, sizeof(flow_t));
	Array_New(&animationTile, 40, sizeof(animation_t));
	memset(&player, 0, sizeof(struct player_t));
	
	// initialize graphics
	gfx_Begin();
	gfx_SetPalette(mypalette, sizeof_mypalette, 0);
	gfx_SetTransparentColor(COLOR_TRANSPARENT);

	loadData();

	waterSpriteBuffer[0] = waterSpriteBuffer[1] = 8;
	
	createMap(&lev[0], 4, 1, 8, 7, lvl1, false);
	createMap(&lev[1], 1, 1, 8, 6, lvl2, (bool)sizeof(lvl2));
	createMap(&lev[2], 1, 2, 8, 5, lvl3, (bool)(sizeof(lvl3)));
	createMap(&lev[3], 2, 3, 8, 10, lvl4, (bool)(sizeof(lvl4)));
	createMap(&lev[4], 4, 3, 7, 7, lvl5, false);
	createMap(&lev[5], 3, 1, 10, 8, lvl6, (bool)(sizeof(lvl6)));
	createMap(&lev[6], 3, 1, 10, 6, lvl7, (bool)(sizeof(lvl7)));
	createMap(&lev[7], 3, 1, 10, 9, lvl8, (bool)(sizeof(lvl8)));
	createMap(&lev[8], 1, 1, 10, 11, lvl9, (bool)(sizeof(lvl9)));
	
	// title screen
	
	gfx_SetDraw(gfx_buffer);

	gfx_FillScreen(COLOR_YELLOW);

	levelNumber = showWorldMap();

	loadMap(&lev[levelNumber]);

	do {
		kb_Scan();
		kGroup6 = kb_Data[6];

		if(kGroup6 == kb_Clear) {
			levelNumber = showWorldMap();
			loadMap(&lev[levelNumber]);
		}
		
		erasePlayer();

		if(kb_IsDown(kb_Key2nd)) {
			player.isPulling = true;
		} else {
			player.isPulling = false;
		}
			
		tickFlows();
		doAnimations();


		// pass d-pad buttons
		move(kb_Data[7]);
		
		updatePlayer();
		drawPlayer();

		gfx_BlitBuffer();
	} while(true);
}


/* Frees up all memory and exits the program
 * @returns does not return. */
void cleanUp() {
	uint8_t i;

	saveData();

	gfx_End();

	// free each level
	for(i = 0; i < SIZEOF_LEV; i++) {
		freeLevel(&lev[i]);
	}

	Array_Destroy(&flows);
	Array_Destroy(&animationTile);
	exit(0);
}


void move(uint8_t key) {
	Position pos;
	
	if(player.isMoving)
		return;
		
	if(key & kb_Right)
	{
		player.direction = RIGHT;
	}
	else if(key & kb_Left)
	{
		player.direction = LEFT;
	}
	else if(key & kb_Up)
	{
		player.direction = UP;
	}
	else if(key & kb_Down)
	{
		player.direction = DOWN;
	}
	else
	{
		return;
	}

	pos = facingOffset(player.direction);
	movePlayer(pos.x, pos.y);
}


// called by `completeLevel()`.
// Loads the next level
void nextLevel() {
	setLevelCompletion(levelNumber);

	if(++levelNumber >= SIZEOF_LEV)
		levelNumber = 0;

	loadMap(&lev[levelNumber]);
}


void drawMapTitle(struct Map_t *m) {
	gfx_SetColor(COLOR_YELLOW);
	gfx_FillRectangle_NoClip((SCREEN_WIDTH - 8 * 17) / 2, SCREEN_HEIGHT - 24, 17 * 8, 8);
	centeredString(m->title, SCREEN_HEIGHT - 24);
}


/* Moves the player on the world map.
 * The player's position may not change
 * @param **m pointer to the memory location of a Map_t pointer
 * @param lvl level number to set
 * @returns none. */
static void movePlayerWorldMap(struct Map_t **m, uint8_t lvl) {
	// id = 0 means cannot move
	if(!lvl)
		return;

	*m = &worldMaps[lvl-1];

	drawMapTitle(*m);
	player.x = (*m)->x;	
	player.y = (*m)->y;

	(*m)->id = lvl-1;

}

/* Presents the world map.
 * @returns level number that was chosen. */
uint8_t showWorldMap() {
	uint8_t key;
	level_t map;
	uint8_t level = 0;
	struct Map_t *curPosition;

	createMap(&map, 1, 1, 14, 15, WorldMap, sizeof(WorldMap));
	loadMap(&map);
	
	gfx_SetTextBGColor(30);
	gfx_SetTextTransparentColor(30);

	gfx_SetTextFGColor(COLOR_BLACK);

	// if a level has been completed, then set a star tile
	for(key = 0; key < SIZEOF_LEV; key++)
	{
		const struct Map_t m = worldMaps[key];

		if(getLevelCompletion(key))
		{
			tile_t t;
			t.id = TILE_STAR;

			setTile(m.x, m.y, &t);
			removeAnimation(m.x, m.y);
		}

	}

	if(getLevelCompletion(0))
		while(getLevelCompletion(++level));
	
	curPosition = &worldMaps[level];
	curPosition->id = level;
	player.x = curPosition->x;
	player.y = curPosition->y;
	drawMapTitle(curPosition);

	while((key = os_GetCSC()) != sk_2nd && key != sk_Enter)
	{
		erasePlayer();
		
		switch(key)
		{
		case sk_Right:
			movePlayerWorldMap(&curPosition, curPosition->right);
			break;
		case sk_Left:
			movePlayerWorldMap(&curPosition, curPosition->left);
			break;
		case sk_Down:
			movePlayerWorldMap(&curPosition, curPosition->down);
			break;
		case sk_Up:
			movePlayerWorldMap(&curPosition, curPosition->up);
			break;
		case sk_Clear:
			cleanUp();
		}

		doAnimations();
		drawPlayer();

		gfx_BlitBuffer();
	}

	freeLevel(&map);
	return curPosition->id;
}