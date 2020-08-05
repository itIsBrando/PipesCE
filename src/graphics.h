#ifndef GRAPHICS_H
#define GRAPHICS_H

#define IS_PIPE(t) t.type == TYPE_PIPE
#define SCALEBY 2
#define SHIFTBY 4

//#define drawSprite(sprite, x, y) gfx_ScaledSprite_NoClip(sprite, x, y, SCALEBY, SCALEBY);




void updateTile(uint8_t x, uint8_t y);
void drawTile(tile_t tile, uint8_t x, uint8_t y);
void drawSprite(gfx_sprite_t *sprite, unsigned int x, uint8_t y);
void drawSprite_Transparent(gfx_sprite_t *sprite, unsigned int x, uint8_t y);
void centeredString(const char *string, uint8_t y);

static void drawFilledPipe(const gfx_sprite_t *sprite, uint8_t x, uint8_t y);

void doAnimations();
void initAnimations();
void removeAnimation(const uint8_t x, const uint8_t y);
extern Array animationTile;
extern uint8_t animationTimer;

void erasePlayer();
void drawPlayer(void);
Position realPlayerPosition();



#endif