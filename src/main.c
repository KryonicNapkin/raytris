#include "raylib.h"

#include "graphics.h"
#include "game.h"

#include <time.h>

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, GAME_TITLE);
    SetTargetFPS(60);

    load_texture_atlas();
    load_fonts();
    SetRandomSeed(time(NULL));
    Game game = {0};
    game_init(&game);

    float frame_time = 0.0f;
    while (!WindowShouldClose() && game.running) {
        handle_input(&game, game_get_input());
        game_update(&game);
        BeginDrawing(); {
            ClearBackground(GetColor(BG_COLOR));
            game_draw(game);
            draw_score(game.grid.bounds, game.score);
        } EndDrawing();
    }
    unload_fonts();
    unload_texture_atlas();

    CloseWindow();
    return 0;
}
