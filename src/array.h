#ifndef ARRAY_H
#define ARRAY_H

typedef void* var;

typedef struct
{
    unsigned int size;
    var data;
    unsigned int lastUsed; // the index of the last functional element
    size_t nsize;
} Array;

// #define Array_Init(self, elements, size, ...) do { Array_New(self, elements, size); memcpy((self)->data, &{__VA_ARGS__})}, size * elements); while(false)

void Array_Set(Array *self, var value, unsigned int index);
void Array_Append(Array *self, var value);
var Array_GetLast(Array *self);
void Array_New(Array *self, const unsigned int elements, const size_t size);
bool Array_Resize(Array *self, unsigned int size);
void Array_Destroy(Array *self);
void Array_RemoveAt(Array *self, const unsigned int index);
void Array_Remove(Array *self, var item);
var Array_Get(Array *, unsigned int);
void Array_Clear(Array *self);

#define Array_Len(self) (self)->size
#define foreach(array, iter) for(iter = 0; iter < array.lastUsed; iter++)


extern struct Array;





#endif