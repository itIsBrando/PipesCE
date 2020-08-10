#include "main.h"
#include "array.h"
#include "fire.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"


bool isSolid(uint8_t tile) {
	return lookupTile(tile).flags & 0x01;
}

bool isPushable(uint8_t tile) {
	return lookupTile(tile).flags & 0x02;
}

bool isSpout(uint8_t tile) {
	return lookupTile(tile).flags & 0x4;
}


/* fetches an X and Y offset based off of the direction
 * @param d Direction.
 * @returns a position.*/
Position facingOffset(Direction d) {
	const char dx[4] = {-1, 1, 0, 0};
	const char dy[4] = {0, 0, -1, 1};
	Position pos;
	pos.x = dx[d];
	pos.y = dy[d];
	return pos;
}


/* returns a tile given an x and y position.
 * or returns 1 if off the map
 * @param row X coordinate of tile
 * @param column Y coordinate of tile
 * @returns tile_t.
 * @see getTile */
tile_t chkCollision(uint8_t x, uint8_t y) {
	const uint8_t w = curLevel.width;
	const uint8_t h = curLevel.height;

	if(x > w || x < 0 || y > h || y < 0) {
		// create a fake tile
		tile_t t = {0};
		return t;
	}
	else
	{
		return getTile(x, y);
	}

}


/* gets a copy of the tile at (row, column)
 * @param row X coordinate of tile
 * @param column Y coordinate of tile
 * @returns tile_t. */
tile_t getTile(uint8_t row, uint8_t column) {
	const uint8_t width = curLevel.width;

	return curLevel.data[column * width + row];
}



/* gets a pointer to the tile at (row, column).
 * @param row X coordinate of tile
 * @param column Y coordinate of tile
 * @returns tile_t *. */
tile_t *getTilePointer(uint8_t row, uint8_t column) {
	const uint8_t width = curLevel.width;

	return &curLevel.data[column * width + row];
}


/* gets a pointer to the tile at (row, column) safely.
 * @param row X coordinate of tile
 * @param column Y coordinate of tile
 * @returns tile_t *. */
tile_t *getTilePointerSafely(uint8_t row, uint8_t column) {
	const uint8_t width = curLevel.width;
	const uint8_t height = curLevel.height;

	return &curLevel.data[(column >= height ? height-1 : column) * width + (row >= width ? width-1 : row)];
}


/* places a tile at an index.
 * also draws the tile.
 * @param row X coordinate of tile
 * @param column Y coordinate of tile
 * @param *tile pointer to the draw to write (unchanged).
 * @returns none. */
void setTile(uint8_t row, uint8_t column, const tile_t *tile) {
	const uint8_t width = curLevel.width;
	tile_t *ptr = &curLevel.data[column * width + row];

	if(tile) {
		*ptr = *tile;
	} else {
		tile_t t = {0};
		*ptr = t;
	}
	
	scrn_drawTile(*ptr, row, column);
}



/* called every frame to animate the player. never used
 * @returns none.*/
void updatePlayer() {
	if(player.dx != 0)
		player.dx++;
	else if(player.dy != 0)
		player.dy++;

	if(player.dx >= 8 || player.dy >= 8) {
		const Position pos = facingOffset(player.direction);
		
		player.dx = player.dy = 0;
		player.isMoving = false;
		// actually move player

		//addPosition(&player.position, pos);
		player.x += (char)pos.x;
		player.y += (char)pos.y;

		// check for starting water flow
		if(chkCollision(player.x, player.y).id == TILE_HANDLE && !player.isWaterFlowing)
			wtr_initFlows();
		else if(player.isWaterFlowing) {
			wtr_drainWater();
		}
	}
	
	
}


void addPosition(Position *self, Position pos) {
	self->x += pos.x;
	self->y += pos.y;
}

// this is a dangerous routine because the return struct will get eaten on the stack
Position newPosition(uint8_t x, uint8_t y) {
	Position pos;
	pos.x = x;
	pos.y = y;
	
	return pos;
}


/* opposite of the passed direction.
 * e.g. LEFT returns RIGHT
 * @param dir Direction to invert
 * @returns the opposite direction that is passed.*/
Direction getOppositeDirection(Direction dir) {
	const Direction old[] = {LEFT, RIGHT, UP, DOWN};
	const Direction new[] = {RIGHT, LEFT, DOWN, UP};
	uint8_t i;

	for(i = 0; i < sizeof old; i++) {
		if(old[i] == dir) 
			return new[i];
	}

	return 0; // will always return tho. just to silence the compiler
}


/* only one direction allowed at a time
 * safely moves the player. Should be called even if we know that the tile in a certain direction is solid.
 * Does not actually update player's position yet.
 * @param dx x displacement
 * @param dy y displacement
 * @returns a 'pipe_t' or NULL if not found.*/
void movePlayer(char dx, char dy) {
	tile_t tile = chkCollision(player.x + dx, player.y + dy);
	bool forceMove = false;

	// if we found a pipe
	if(pipeFromIndex(tile.id)) {
		// check if we moved that tile, if so, move player, else return
		if(moveTile(player.x + dx, player.y + dy, player.direction))
		{
			// if `moveTile` succeeded, then `tile` will have changed
			tile = chkCollision(player.x + dx, player.y + dy);
			// force the player to move
			forceMove = true;
		}
	}


	// if we are pulling a pipe
	if(player.isPulling) {
		const Direction d = getOppositeDirection(player.direction);
		const Position pos = facingOffset(d);
		const uint8_t x = player.x + (char)pos.x, y = player.y + (char)pos.y;
		const tile_t t = chkCollision(x, y);

		if(pipeFromIndex(t.id) && !isSolid(tile.id))
			moveTile(x, y, player.direction);
		
	}

	// initate move if tile is not solid and we are not currently moving
	if(forceMove || !isSolid(tile.id)) {
		player.isMoving = true;
		player.dx = dx != 0;
		player.dy = dy != 0;
	}

}


/* Pushs a tile at (x, y) in a direction.
 * Performs checks to ensure the tile can be pushed.
 * @param x x coordinate.
 * @param y y coordinate.
 * @param dir direction to push the tile.
 * @returns true if successful.*/
bool moveTile(uint8_t x, uint8_t y, Direction dir) {
	const Position pos = facingOffset(dir);
	const uint8_t newX = x + (char)pos.x, newY = y + (char)pos.y;
	const tile_t newTile = chkCollision(newX, newY);
	const bool isRotate = newTile.type == TYPE_ROTATE && newTile.id == TILE_ROTATION;
	tile_t tile = getTile(x, y);

	if ((isRotate || newTile.id == 0) && isPushable(tile.id))
	{
		if(tile.type == TYPE_ROTATE && tile.id != TILE_ROTATION)
		{
			// is there is a pipe on the rotation tile and we moved it off of the rotation tile,
			// then replace the id of the rotation tile and fix pipe type
			tile.type = TYPE_PIPE;
			setTile(newX, newY, &tile);

			tile.type = TYPE_ROTATE;
			tile.id = TILE_ROTATION;
			tile.flags = 0;
			setTile(x, y, &tile);
			return true;
		} else if(isRotate)
		{
			tile.type = TYPE_ROTATE;
		}

		setTile(newX, newY, &tile);
		setTile(x, y, NULL);


		return true;
	}
	
	return false;
}

/* All pipes must be register in `pipes` array so this
 * routine can idenify them
 * @param tile index of a tile
 * @returns a 'pipe_t' or NULL if not found.*/
const pipe_t *pipeFromIndex(const uint8_t tile) {
	uint8_t i;

	for(i = 0; i < PIPES_SIZE; i++)
	{
		if(lookupTile(tile).id == pipes[i].id) 
			return &pipes[i];
	}

	return NULL;
}

/* lookups an index in 'tileSprites'
 * @param index tile index
 * @returns tilesprite_t
*/
tilesprite_t lookupTile(uint8_t index)
{
	return tileset[index];
}