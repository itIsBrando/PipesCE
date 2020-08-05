
#include "main.h"
#include "tilemap.h"
#include "sprites.h"
#include "engine.h"
#include "graphics.h"
#include "water.h"
#include "array.h"
#include "fire.h"
#include "array.h"


static void obj_error(const char *s) {

    dbg_sprintf(dbgerr, s);
}

void Array_Destroy(Array *a) {
    free(a->data);
    free(a);
}

var Array_Get(var array, var i) {
    const Array *a = (Array*) array;
    const unsigned int index = (unsigned int)i * a->nsize;

    if(index > a->size)
        obj_error(E_OUT_OF_BOUNDS);

    return a->data + index;
}

void Array_Set(var array, var key, var i) {
    Array *a = (Array*) array;
    const unsigned int index = (unsigned int)i * a->nsize;

    if(index > a->size)
        obj_error(E_OUT_OF_BOUNDS);

    ((uint8_t*)a->data)[index] = (uint8_t)key;
}


var Array_New(unsigned int size, const size_t nsize) {
    Array *a = malloc(sizeof(Array));
    Type type;
    type.get = &Array_Get;
    type.set = &Array_Set;
    
    a->data = malloc(nsize * size);
    a->size = size;
    a->nsize = nsize;
    a->type = type;

    if(!a)
        obj_error(E_ALLOC);
    
    return a;
}