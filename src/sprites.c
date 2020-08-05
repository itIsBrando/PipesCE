#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "images/gfx.h"
#include "images/mypalette.h"


tilesprite_t tileset[TILE_NUM] = {
	{TILE_FLOOR,			(gfx_sprite_t*)floor_data, 0x00},
	{TILE_BLOCK, 			(gfx_sprite_t*)block_data, 0x01},
	{TILE_HANDLE, 			(gfx_sprite_t*)handle_data, 0x00},
	{TILE_PIPE_TOP_RIGHT,	(gfx_sprite_t*)pipe_top_right_data, 0x03},
	{TILE_SPOUT_BOT,		(gfx_sprite_t*)spout_bot_data, FLG_SOLID | FLG_SPOUT},
	{TILE_PIPE_VERTICAL,	(gfx_sprite_t*)pipe_vertical_data, 0x03},
	{TILE_PIPE_BOT_RIGHT, 	(gfx_sprite_t*)pipe_bot_right_data, 0x03},
	{TILE_PIPE_BOT_LEFT, 	(gfx_sprite_t*)pipe_bot_left_data, 0x03},
	{TILE_PIPE_HORIZONTAL,	(gfx_sprite_t*)pipe_horizontal_data, 0x03},
	{TILE_PIPE_TOP_LEFT,	(gfx_sprite_t*)pipe_top_left_data, 0x03},
	{TILE_FIRE_1,			(gfx_sprite_t*)fire_1_data, FLG_SOLID},
	{TILE_FIRE_2,			(gfx_sprite_t*)fire_2_data, FLG_SOLID},
	{TILE_SPOUT_LEFT,		(gfx_sprite_t*)spout_left_data, FLG_SOLID | FLG_SPOUT},
	{TILE_SPOUT_RIGHT,		(gfx_sprite_t*)spout_right_data, FLG_SOLID | FLG_SPOUT},
	{TILE_SPOUT_TOP,		(gfx_sprite_t*)spout_top_data, FLG_SOLID | FLG_SPOUT},
	{TILE_PIPE_ALL,			(gfx_sprite_t*)pipe_all_data, 0x03},
	{TILE_BLUE_FIRE_1,		(gfx_sprite_t*)blue_fire_1_data, FLG_SOLID},
	{TILE_BLUE_FIRE_2,		(gfx_sprite_t*)blue_fire_2_data, FLG_SOLID},
	{TILE_WATER,			(gfx_sprite_t*)waterSpriteBuffer, 0},
	{TILE_STAR,				(gfx_sprite_t*)star_data, 0},
	{TILE_MOUNTAIN,			(gfx_sprite_t*)mountain_data, 0},
	{TILE_ROTATION,		(gfx_sprite_t*)rotator_data, 0}
};

// hosts a buffer to write a sprite temporarily
uint8_t pipeSpriteBuffer[8 * 8 + 2];