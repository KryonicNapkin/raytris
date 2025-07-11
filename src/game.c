#include "game.h"
#include "graphics.h"
#include "raylib.h"

#include <string.h>

Point _shapes[SHAPES_COUNT][SHAPE_BLOCKS] = {
    {{0, 0}, {-1, 0}, {1, 0},  {2, 0}},  // I BLOCK
    {{0, 0}, {1, 0},  {0, 1},  {1, 1}},  // O BLOCK
    {{0, 0}, {1, 0},  {-1, 0}, {0, 1}},  // T BLOCK
    {{0, 0}, {0, -1}, {0, 1},  {1, 1}},  // L BLOCK
    {{0, 0}, {0, -1}, {0, 1},  {-1, 1}}, // J BLOCK
    {{0, 0}, {-1, 0}, {0, 1},  {1, 1}},  // S BLOCK
    {{0, 0}, {0, 1},  {-1, 1},  {1, 0}}, // Z BLOCK
};

void game_init(Game* game) {
    game->grid = grid_create();
    game->active_shape = shape_create(game->grid, NULL);
    game->next_shape = shape_create(game->grid, NULL);
    game->running = 1;
    game->play_time_s = 0;
    game->score = 0;
}

Grid grid_create(void) {
    Grid grid = {0};
    grid.bounds = grid_get_bounds(GRID_ROWS, GRID_COLS, CELL_SPACING, CELL_SIZE); 
    grid.cell_spacing = CELL_SPACING;
    for (int i = 0; i < GRID_COLS*GRID_ROWS; ++i) {
        grid.cells[i] = TEXTURE_CELL_BG;
    }
    return grid;
}

void game_update(Game* game) {
    static float frame_time = 0.0f;
    static float play_time_frame = 0.0f;

    frame_time += GetFrameTime();
    play_time_frame += GetFrameTime();

    if (frame_time >= GAME_UPDATE_FREQ) {
        Point temp = {0};
        for (int i = 0; i < SHAPE_BLOCKS; ++i) {
            temp.x = game->active_shape.blocks[i].x + game->active_shape.x;
            temp.y = game->active_shape.blocks[i].y + game->active_shape.y + 1;
            if (check_collisions(game->grid, temp.x, temp.y)) {
                shape_lock(game, &game->active_shape);
                frame_time = 0.0f;
                return;
            }
        }
        game->active_shape.y += 1;
        frame_time = 0.0f;
    }

    if (play_time_frame >= 1.0f) {
        game->play_time_s += 1;
        play_time_frame = 0.0f;
    }
}

void game_reset(Game* game) {
    game->running = 1;
    game->score = 0;
    game->play_time_s = 0;
    game->state = STATE_IN_GAME;
    for (int i = 0; i < GRID_ROWS*GRID_COLS; ++i) {
        game->grid.cells[i] = TEXTURE_CELL_BG;
    }
    game->active_shape = shape_create(game->grid, NULL);
    game->next_shape = shape_create(game->grid, NULL);
}

void game_draw(Game game) {
    grid_draw(game.grid, GRID_ROWS, GRID_COLS);
    Vector2 base_pos = {
        .x = game.grid.bounds.x+game.grid.cell_spacing,
        .y = game.grid.bounds.y+game.grid.cell_spacing,
    };
    shape_draw(base_pos, game.active_shape);
    draw_score(game.grid.bounds, game.score);
    draw_play_time(game.grid.bounds, game.play_time_s);
    
}

void grid_draw(Grid grid, int grid_rows, int grid_cols) {
    Vector2 pos = {grid.bounds.x+grid.cell_spacing, grid.bounds.y+grid.cell_spacing};
    Vector2 cell_pos = pos;
    for (int i = 0; i < grid_rows*grid_cols; ++i) {
/*         TraceLog(LOG_INFO, "x: %d, y: %d", (int)cell_pos.x, (int)cell_pos.y); */
        DrawTextureRec(_tex_atlas, _tex_rects[grid.cells[i]], cell_pos, WHITE);
        cell_pos.x += (grid.cell_spacing+CELL_SIZE);
        if ((i+1) % grid_cols == 0) {
            cell_pos.y += (grid.cell_spacing+CELL_SIZE);
            cell_pos.x = pos.x;
        }
    }
    Rectangle border = {
        .x = grid.bounds.x-GRID_BORDER_WIDTH,
        .y = grid.bounds.y-GRID_BORDER_WIDTH,
        .width = grid.bounds.width+(2.0f*GRID_BORDER_WIDTH),
        .height = grid.bounds.height+(2.0f*GRID_BORDER_WIDTH),
    };
    DrawRectangleLinesEx(border, GRID_BORDER_WIDTH, GetColor(0x61AFEFFF));
}

void show_next_shape(Shape next_shape) {
    const char* text = "Next";
    Rectangle game_grid_bounds = grid_get_bounds(GRID_ROWS, GRID_COLS, CELL_SPACING, CELL_SIZE);

    Vector2 pos = {
        .x = (game_grid_bounds.x-MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).x)/2.0f+game_grid_bounds.x+game_grid_bounds.width,
        .y = (WIN_HEIGHT/2.0f)+MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).y,
    };

    DrawTextEx(_font, text, pos, FONT_SIZE, FONT_SPACING, WHITE);

    const int width = 100;
    const int height = 100;
    Rectangle border = {
        .width = width,
        .height = height,
        .x = (game_grid_bounds.x-width)/2.0f+game_grid_bounds.x+game_grid_bounds.width,
        .y = (WIN_HEIGHT/2.0f)+MeasureTextEx(_font, text, FONT_SIZE, FONT_SPACING).y+(width/2.0f),
    };

    Vector2 shape_pos = {
        .x = border.x+(border.width)/2.0f-(CELL_SIZE+CELL_SPACING)/1.5f,
        .y = border.y+(border.height)/2.0f-(CELL_SIZE+CELL_SPACING)/1.5f,
    };
    DrawRectangleLinesEx(border, 1, WHITE);
    for (int i = 0; i < SHAPE_BLOCKS; ++i) {
        Point base = {next_shape.blocks[i].x, next_shape.blocks[i].y};

        Vector2 draw_pos = {
            .x = shape_pos.x+(base.x*(CELL_SIZE+CELL_SPACING)), 
            .y = shape_pos.y+(base.y*(CELL_SIZE+CELL_SPACING)),
        };
        DrawTextureRec(_tex_atlas, _tex_rects[next_shape.texture_id], draw_pos, WHITE);
    }
}

Shape shape_create(Grid grid, int* result) {
    Shape shape = {0};

    shape.texture_id = GetRandomValue(TEXTURE_IBLOCK, TEXTURE_ZBLOCK);
    memcpy(shape.blocks, _shapes[shape.texture_id], SHAPE_BLOCKS * sizeof(Point));

    shape.x = GRID_COLS/2;
    shape.y = 0;

    if (result != NULL) *result = check_collisions(grid, shape.x, shape.y);

    return shape;
}

void shape_draw(Vector2 base_pos, Shape shape) {
    for (int i = 0; i < SHAPE_BLOCKS; ++i) {
        Point base = {shape.blocks[i].x+shape.x, shape.blocks[i].y+shape.y};

        Vector2 draw_pos = {
            .x = base_pos.x+(base.x*(CELL_SIZE+CELL_SPACING)), 
            .y = base_pos.y+(base.y*(CELL_SIZE+CELL_SPACING)),
        };

        if (base.y >= 0) {
            DrawTextureRec(_tex_atlas, _tex_rects[shape.texture_id], draw_pos, WHITE);
        }
    }
}

void shape_move(Grid grid, Shape* shape, int dir) {
    Point temp = {0};
    for (int i = 0; i < SHAPE_BLOCKS; ++i) {
        temp.x = shape->blocks[i].x+shape->x+dir;
        temp.y = shape->blocks[i].y+shape->y;
        if (check_collisions(grid, temp.x, temp.y)) return;
    }
    shape->x += dir;
}

void shape_rotate(Grid grid, Shape* shape) {
    if (shape->texture_id == TEXTURE_OBLOCK) return; 

    Point temp[SHAPE_BLOCKS] = {0};

    for (int i = 0; i < SHAPE_BLOCKS; ++i) {
        temp[i].x = shape->blocks[i].y;
        temp[i].y = -shape->blocks[i].x;
        if (check_collisions(grid, temp[i].x+shape->x, temp[i].y+shape->y)) return;
    }
    for (int x = 0; x < SHAPE_BLOCKS; ++x) {
        shape->blocks[x] = temp[x];
    }
}

void shape_lock(Game* game, Shape* shape) {
    Point temp = {0};

    int destroy_rows[SHAPE_BLOCKS];
    int destroy_rows_size = 0;

    for (int i = 0; i < SHAPE_BLOCKS; ++i) {
        temp.x = shape->blocks[i].x+shape->x;
        temp.y = shape->blocks[i].y+shape->y;

        if (temp.x >= 0 && temp.x < GRID_COLS && temp.y >= 0 && temp.y < GRID_ROWS) {
            game->grid.cells[(temp.y*GRID_COLS)+temp.x] = shape->texture_id;
        }
        if (is_row_full(game->grid, temp.y)) {
            destroy_rows[destroy_rows_size++] = temp.y;
        }
    }
    for (int x = 0; x < destroy_rows_size; ++x) {
        grid_clear_row(&game->grid, destroy_rows[x], &game->score);
    }
    int result;

    if (destroy_rows_size == 0) game->score += 1;
    game->active_shape = game->next_shape;
    game->next_shape = shape_create(game->grid, &result);
    if (result) game->state = STATE_GAME_OVER;
}

void shape_fall(Game* game, Shape* shape) {
    Point temp[SHAPE_BLOCKS];
    int result = 0;
    calc:
        for (int i = 0; i < SHAPE_BLOCKS; ++i) {
            Point temp = {shape->blocks[i].x+shape->x, shape->blocks[i].y+shape->y+1};
            if (!check_collisions(game->grid, temp.x, temp.y)) {
                result++;
            } else {
                shape_lock(game, shape);
                return;
            }
        }
        if (result) {
            shape->y += 1;
            result = 0;
            goto calc;
        }
}

int check_collisions(Grid grid, int x, int y) {
    if (x < 0 || x >= GRID_COLS) return 1;
    if (y >= GRID_ROWS) return 1;
    if (y >= 0 && grid.cells[(y*GRID_COLS)+x] != TEXTURE_CELL_BG) return 1;
    return 0;
}

void grid_clear_row(Grid* grid, int row, unsigned int* new_score) {
    if (row < 0 || row >= GRID_ROWS) return;

    for (int r = row; r > 0; --r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            grid->cells[(r*GRID_COLS)+c] = grid->cells[((r-1)*GRID_COLS)+c];
        }
    }
    if (new_score != NULL) *new_score += 50; 
}

int is_row_full(Grid grid, int row) {
    if (row < 0 || row >= GRID_ROWS) return -1;

    int result = GRID_COLS;
    for (int c = 0; c < GRID_COLS; ++c) {
        if (grid.cells[(row*GRID_COLS)+c] != TEXTURE_CELL_BG) result--;
    }
    if (!result) return 1;
    else return 0;
}

Input game_get_input(void) {
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) return INPUT_MOVE_LEFT;
    else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) return INPUT_MOVE_RIGHT;
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) return INPUT_MOVE_FAST_DOWN;
    else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) return INPUT_ROTATE;
    return INPUT_NONE;
}

void handle_input(Game* game, Input input) {
    switch (input) {
        case INPUT_MOVE_LEFT:
            shape_move(game->grid, &game->active_shape, INPUT_MOVE_LEFT);
        break;
        case INPUT_MOVE_RIGHT:
            shape_move(game->grid, &game->active_shape, INPUT_MOVE_RIGHT);
        break;
        case INPUT_MOVE_FAST_DOWN:
            shape_fall(game, &game->active_shape);
        break;
        case INPUT_ROTATE:
            shape_rotate(game->grid, &game->active_shape);
        break;
        case INPUT_NONE:
        default:
            return;
        break;
    }
}

void handle_end_game(Game* game, int end_game_feedback) {
    if (end_game_feedback == 1) {
        game->running = 0;
    } else if (end_game_feedback == 2) {
        game_reset(game);
    } 
    return;
}

Rectangle grid_get_bounds(int grid_rows, int grid_cols, int cell_spacing, int cell_size) {
    Rectangle bounds = {0};

    bounds.width = (cell_spacing*(grid_cols+1))+(grid_cols*cell_size);
    bounds.height = (cell_spacing*(grid_rows+1))+(grid_rows*cell_size); 
    bounds.x = (WIN_WIDTH-bounds.width)/2.0f;
    bounds.y = (WIN_HEIGHT-bounds.height)/2.0f;

    return bounds;
}
