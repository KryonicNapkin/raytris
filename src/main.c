#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "RayTris");

    while (!WindowShouldClose()) {
        BeginDrawing(); {
            ClearBackground(GetColor(0x282C34FF));
        } EndDrawing();
    }

    CloseWindow();
    return 0;
}
