#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "raylib.h"

#define TEXTURE_ATLAS_FILENAME   "assets/texture_atlas.png"
#define TEXTURE_ATLAS_SIZE       192
#define TEXTURE_SIZE             24
#define TEXTURES_COUNT           8*8 // 8 textures per row and column

#define FONT_FILENAME            "assets/8-BIT WONDER.TTF"
#define HIGH_FONT_SIZE           36
#define LOW_FONT_SIZE            27
#define SCORE_TEXT_TOP_OFFSET    10

#define TEXTURE_RECT(id)         ((Rectangle){ \
    .x = (id % 8)*TEXTURE_SIZE,                \
    .y = (id / 8)*TEXTURE_SIZE,                \
    .width = TEXTURE_SIZE,                     \
    .height = TEXTURE_SIZE,                    \
})

typedef enum {
    TEXTURE_IBLOCK = 0,
    TEXTURE_OBLOCK,
    TEXTURE_TBLOCK,
    TEXTURE_JBLOCK,
    TEXTURE_LBLOCK,
    TEXTURE_SBLOCK,
    TEXTURE_ZBLOCK,
    TEXTURE_CELL_BG,
} Texture_Id; 

extern const Rectangle _tex_rects[TEXTURES_COUNT];
extern Texture2D _tex_atlas;
extern Font _high_font;
extern Font _low_font;

void load_texture_atlas(void);
void unload_texture_atlas(void);
void load_fonts(void);
void unload_fonts(void);

void draw_score(Rectangle grid_bounds, unsigned int score);

#endif /* GRAPHICS_H_ */
