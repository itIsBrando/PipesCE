#ifndef GRAPHICS_H
#define GRAPHICS_H

#define IS_PIPE(t) t.type == TYPE_PIPE
#define SCALEBY 2
#define SHIFTBY 4

//#define scrn_drawSprite(sprite, x, y) gfx_ScaledSprite_NoClip(sprite, x, y, SCALEBY, SCALEBY);




void updateTile(uint8_t x, uint8_t y);
void scrn_drawTile(tile_t tile, uint8_t x, uint8_t y);
void scrn_drawSprite(gfx_sprite_t *sprite, unsigned int x, uint8_t y);
void scrn_drawSprite_Transparent(gfx_sprite_t *sprite, unsigned int x, uint8_t y);
void scrn_centeredString(const char *string, uint8_t y);

static void scrn_drawFilledPipe(const gfx_sprite_t *sprite, uint8_t x, uint8_t y);

void scrn_doAnimations();
void scrn_initAnimations();
void scrn_removeAnimation(const uint8_t x, const uint8_t y);
extern Array animationTile;
extern uint8_t animationTimer;

void scrn_erasePlayer();
void scrn_drawPlayer(void);
static Position realPlayerPosition();



#endif