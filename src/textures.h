#ifndef TEXTURES_H_
#define TEXTURES_H_

#include "raylib.h"

#define ATLAS_FILENAME   "assets/texture_atlas.png"
#define ATLAS_SIZE       192
#define TEXTURE_SIZE     24
#define TEXTURES_COUNT   8*8 // 8 textures per row and column

#define TEXTURE_RECT(id) ((Rectangle){ \
    .x = (id % 8)*TEXTURE_SIZE,        \
    .y = (id / 8)*TEXTURE_SIZE,        \
    .width = TEXTURE_SIZE,             \
    .height = TEXTURE_SIZE,            \
})

typedef enum {
    TEXTURE_CELL_BG = 0,
    TEXTURE_IBLOCK,
    TEXTURE_OBLOCK,
    TEXTURE_TBLOCK,
    TEXTURE_JBLOCK,
    TEXTURE_LBLOCK,
    TEXTURE_SBLOCK,
    TEXTURE_ZBLOCK,
} Texture_Id; 

extern const Rectangle _tex_rects[TEXTURES_COUNT];
extern Texture2D _tex_atlas;
extern int _atlas_loaded;

void load_atlas(void);
void unload_atlas(void);

#endif /* TEXTURES_H_ */
