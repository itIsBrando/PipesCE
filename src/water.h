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

void wtr_initFlows();
void wtr_drainWater();

void wtr_createFlow(uint8_t x, uint8_t y, Direction dir);
void wtr_deleteFlow(flow_t *self);

bool wtr_doFlow(flow_t *);
void wtr_tickFlows();

void wtr_animateWater();

#endif