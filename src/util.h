#ifndef UTIL_H
#define UTIL_H

// version 0
/* typedef struct 
{
    bool completed[20]; // size should be equal to total levels

} save_t; */

// version 1
/* typedef struct 
{
    bool version;
    bool completed[20]; // size should be equal to total levels
} save_t;
 */

// version 2
typedef struct
{
    uint8_t version;
    uint8_t lastLevelPlayed;
    bool completed[20];
} save_t;


extern struct save_t;

void utl_load();
void utl_save();
ti_var_t utl_upgradeSave();
void setLevelCompletion(uint8_t level);
bool getLevelCompletion(uint8_t level);
void setLastLevelPlayed(uint8_t level);
uint8_t getLastLevelPlayed();
void utl_rleDecompress(uint8_t *data, uint8_t **dest, size_t size);


#endif