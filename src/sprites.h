#ifndef SPRITES_H
#define SPRITES_H

//#define TILE_NUM 5
#define TILE_SIZE 8

enum {
	TILE_FLOOR=0,
	TILE_BLOCK,
	TILE_HANDLE,
	TILE_PIPE_TOP_RIGHT,
	TILE_SPOUT_BOT,
	TILE_PIPE_VERTICAL,
	TILE_PIPE_BOT_RIGHT,
	TILE_PIPE_BOT_LEFT,
	TILE_PIPE_HORIZONTAL,
	TILE_PIPE_TOP_LEFT,
	TILE_FIRE_1,
	TILE_FIRE_2,
	TILE_SPOUT_LEFT,
	TILE_SPOUT_RIGHT,
	TILE_SPOUT_TOP,
	TILE_PIPE_ALL,
	TILE_BLUE_FIRE_1,
	TILE_BLUE_FIRE_2,
	TILE_WATER,
	TILE_STAR,
	TILE_MOUNTAIN,
	TILE_ROTATION,
	TILE_NUM
};

#define BIT_UP (1<<UP)
#define BIT_DOWN (1<<DOWN)
#define BIT_LEFT (1<<LEFT)
#define BIT_RIGHT (1<<RIGHT)


#define playerSprite		((gfx_sprite_t*)character_still_data)
#define playerSpriteRun1	((gfx_sprite_t*)character_run_1_data)
#define playerSpriteRun2	((gfx_sprite_t*)character_run_2_data)

typedef struct {
	uint8_t id;
	gfx_sprite_t *data;
	flags_t flags;
} tilesprite_t;

extern struct tilesprite_t;
extern tilesprite_t tileset[TILE_NUM];

extern uint8_t pipeSpriteBuffer[];

#endif