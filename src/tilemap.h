#ifndef TILEMAP_H
#define TILEMAP_H


#define PIPES_SIZE 11
#define TOTAL_LEVELS 20

typedef struct {
	uint8_t directions; // directions that the water can flow
	uint8_t id;
} pipe_t;

extern struct pipe_t;
extern pipe_t pipes[];

extern tile_t *levelPointers[TOTAL_LEVELS];

void loadMap(mapstore_t, uint8_t);
void drawLevel();
void completeLevel();

void rotateTile(uint8_t x, uint8_t y);

#endif