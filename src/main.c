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
static level_t WorldMapLevel;

// RLE = 14 bytes. 25% reduction. (originally uncompressable)
const uint8_t lvl1[] = {
	6, 1, 132, 2, 1, 138, 3, 0, 130, 133, 2, 1, 133, 4, 0, 133, 2, 1, 128, 133, 2, 0, 133, 128, 2, 1, 133, 128, 136, 2, 0, 133, 2, 1, 131, 136, 128, 2, 8, 137, 9, 1
};

// RLE = 22 bytes. 48% reduction.
const uint8_t lvl2[] = {
	9, 1, 6, 0, 2, 1, 5, 0, 130, 2, 1, 138, 3, 0, 2, 8, 140, 129, 128, 3, 8, 2, 0, 8, 1
};

// RLE = 15 bytes. 40% reduction.
const uint8_t lvl3[] = {
	9, 1, 2, 0, 138, 2, 0, 130, 2, 1, 4, 0, 133, 138, 129, 141, 2, 8, 143, 2, 8, 137, 8, 1
};

// RLE = 29 bytes. 36% reduction.
const uint8_t lvl4[] = {
	129, 132, 7, 1, 133, 130, 4, 0, 2, 1, 133, 5, 0, 2, 1, 133, 5, 0, 2, 1, 133, 7, 1, 133, 3, 0, 138, 128, 2, 1, 133, 2, 0, 2, 5, 128, 2, 1, 133, 5, 0, 2, 1, 131, 3, 8, 137, 128, 8, 1        
};

// RLE = 13 bytes. 28% reduction. (originally uncompressable)
const uint8_t lvl5[] = {/* 
	1,  1,  4,  1,  1,  1,  1,
	1,  3,  5,  0,  0,  0,  1,
	1,  1,  1,  8,  0,  0,  1,
	1,  6,  9,  5,  0,  0,  1,
	1,  3,  7,  0,  0,  0,  1,
	1,  0, 10,  0,  0,  2,  1,
	1,  1,  1,  1,  1,  1,  1, */
	2, 1, 132, 5, 1, 131, 133, 3, 0, 4, 1, 136, 2, 0, 2, 1, 134, 137, 133, 2, 0, 2, 1, 131, 135, 3, 0, 2, 1, 128, 138, 2, 0, 130, 8, 1
};

// RLE = 34 bytes. 44% reduction
const uint8_t lvl6[] = {
	129, 132, 9, 1, 133, 2, 0, 130, 4, 0, 2, 1, 133, 6, 1, 143, 2, 1, 131, 3, 8, 144, 3, 0, 2, 1, 4, 0, 131, 3, 0, 2, 1, 133, 135, 6, 0, 2, 1, 138, 137, 6, 0, 10, 1
};

// RLE = 27 bytes. 47% reduction
const uint8_t lvl7[] = {
	129, 132, 9, 1, 133, 6, 0, 130, 2, 1, 131, 136, 3, 16, 3, 0, 2, 1, 128, 131, 3, 15, 3, 0, 2, 1, 128, 135, 6, 0, 10, 1
};

// RLE = 27 bytes. 31% reduction
const uint8_t lvl8[] = {
	3, 1, 132, 7, 1, 2, 0, 133, 4, 0, 130, 2, 1, 144, 129, 143, 136, 135, 3, 0, 2, 1, 2, 0, 133, 134, 137, 3, 0, 2, 1, 2, 0, 2, 5, 4, 0, 2, 1, 2, 0, 144, 133, 144, 135, 2, 0, 2, 1, 3, 0, 131, 136, 137, 2, 0, 2, 1, 8, 0, 10, 1
};

// RLE = 35 bytes. 33% reduction
const uint8_t lvl9[] = {
	11, 1, 130, 7, 0, 2, 1, 3, 0, 2, 8, 135, 2, 0, 2, 1, 2, 0, 143, 136, 135, 144, 2, 0, 2, 1, 2, 0, 133, 129, 2, 5, 2, 0, 2, 1, 2, 0, 133, 141, 137, 133, 2, 0, 2, 1, 2, 0, 133, 129, 144, 133, 2, 0, 2, 1, 128, 144, 131, 2, 8, 137, 2, 0, 2, 1, 8, 0, 2, 1, 8, 0, 10, 1
};

// RLE = 16 bytes. 34% reduction
const uint8_t lvl10[] = {
	129, 132, 6, 1, 131, 133, 2, 8, 138, 2, 1, 5, 0, 2, 1, 128, 149, 3, 0, 2, 1, 130, 4, 0, 8, 1
};

// RLE = 52 bytes!! 52% reduction
const uint8_t lvl11[] = {
	24, 0, 2, 1, 141, 135, 3, 1, 2, 0, 2, 5, 2, 1, 128, 134, 143, 135, 2, 1, 128, 131, 136, 137, 2, 1, 4, 0, 2, 1, 130, 128, 138, 128, 7, 1, 24, 0
};

// RLE = 25 bytes. 35% reduction
const uint8_t lvl12[] = {
	10, 1, 134, 135, 132, 144, 135, 2, 0, 2, 1, 143, 131, 137, 129, 131, 135, 128, 2, 1, 131, 135, 134, 135, 134, 137, 128, 2, 1, 144, 131, 137, 131, 137, 2, 0, 2, 1, 7, 0, 2, 1, 6, 0, 130, 10, 1
};

// RLE = 26 bytes. 43% reduction
const uint8_t lvl13[] = {
	129, 132, 8, 1, 6, 0, 138, 2, 1, 128, 130, 128, 130, 128, 130, 128, 2, 1, 128, 131, 5, 8, 2, 1, 128, 130, 128, 130, 128, 130, 128, 2, 1, 7, 0, 10, 1
};

// RLE = 24 bytes. 39% reduction
const uint8_t lvl14[] = {
	10, 1, 130, 2, 0, 138, 3, 0, 2, 1, 2, 0, 4, 8, 128, 2, 1, 128, 138, 128, 149, 128, 138, 128, 2, 1, 128, 137, 5, 0, 2, 1, 3, 0, 142, 3, 0, 10, 1
};

// RLE = 23 bytes. 34% reduction
const uint8_t lvl15[] = {
	4, 1, 132, 6, 1, 128, 134, 136, 143, 136, 135, 2, 0, 2, 1, 130, 136, 128, 133, 149, 133, 2, 0, 2, 1, 128, 133, 129, 133, 129, 133, 128, 3, 1, 128, 131, 3, 16, 137, 2, 0, 2, 1, 8, 0, 11, 1
};

// RLE = 24 bytes. 34% reduction
const uint8_t lvl16[] = {
	10, 1, 141, 135, 138, 2, 8, 128, 135, 2, 1, 2, 0, 144, 136, 128, 144, 128, 2, 1, 130, 128, 138, 136, 128, 143, 128, 2, 1, 128, 131, 138, 6, 1, 4, 0, 133, 2, 0, 2, 1, 4, 0, 133, 2, 0, 10, 1
};

// RLE = 39 bytes. 49% reduction
const uint8_t lvl17[] = {
	10, 1, 7, 0, 2, 1, 3, 0, 130, 136, 2, 5, 2, 1, 128, 4, 1, 2, 0, 2, 1, 7, 0, 2, 1, 7, 0, 2, 1, 7, 0, 2, 1, 141, 149, 136, 149, 136, 149, 138, 10, 1
};

static const struct Map_t worldMaps[] = {
	// 1, 2, 3, 4, 5
	{"FIRST PUSHES", 1, 1, 0, 2, 0, 0}, {"PULL IT", 4, 1, 1, 3, 0, 0}, {"MORE FIRE", 7, 1, 2, 4, 0, 0}, {"LONG PIPE", 10, 1, 3, 5, 0, 0}, {"SNAKE PIPE", 12, 1, 4, 0, 0, 6},
	// 6, 7
	{"BLUE FIRE", 12, 4, 7, 0, 5, 8}, {"BLUE FIRE 3x", 9, 4, 0, 6, 0, 9},
	// 8, 9, 10, 11
	{"ROUND THE CORNER", 12, 7, 9, 0, 6, 0}, {"SPIRAL", 9, 7, 10, 8, 7, 12}, {"ROTATION", 6, 7, 11, 9, 14, 0}, {"OUT OF THE BOX", 3, 7, 0, 10, 0, 13},
	// 12, 13
	{"SNAKE PIPE", 7, 10, 13, 0, 9, 0}, {"MANY VALVES", 4, 10, 15, 12, 11, 0},
	// 14, 15
	{"FULL CIRCLE", 4, 4, 16, 10, 0, 10}, {"TIGHT SPACE", 1, 10, 0, 13, 17, 0},
	// 16, 17
	{"FIRE WALL", 1, 4, 0, 14, 0, 17}, {"SYNC", 1, 7, 0, 0, 16, 15}
};

level_t curLevel;
static uint8_t levelNumber = 0;

// RLE = 71 bytes. 34% reduction
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
	15, 1, 138, 2, 8, 138, 2, 8, 138, 2, 8, 138, 136, 138, 2, 1, 5, 0, 148, 133, 4, 0, 133, 2, 1, 6, 0, 133, 2, 0, 148, 128, 133, 2, 1, 138, 2, 8, 138, 135, 148, 138, 148, 138, 2, 8, 138, 2, 1, 133, 3, 18, 133, 3, 20, 133, 2, 18, 133, 2, 1, 133, 3, 18, 131, 135, 2, 0, 133, 2, 18, 133, 2, 1, 138, 146, 138, 2, 8, 138, 2, 8, 138, 2, 8, 138, 2, 1, 133, 146, 133, 4, 0, 134, 137, 128, 148, 128, 2, 1, 133, 146, 131, 135, 128, 148, 134, 137, 4, 0, 2, 1, 138, 2, 8, 138, 2, 8, 138, 2, 8, 138, 136, 138, 15, 1, 134, 12, 8, 135, 133, 12, 0, 133, 131, 12, 8, 1, 9
};

const mapstore_t WorldMapData = {
	1, 1, 14, 15, WorldMap, sizeof(WorldMap)
};

mapstore_t maps[] = {
	{4, 1, 8, 7, lvl1,   sizeof(lvl1)},
	{1, 1, 8, 6, lvl2,   sizeof(lvl2)},
	{1, 2, 8, 5, lvl3,   sizeof(lvl3)},
	{2, 3, 8, 10, lvl4,  sizeof(lvl4)},
 	{4, 3, 7, 7, lvl5,   sizeof(lvl5)},
 	{3, 1, 10, 8, lvl6,  sizeof(lvl6)},
 	{3, 1, 10, 6, lvl7,  sizeof(lvl7)},
 	{4, 1, 10, 9, lvl8,  sizeof(lvl8)},
 	{1, 1, 10, 11, lvl9, sizeof(lvl9)},
 	{1, 3, 7, 6, lvl10,  sizeof(lvl10)}, // this is the 'rotation level'
 	{2, 5, 6, 15, lvl11, sizeof(lvl11)}, // out of the box
 	{7, 1, 9, 8, lvl12,  sizeof(lvl12)}, // snake pipe
 	{2, 1, 9, 7, lvl13,  sizeof(lvl13)}, // many valves
	{1, 1, 9, 7, lvl14,  sizeof(lvl14)}, // Full Circle
	{1, 2, 10, 7, lvl15, sizeof(lvl15)}, // Tight Space
	{1, 2, 9, 8, lvl16,  sizeof(lvl16)}, // FIRE WALL
	{1, 1, 9, 9, lvl17,  sizeof(lvl17)}, // SYNC
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
	memset(&levelPointers, 0, sizeof(levelPointers));
	
	// initialize graphics
	gfx_Begin();
	gfx_SetPalette(mypalette, sizeof_mypalette, 0);
	gfx_SetTransparentColor(COLOR_TRANSPARENT);

	utl_load();

	waterSpriteBuffer[0] = waterSpriteBuffer[1] = 8;
	
	gfx_SetDraw(gfx_buffer);
	gfx_FillScreen(COLOR_YELLOW);

	// keep the world map loaded the the entire runtime
	lvl_load(WorldMapData, 0);
	memcpy(&WorldMapLevel, &curLevel, sizeof(level_t));

	// title screen
	levelNumber = showWorldMap();

	do {
		kb_Scan();
		kGroup6 = kb_Data[6];

		if(kGroup6 == kb_Clear) {
			levelNumber = showWorldMap();
		}
		
		scrn_erasePlayer();

		if(kb_IsDown(kb_Key2nd)) {
			player.isPulling = true;
		} else {
			player.isPulling = false;
		}
		
		wtr_tickFlows();
		scrn_doAnimations();


		// pass d-pad buttons
		move(kb_Data[7]);
		
		updatePlayer();
		scrn_drawPlayer();

		gfx_BlitBuffer();
	} while(true);

}




/* Frees up all memory and exits the program
 * @returns does not return. */
void cleanUp() {
	
	// save the file and close it
	utl_save();

	gfx_End();

	// free levels
	lvl_free();

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


// called by `lvl_complete()`.
// Loads the next level
// calling 'freeLevel' crashes the game
void nextLevel() {
	setLevelCompletion(levelNumber);
	// freeLevel();	

	levelNumber = showWorldMap();
}


void drawMapTitle(struct Map_t *m) {
	gfx_SetColor(COLOR_YELLOW);
	gfx_FillRectangle_NoClip((SCREEN_WIDTH - 8 * 17) / 2, SCREEN_HEIGHT - 24, 17 * 8, 8);
	scrn_centeredString(m->title, SCREEN_HEIGHT - 24);
}


void setCompletionStars()
{
	uint8_t i;
	// if a level has been completed, then set a star tile
	for(i = 0; i < SIZEOF_MAPS; i++)
	{
		const struct Map_t m = worldMaps[i];

		if(getLevelCompletion(i))
		{
			const tile_t t = {{0}, 0, 0, TILE_STAR};

			setTile(m.x, m.y, &t);
			scrn_removeAnimation(m.x, m.y);
		}

	}
}


/* Moves the player on the world map.
 * The player's position may not change
 * @param **m pointer to the memory location of a Map_t pointer
 * @param lvl level number to set
 * @returns none. */
static void movePlayerWorldMap(struct Map_t **m, uint8_t lvl)
{
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
	uint8_t level = getLastLevelPlayed();
	struct Map_t *curPosition;

	// curLevel = WorldMapLevel;
	memcpy(&curLevel, &WorldMapLevel, sizeof(level_t));

	lvl_draw();
	fre_init();
	scrn_initAnimations();
	Array_Clear(&flows);
	
	gfx_SetTextBGColor(30);
	gfx_SetTextTransparentColor(30);

	gfx_SetTextFGColor(COLOR_BLACK);

	// draws the stars over levels that are completed
	setCompletionStars();

	memset(&player, 0, sizeof(struct player_t));
	
	// initialize player position
	curPosition = &worldMaps[level];
	curPosition->id = level;
	player.x = curPosition->x;
	player.y = curPosition->y;
	drawMapTitle(curPosition);

	while((key = os_GetCSC()) != sk_2nd && key != sk_Enter)
	{
		scrn_erasePlayer();
		
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

		scrn_doAnimations();
		scrn_drawPlayer();

		gfx_BlitBuffer();
	}

	key = curPosition->id;
	lvl_load(maps[key], key + 1);
	
	setLastLevelPlayed(key);

	return key;
}