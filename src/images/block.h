#ifndef block_include_file
#define block_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define block_width 8
#define block_height 8
#define block_size 66
#define block ((gfx_sprite_t*)block_data)
extern unsigned char block_data[66];

#ifdef __cplusplus
}
#endif

#endif
