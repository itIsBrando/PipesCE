#ifndef TILEMAP_H
#define TILEMAP_H


#define PIPES_SIZE 8

typedef struct {
	uint8_t directions; // directions that the water can flow
	uint8_t id;
} pipe_t;

extern struct pipe_t;
extern pipe_t pipes[];

void loadMap(level_t*);
void drawLevel(level_t);
void completeLevel();
void freeLevel(level_t *l);

void createMap(level_t *level, const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height, uint8_t *data, const bool isRLE);

#endif