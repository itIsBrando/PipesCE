#include "main.h"
#include "array.h"
#include "tilemap.h"
#include "fire.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"


/* puts an object at an index.
 * @param &self pointer to array to modify
 * @param value object to write
 * @param index index to write 
 * @returns none.*/
void Array_Set(Array *self, var value, unsigned int index) {
    const unsigned int i = index * self->nsize;
    const char *ptr = (char*)self->data + i;
    
    if(index > self->size)
        dbg_sprintf(dbgerr, "\nIndex Out of Bounds tried: %d largest valid: %d\n\n", index, self->size);

    memcpy(ptr, value, self->nsize);
}

/* resets the array but doesn't clear memory.
 * @param &self pointer to array
 * @returns none.*/
void Array_Clear(Array *self) {
    self->lastUsed = 0;
}


/* safely looks up an index
 * @param &self pointer to array to modify
 * @param index index to read 
 * @returns an object at a given index.*/
var Array_Get(Array *self, unsigned int index) {
    return (char*)self->data + index * self->nsize;
}


/* safely adds an object to the end of the array
 * @param &self pointer to array to modify
 * @param value object
 * @returns none.*/
void Array_Append(Array *self, var value) {
    Array_Set(self, value, self->lastUsed);
    self->lastUsed++;
}


var Array_GetLast(Array *self) {
    return Array_Get(self, self->lastUsed-1);
}


/* Initializes a new array.
 * Throws an error if memory could not be allocated.
 * @param &self pointer to array to modify
 * @param elements number of indexes
 * @param size size_t of each object
 * @returns none.*/
void Array_New(Array *self, const unsigned int elements, const size_t size) {
    const unsigned int BYTES = elements * size;
    self->size = elements;
    self->nsize = size;
    self->lastUsed = 0;
    self->data = malloc(BYTES);

    dbg_sprintf(dbgerr, "New array loc: %p, size: %d\n", self->data, BYTES);
    assert(self->data);
}


/* Cleans up an array
 * @param &self pointer to array to modify
 * @returns none.*/
void Array_Destroy(Array *self) {
    free(self->data);
}


/* Removes an index from an array
 * Updates the size
 * @param &self pointer to array to modify
 * @param index index to remove 
 * @returns none.*/
void Array_RemoveAt(Array *self, const unsigned int index) {

    if(index > self->lastUsed)
        assert(false);

    self->lastUsed--;

    if(index == self->lastUsed)
        return;

    memcpy((char*)self->data + index * self->nsize, (char*)self->data + (index+1) * self->nsize, (self->size - index) * self->nsize);
}


/* Removes an element from an Array given a pointer to the object
 * @param &self pointer to array to modify
 * @param item pointer of object to remove (var)
 * @returns none.*/
void Array_Remove(Array *self, var item) {
    unsigned int i;

    for(i = 0; i < self->lastUsed; i++) {
        if((var)Array_Get(self, i) == (var)item) {
            Array_RemoveAt(self, i);
            return;
        }
    }

    assert(false);
}


/* Resizes an Array
 * ***lastUsed is not modified!!!***
 * @param &self pointer to array to modify
 * @param size number of elements
 * @returns true if succesful.*/
bool Array_Resize(Array *self, unsigned int size) {
    const unsigned int actualSize = size * self->nsize;

    // break if 0 is passed as size or array was never initialized
    assert(self->data);
    assert(actualSize);

    free(self->data);

    self->data = malloc(actualSize);

    return (bool)self->data;
}