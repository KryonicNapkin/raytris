#include "raylib.h"

#include "graphics.h"
#include "game.h"

#include <time.h>

// TODO: End game screen
// TODO: Shows the next shape
// DONE: Keep track of the time
// TODO: Add levels

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, GAME_TITLE);
    SetTargetFPS(60);

    load_texture_atlas();
    load_font();
    SetRandomSeed(time(NULL));
    Game game = {0};
    game_init(&game);

    while (!WindowShouldClose() && game.running) {
        if (game.state != STATE_GAME_OVER) {
            handle_input(&game, game_get_input());
            game_update(&game);
        } else {
            handle_end_game(&game, draw_endgame_screen(WIN_WIDTH, WIN_HEIGHT, game.play_time_s, game.score));
        }
        BeginDrawing(); {
            ClearBackground(GetColor(BG_COLOR));
            if (game.state != STATE_GAME_OVER) game_draw(game);
        } EndDrawing();
    }
    unload_font();
    unload_texture_atlas();

    CloseWindow();
    return 0;
}
