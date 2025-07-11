#include "graphics.h"
#include "raylib.h"

#include <stdio.h>

const Rectangle _tex_rects[TEXTURES_PER_COL*TEXTURES_PER_ROW] = {
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
Font _font = {0};

void load_texture_atlas(void) {
    Image img = LoadImage(TEXTURE_ATLAS_FILENAME);
    _tex_atlas = LoadTextureFromImage(img);
    UnloadImage(img);
}

void unload_texture_atlas(void) {
    UnloadTexture(_tex_atlas);
}

void load_font(void) {
    _font = LoadFontEx(FONT_FILENAME, FONT_SIZE, NULL, 0);
}

void unload_font(void) {
    UnloadFont(_font);
}

void draw_score(Rectangle grid_bounds, unsigned int score) {
    const char* text = "SCORE";
    char score_buff[8];
    snprintf(score_buff, sizeof(score_buff), "%05d", score);

    Vector2 text_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).x)/2.0f)+grid_bounds.x+grid_bounds.width,
        .y = MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).y+SCORE_TEXT_TOP_OFFSET,
    };

    Vector2 score_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_font, score_buff, FONT_SIZE, FONT_SPACING).x)/2.0f)+grid_bounds.x+grid_bounds.width,
        .y = text_pos.y+(6.0*SCORE_TEXT_TOP_OFFSET),
    };

    DrawTextEx(_font, text, text_pos, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, score_buff, score_pos, FONT_SIZE, FONT_SPACING, WHITE);
}

void draw_play_time(Rectangle grid_bounds, unsigned int play_time) {
    int secs = play_time % 60;
    int mins = play_time / 60;

    const char* time_text = "PLAY TIME";
    char time_buff[64];
    snprintf(time_buff, sizeof(time_buff), "%02d:%02d", mins, secs);

    Vector2 text_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_font, time_text, FONT_SIZE, FONT_SPACING).x)/2.0f),
        .y = MeasureTextEx(_font, time_text, FONT_SIZE, FONT_SPACING).y+SCORE_TEXT_TOP_OFFSET,
    };

    Vector2 time_pos = {
        .x = ((grid_bounds.x-MeasureTextEx(_font, time_buff, FONT_SIZE, FONT_SPACING).x)/2.0f),
        .y = text_pos.y+(6.0f*SCORE_TEXT_TOP_OFFSET),
    };

    DrawTextEx(_font, time_text, text_pos, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, time_buff, time_pos, FONT_SIZE, FONT_SPACING, WHITE);
}
