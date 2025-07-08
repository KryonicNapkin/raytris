#include "textures.h"
#include "raylib.h"

const Rectangle _tex_rects[TEXTURES_COUNT] = {
    TEXTURE_RECT(TEXTURE_CELL_BG),
    TEXTURE_RECT(TEXTURE_IBLOCK),
    TEXTURE_RECT(TEXTURE_OBLOCK),
    TEXTURE_RECT(TEXTURE_TBLOCK),
    TEXTURE_RECT(TEXTURE_JBLOCK),
    TEXTURE_RECT(TEXTURE_LBLOCK),
    TEXTURE_RECT(TEXTURE_SBLOCK),
    TEXTURE_RECT(TEXTURE_ZBLOCK),
};

Texture2D _tex_atlas;
int _atlas_loaded = 0;

void load_texture_atlas(void) {
    Image img = LoadImage(TEXTURE_ATLAS_FILENAME);
    _tex_atlas = LoadTextureFromImage(img);
    _atlas_loaded = 1;
    UnloadImage(img);
}

void unload_texture_atlas(void) {
    UnloadTexture(_tex_atlas);
}
