#ifndef WATER_H
#define WATER_H


typedef struct
{
    Position position;
    Direction spreadDirection;
} flow_t;


extern Array flows;
extern uint8_t waterSpriteBuffer[66];
static bool keepWaterTick;

void initFlows();
void drainWater();

bool createFlow(uint8_t x, uint8_t y, Direction dir);
void deleteFlow(flow_t *self);

bool doFlow(flow_t *);
void tickFlows();

void animateWater();

// this should move to engine.c
void rotateTile(uint8_t x, uint8_t y);

#endif