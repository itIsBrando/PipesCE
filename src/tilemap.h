#ifndef TILEMAP_H
#define TILEMAP_H


#define PIPES_SIZE 8

typedef struct {
	uint8_t directions; // directions that the water can flow
	uint8_t id;
} pipe_t;

extern struct pipe_t;
extern pipe_t pipes[];

void loadMap(mapstore_t);
void drawLevel();
void completeLevel();
void freeLevel();


#endif