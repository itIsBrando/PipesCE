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
#include "levels.h"


struct player_t player;
static level_t WorldMapLevel;

static const struct Map_t worldMaps[] = {
	// 1, 2, 3, 4, 5
	{"FIRST PUSHES", 1, 1, 0, 2, 0, 0}, {"PULL IT", 4, 1, 1, 3, 0, 0}, {"MORE FIRE", 7, 1, 2, 4, 0, 19}, {"LONG PIPE", 10, 1, 3, 5, 0, 0}, {"SNAKE PIPE", 12, 1, 4, 0, 0, 6},
	// 6, 7
	{"BLUE FIRE", 12, 4, 7, 0, 5, 8}, {"BLUE FIRE 3x", 9, 4, 0, 6, 0, 9},
	// 8, 9, 10, 11
	{"ROUND THE CORNER", 12, 7, 9, 0, 6, 0}, {"SPIRAL", 9, 7, 10, 8, 7, 12}, {"ROTATION", 6, 7, 11, 9, 14, 0}, {"OUT OF THE BOX", 3, 7, 0, 10, 0, 13},
	// 12, 13
	{"SNAKE PIPE", 7, 10, 13, 18, 9, 0}, {"MANY VALVES", 4, 10, 15, 12, 11, 0},
	// 14, 15
	{"FULL CIRCLE", 4, 4, 16, 10, 0, 10}, {"TIGHT SPACE", 1, 10, 0, 13, 17, 0},
	// 16, 17, 18
	{"FIRE WALL", 1, 4, 0, 14, 0, 17}, {"SYNC", 1, 7, 0, 0, 16, 15}, {"INFERNO", 10, 10, 12, 0, 0, 0, 0},
	// 19
	{"OPTIONAL LEVEL", 7, 4, 0, 0, 3, 0}
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
	{1, 3, 10, 8,lvl18,  sizeof(lvl18)}, // INFERNO
	{1, 2, 8, 6, lvl19,  sizeof(lvl19)}, // OPTIONAL
};


void main(void) {
	uint8_t kGroup6;

	// *(char*)0xFD0000 = 1;

	// initialize arrays
    Array_New(&flows, 30, sizeof(flow_t));
	Array_New(&animationTile, 40, sizeof(animation_t));
	memset(&levelPointers, 0, SIZEOF_LEVEL_POINTERS);
	
	// initialize graphics
	gfx_Begin();
	gfx_SetPalette(mypalette, sizeof_mypalette, 0);
	gfx_SetTransparentColor(COLOR_TRANSPARENT);

	// load save
	utl_load();
	
	// more graphics stuff
	*(uint24_t*)waterSpriteBuffer = 0x0808;
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
void nextLevel() {
	setLevelCompletion(levelNumber);

	levelNumber = showWorldMap();
}


void drawMapTitle(struct Map_t *m) {
	gfx_SetColor(COLOR_YELLOW);
	gfx_FillRectangle_NoClip((SCREEN_WIDTH - 8 * 17) / 2, SCREEN_HEIGHT - 24, 17 * 8, 8);
	scrn_centeredString(m->title, SCREEN_HEIGHT - 24);
}


static void setCompletionStars()
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

	curPosition = &worldMaps[level];
	curPosition->id = level;
	
	// initialize player position
	memset(&player, 0, sizeof(struct player_t));
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