#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "raylib.h"

#define TEXTURE_ATLAS_FILENAME   "assets/texture_atlas.png"
#define TEXTURE_ATLAS_WIDTH      160
#define TEXTURE_ATLAS_HEIGHT     20
#define TEXTURE_SIZE             20
#define TEXTURES_PER_ROW         8 
#define TEXTURES_PER_COL         1 

#define FONT_FILENAME            "assets/CutePixel.ttf"
#define FONT_SIZE                48
#define FONT_SPACING             2
#define SCORE_TEXT_TOP_OFFSET    10

#define TEXTURE_RECT(id)         ((Rectangle){ \
    .x = (id % TEXTURES_PER_ROW)*TEXTURE_SIZE, \
    .y = (id / TEXTURES_PER_COL)*TEXTURE_SIZE, \
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

extern const Rectangle _tex_rects[TEXTURES_PER_COL*TEXTURES_PER_ROW];
extern Texture2D _tex_atlas;
extern Font _font;

void load_texture_atlas(void);
void unload_texture_atlas(void);
void load_font(void);
void unload_font(void);

void draw_score(Rectangle grid_bounds, unsigned int score);
void draw_play_time(Rectangle grid_bounds, unsigned int play_time);

#endif /* GRAPHICS_H_ */
