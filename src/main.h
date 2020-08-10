#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <keypadc.h>
#include <graphx.h>
#include <debug.h>
#include <assert.h>

#include "array.h"
#include "images/colors.h"


#define SCREEN_HEIGHT	240
#define SCREEN_WIDTH	320

#define SIZEOF_MAPS (sizeof(maps) / sizeof(maps[0]))
#define SIZEOF_POSITIONS (sizeof(worldMaps) / sizeof(worldMaps[0]))
#define SIZEOF_LEVEL_POINTERS (sizeof(levelPointers) / sizeof(levelPointers[0]))


typedef struct {
    uint8_t item1;
    uint8_t item2;
    uint8_t x, y;
} animation_t;


typedef struct {
    uint8_t x;
    uint8_t y;
    
} Position;


typedef enum {
    FLG_SOLID = 0x1,
    FLG_PUSHABLE = 0x2,
    FLG_SPOUT = 0x4
} flags_t;

typedef struct
{
    bool isLit;
    bool isPersistent; // if true, this fire will regrow after being watered down
} fire_t;


typedef enum
{
	TYPE_BLOCK,
	TYPE_PIPE,
	TYPE_FIRE,
	TYPE_ROTATE
} tiletype_t;

typedef union 
{
	bool hasWater;
	fire_t fire;
} data_t;

typedef struct
{
	data_t data;
	uint8_t flags;
	tiletype_t type;
	uint8_t id;
} tile_t;

typedef struct {
	// coords of the player's start position
	uint8_t px;
	uint8_t py;

	uint8_t ox, oy; // pixel offsets to add to the map to ensure its centered
	
	uint8_t width;
	uint8_t height;
	tile_t *data;
} level_t;


typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN
} Direction;

struct player_t {
	uint8_t x;
	uint8_t y;
	Direction direction;
	bool isMoving;
	bool isPulling;
	bool isWaterFlowing;
	uint8_t dx; // used for per-pixel movement
	uint8_t dy;
};

struct Map_t {
	char title[17];
	uint8_t x, y;
	uint8_t left, right, up, down;
	uint8_t id; // only set externally
};

typedef struct
{
	uint8_t sx, sy; // player start position
	uint8_t width, height;
	const uint8_t *data;
	uint8_t rleSize; // 0 if not compressed
} mapstore_t;


extern struct animation_t;
extern struct fire_t;
extern struct level_t;
extern struct mapstore_t;
extern struct tile_t;
extern struct Position;

extern struct player_t player;
extern level_t curLevel;
mapstore_t maps[];

void cleanUp();
void move(uint8_t key);
void nextLevel();
uint8_t showWorldMap();

#endif