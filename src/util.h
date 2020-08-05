#ifndef UTIL_H
#define UTIL_H

// version 0
/* typedef struct 
{
    bool completed[20]; // size should be equal to total levels

} save_t; */

// version 1
typedef struct 
{
    bool version;
    bool completed[20]; // size should be equal to total levels
} save_t;

extern struct save_t;

void loadData();
void saveData();
void upgradeSave();
void setLevelCompletion(uint8_t level);
bool getLevelCompletion(uint8_t level);
void rleDecompress(uint8_t *data, uint8_t **dest, size_t size);
void upgradeSave();

#endif