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

int draw_endgame_screen(int win_width, int win_height, unsigned int play_time, unsigned int score) {
    const char* text = "Game over!";

    Vector2 text_pos = {
        .x = (win_width-MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).x)/2.0f,
        .y = ((2.0*(win_height/5.0f))-MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).y)/2.0f,
    };

    int mins = play_time / 60;
    int secs = play_time % 60;

    char playtime_buff[64];
    snprintf(playtime_buff, sizeof(playtime_buff), "PLAYTIME: %02d:%02d", mins, secs);

    Vector2 playtime_pos = {
        .x = (win_width-MeasureTextEx(_font, playtime_buff, FONT_SIZE, FONT_SPACING).x)/2.0f,
        .y = ((3.0*(win_height/5.0f))-MeasureTextEx(_font, playtime_buff, FONT_SIZE, FONT_SPACING).y)/2.0f,
    };

    char score_buff[64];
    snprintf(score_buff, sizeof(score_buff), "SCORE: %d", score);

    Vector2 score_pos = {
        .x = (win_width-MeasureTextEx(_font, score_buff, FONT_SIZE, FONT_SPACING).x)/2.0f,
        .y = ((4.0*(win_height/5.0f))-MeasureTextEx(_font, score_buff, FONT_SIZE, FONT_SPACING).y)/2.0f,
    };

    const char* feedback_text1 = "For restart press 'R'";
    const char* feedback_text2 = "For exit press 'Q'";

    Vector2 feedback_pos1 = {
        .x = (win_width-MeasureTextEx(_font, feedback_text1, FONT_SIZE, FONT_SPACING).x)/2.0f,
        .y =((7.0*(win_height/5.0f))-MeasureTextEx(_font, feedback_text1, FONT_SIZE, FONT_SPACING).y)/2.0f, 
    };
    Vector2 feedback_pos2 = {
        .x = (win_width-MeasureTextEx(_font, feedback_text2, FONT_SIZE, FONT_SPACING).x)/2.0f,
        .y =((8.0*(win_height/5.0f))-MeasureTextEx(_font, feedback_text2, FONT_SIZE, FONT_SPACING).y)/2.0f, 
    };

    DrawTextEx(_font, text, text_pos, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, playtime_buff, playtime_pos, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, score_buff, score_pos, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, feedback_text1, feedback_pos1, FONT_SIZE, FONT_SPACING, WHITE);
    DrawTextEx(_font, feedback_text2, feedback_pos2, FONT_SIZE, FONT_SPACING, WHITE);

    if (IsKeyPressed(KEY_R)) return 2;
    else if (IsKeyPressed(KEY_Q)) return 1;
    return 0;
}
