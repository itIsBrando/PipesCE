#include "main.h"
#include "array.h"
#include "util.h"
#include "images/colors.h"

#define SAVE_NAME "PIPESAVE"
#define CURRENT_SAVE_VERSION 2

static ti_var_t saveSlot;
static save_t save;


void loadData() {
    ti_var_t file;
    ti_CloseAll();

 
    if(ti_Open(SAVE_NAME, "r"))
    {
        file = ti_Open(SAVE_NAME, "r+");
        memcpy(&save, ti_GetDataPtr(file), sizeof(save_t));
        if(save.version != CURRENT_SAVE_VERSION)
        {
            file = upgradeSave();
        }
    } else {
        file = ti_Open(SAVE_NAME, "w+");
        ti_Resize(sizeof(save_t), file);
        save.version = CURRENT_SAVE_VERSION;
    }

    saveSlot = file;
}


void saveData() {
    const void *pointer = ti_GetDataPtr(saveSlot);

    memcpy(pointer, &save, sizeof(save_t));
    ti_CloseAll();
}


void upgradeSaveFail() {
    gfx_SetTextFGColor(COLOR_RED);
    gfx_PrintStringXY("Save couldn't be upgraded.", 0, 16);
    gfx_PrintStringXY("Delete "SAVE_NAME" appvar. Report version:", 0, 24);
    gfx_PrintUInt(save.version, 2);

    while(!os_GetCSC());
    gfx_End();
    exit(0);
}


ti_var_t upgradeSave() {
    ti_var_t file;
    save_t *oldSave = malloc(sizeof(save_t));

    gfx_PrintStringXY("Save version depreciated.", 0, 0);
    gfx_PrintStringXY("Attempting to upgrade save.", 0, 8);
            
    ti_CloseAll();
    file = ti_Open(SAVE_NAME, "r+");
    ti_Resize(sizeof(save_t), file);

    // copy old data into the save struct
    memcpy(oldSave, &save, sizeof(save_t));


    // all handable save versions
    switch(oldSave->version)
    {
        case 0:
            break;
        case 1:
            // version byte (1) = lastLevel
            // then copy all the complete level stuff
            memcpy(&save.completed, &oldSave->lastLevelPlayed, 20);
            break;
        default:
            upgradeSaveFail();

    }
    // change version
    save.version = CURRENT_SAVE_VERSION;

    gfx_PrintStringXY("Save upgraded from ", 0, 16);
    gfx_PrintUInt(oldSave->version, 2);
    gfx_PrintString("to newest version ");
    gfx_PrintUInt(CURRENT_SAVE_VERSION, 2);

    free(oldSave);

    while(!os_GetCSC());

    return file;
}


void setLevelCompletion(uint8_t level) {
    save.completed[level] = true;    
}


bool getLevelCompletion(uint8_t level) {
    return save.completed[level];
}

uint8_t getLastLevelPlayed()
{
    return save.lastLevelPlayed;
}

void setLastLevelPlayed(uint8_t level)
{
    save.lastLevelPlayed = level;
}



/* Fully compatibilty with normal RLE compression.
 * However, data should be optimized.
 * Any single-run data byte should have its highest bit set and data byte omitted
 * @param *data source of data
 * @param **dest memory location of a pointer to uint8_t
 * @param size number of bytes of *data
 * @returns void */
void rleDecompress(uint8_t *data, uint8_t **dest, size_t size) {
    uint8_t j;
    uint24_t i, out = 0;

    for(i = 0; i <= size; i++)
    {
        const uint8_t count = data[i];

        // if bit 7 is set, then this is a data byte and needs masking
        if(count & 0x80)
        {
            (*dest)[out++] = count & 0x7F;
        } else
        {
            // if we have a size, copy data to our buffer
            memset((*dest + out), data[i + 1], count);
            out += count;
            i++; // skip over the data byte
        }
        
    }

}