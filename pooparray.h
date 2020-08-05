#ifndef ARRAY_H
#define ARRAY_H

typedef void* var;

typedef struct 
{
    char *name;
    var (*get)(var, var);
    void (*set)(var, var, var);
} Type;

typedef struct 
{
    unsigned int size;
    var data;
    size_t nsize;
    Type type;
} Array;

var Array_Get(var array, var i);
void Array_Set(var array, var key, var i);
var Array_New(unsigned int size, const size_t nsize);
void Array_Destroy(Array *a);

extern struct Array;
extern struct Type;

#define E_ALLOC "Could not allocate"
#define E_OUT_OF_BOUNDS "Out of Bounds"
#define ARRAY_TYPE {"Array", &Array_Get, &Array_Set};

#define new(T, size, ntype) (T*)Array_New(size, sizeof(ntype))
//#define Array(size, ntype) (Array*)Array_New(size, sizeof(ntype))
#define destroy(obj) Array_Destroy(obj)
#define get(obj, index) ((Array*)obj)->type.get(obj, index)
#define set(obj, key, index) ((Array*)obj)->type.set(obj, key, index)

#endif