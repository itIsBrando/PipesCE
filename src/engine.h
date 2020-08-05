#ifndef ENGINE_H
#define ENGINE_H


void addPosition(Position *pos1, Position pos2);
Position newPosition(uint8_t x, uint8_t y);

Direction getOppositeDirection(Direction dir);

bool isSolid(uint8_t tile);
bool isPushable(uint8_t tile);
bool isSpout(uint8_t tile);

tile_t chkCollision(uint8_t, uint8_t);
Position facingOffset(Direction);

tile_t getTile(uint8_t, uint8_t);
tile_t *getTilePointer(uint8_t row, uint8_t column);
tile_t *getTilePointerSafely(uint8_t row, uint8_t column);

void setTile(uint8_t row, uint8_t column, const tile_t *tile);
bool moveTile(uint8_t x, uint8_t y, Direction dir);


void updatePlayer();
void movePlayer(char dx, char dy);

const pipe_t *pipeFromIndex(const uint8_t tile);

tilesprite_t lookupTile(uint8_t index);

#endif