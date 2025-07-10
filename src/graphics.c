#include "graphics.h"
#include "raylib.h"

#include "game.h"

#include <stdio.h>

const Rectangle _tex_rects[TEXTURES_COUNT] = {
    TEXTURE_RECT(TEXTURE_IBLOCK),
    TEXTURE_RECT(TEXTURE_OBLOCK),
    TEXTURE_RECT(TEXTURE_TBLOCK),
    TEXTURE_RECT(TEXTURE_JBLOCK),
    TEXTURE_RECT(TEXTURE_LBLOCK),
    TEXTURE_RECT(TEXTURE_SBLOCK),
    TEXTURE_RECT(TEXTURE_ZBLOCK),
    TEXTURE_RECT(TEXTURE_CELL_BG),
};

Texture2D _tex_atlas;
Font _high_font = {0};
Font _low_font = {0};

void load_texture_atlas(void) {
    Image img = LoadImage(TEXTURE_ATLAS_FILENAME);
    _tex_atlas = LoadTextureFromImage(img);
    UnloadImage(img);
}

void unload_texture_atlas(void) {
    UnloadTexture(_tex_atlas);
}

void load_fonts(void) {
    _high_font = LoadFontEx(FONT_FILENAME, HIGH_FONT_SIZE, NULL, 0);
    _low_font = LoadFontEx(FONT_FILENAME, LOW_FONT_SIZE, NULL, 0);
}

void unload_fonts(void) {
    UnloadFont(_high_font);
    UnloadFont(_low_font);
}

void draw_score(Rectangle grid_bounds, unsigned int score) {
    const char* text = "SCORE";
    char score_buff[256];
    snprintf(score_buff, sizeof(score_buff), "%05d", score);

    Vector2 text_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_high_font, text, HIGH_FONT_SIZE, 0).x)/2.0f)+grid_bounds.x+grid_bounds.width,
        .y = MeasureTextEx(_high_font, text, HIGH_FONT_SIZE, 0).y+SCORE_TEXT_TOP_OFFSET,
    };

    Vector2 score_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_low_font, score_buff, LOW_FONT_SIZE, 0).x)/2.0f)+grid_bounds.x+grid_bounds.width,
        .y = text_pos.y+(6.0*SCORE_TEXT_TOP_OFFSET),
    };

    DrawTextEx(_high_font, text, text_pos, HIGH_FONT_SIZE, 0, WHITE);
    DrawTextEx(_low_font, score_buff, score_pos, LOW_FONT_SIZE, 0, WHITE);
}
