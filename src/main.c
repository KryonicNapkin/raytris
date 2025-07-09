#include "raylib.h"

#include "textures.h"
#include "game.h"

#include <time.h>

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, GAME_TITLE);
    SetTargetFPS(60);

    load_texture_atlas();
    SetRandomSeed(time(NULL));
    Game game = {0};
    game_init(&game);

    float frame_time = 0.0f;
    while (!WindowShouldClose() && game.running) {
        frame_time += GetFrameTime();
        if (frame_time >= 3/4.0f) {
            frame_time = 0.0f;
            game_update(&game);
        }
        BeginDrawing(); {
            ClearBackground(GetColor(BG_COLOR));
            game_draw(game);
        } EndDrawing();
    }
    unload_texture_atlas();

    CloseWindow();
    return 0;
}
