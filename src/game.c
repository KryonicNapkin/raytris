#include "game.h"
#include "raylib.h"

#include <string.h>

Point _shapes[SHAPES_COUNT][SHAPE_VALS] = {
    {{0, 0}, {1, 0},  {0, 1},  {1, 1}},  // I BLOCK
    {{0, 0}, {-1, 0}, {1, 0},  {2, 0}},  // O BLOCK
    {{0, 0}, {1, 0},  {-1, 0}, {0, 1}},  // T BLOCK
    {{0, 0}, {0, -1}, {0, 1},  {1, 1}},  // L BLOCK
    {{0, 0}, {0, -1}, {0, 1},  {-1, 1}}, // J BLOCK
    {{0, 0}, {-1, 0}, {0, 1},  {1, 1}},  // S BLOCK
    {{0, 0}, {0, 1},  {-1, 1},  {1, 0}}, // Z BLOCK
};

void game_init(Game* game) {
    game->grid = grid_create();
    game->active_shape = shape_create();
    game->running = 1;
    game->score = 0;
}

Grid grid_create(void) {
    Grid grid = {0};
    grid.bounds = grid_get_bounds(GRID_ROWS, GRID_COLS, CELL_SPACING, CELL_SIZE); 
    grid.cell_spacing = CELL_SPACING;
    memset(&grid.cells, TEXTURE_CELL_BG, (GRID_ROWS*GRID_COLS) * sizeof(Texture_Id));
    return grid;
};

void game_update(Game* game) {
    game->active_shape.y += 1;
}

void game_draw(Game game) {
    grid_draw(game.grid);
    Vector2 base_pos = {
        .x = game.grid.bounds.x+game.grid.cell_spacing,
        .y = game.grid.bounds.y+game.grid.cell_spacing,
    };
    shape_draw(base_pos, game.active_shape);
}

void grid_draw(Grid grid) {
    Vector2 pos = {grid.bounds.x+grid.cell_spacing, grid.bounds.y+grid.cell_spacing};
    Vector2 cell_pos = pos;
    for (int i = 0; i < GRID_ROWS*GRID_COLS; ++i) {
/*         TraceLog(LOG_INFO, "x: %d, y: %d", (int)cell_pos.x, (int)cell_pos.y); */
        DrawTextureRec(_tex_atlas, _tex_rects[grid.cells[i]], cell_pos, WHITE);
        cell_pos.x += (grid.cell_spacing+CELL_SIZE);
        if ((i+1) % GRID_COLS == 0) {
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

Shape shape_create(void) {
    Shape shape = {0};
    shape.x = GRID_COLS/2;
    shape.y = 0;
    shape.texture_id = GetRandomValue(1, TEXTURE_ZBLOCK-1);
    memcpy(shape.blocks, _shapes[shape.texture_id], SHAPE_VALS * sizeof(Point));
    return shape;
}

void shape_draw(Vector2 base_pos, Shape shape) {
    for (int i = 0; i < SHAPE_VALS; ++i) {
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
    for (int i = 0; i < SHAPE_VALS; ++i) {
        temp.x = shape->blocks[i].x+shape->x+dir;
        temp.y = shape->blocks[i].y+shape->y;
        if (check_collisions(grid, temp.x, temp.y)) return;
    }
    shape->x += dir;
}

void shape_rotate(Grid grid, Shape* shape) {
    if (shape->texture_id == TEXTURE_OBLOCK) return; 

    Point temp[SHAPE_VALS] = {0};

    for (int i = 0; i < SHAPE_VALS; ++i) {
        temp[i].x = shape->blocks[i].y;
        temp[i].y = -shape->blocks[i].x;
        if (check_collisions(grid, temp[i].x+shape->x, temp[i].y+shape->y)) return;
    }
    for (int x = 0; x < SHAPE_VALS; ++x) {
        shape->blocks[x] = temp[x];
    }
}

void shape_lock(Game* game, Shape* shape) {
    Point temp = {0};

    int destroy_rows[SHAPE_VALS];
    int destroy_rows_size = 0;
    int rows_full = 0;

    for (int i = 0; i < SHAPE_VALS; ++i) {
        temp.x = shape->blocks[i].x+shape->x;
        temp.y = shape->blocks[i].y+shape->y;

        if (temp.x >= 0 && temp.x < GRID_COLS && temp.y >= 0 && temp.y < GRID_ROWS) {
            game->grid.cells[(temp.y*GRID_ROWS)+temp.x] = shape->texture_id;
        }
        if (is_row_full(game->grid, temp.y)) {
            destroy_rows[destroy_rows_size++] = temp.y;
            rows_full = 1;
        }
    }
    if (rows_full) {
        for (int x = 0; x < destroy_rows_size; ++x) {
            grid_clear_row(&game->grid, destroy_rows[x], &game->score);
        }
    }
    game->score += 1;
    game->active_shape = shape_create();
}

int check_collisions(Grid grid, int x, int y) {
    if (x < 0 || x >= GRID_COLS) return 1;
    else if (y >= GRID_ROWS) return 1;
    else if (grid.cells[(y*GRID_ROWS)+x]) return 1;
    else return 0;
}

void grid_clear_row(Grid* grid, int row, unsigned int* new_score) {
    if (row < 0 || row >= GRID_ROWS) return;

    for (int r = row; r < GRID_ROWS; --r) {
        for (int c = 0; c < GRID_COLS; ++c) {
            grid->cells[(r*GRID_ROWS)+c] = grid->cells[((r-1)*GRID_ROWS)+c];
        }
    }
    if (new_score != NULL) *new_score += 50; 
}

int is_row_full(Grid grid, int row) {
    if (row < 0 || row >= GRID_ROWS) return -1;

    int result = GRID_COLS;
    for (int c = 0; c < GRID_COLS; ++c) {
        if (grid.cells[(row*GRID_ROWS)+c]) result--;
    }
    if (!result) return 1;
    else return 0;
}

Rectangle grid_get_bounds(int grid_rows, int grid_cols, int cell_spacing, int cell_size) {
    Rectangle bounds = {0};

    bounds.width = (cell_spacing*(grid_cols+1))+(grid_cols*cell_size);
    bounds.height = (cell_spacing*(grid_rows+1))+(grid_rows*cell_size); 
    bounds.x = (WIN_WIDTH-bounds.width)/2.0f;
    bounds.y = (WIN_HEIGHT-bounds.height)/2.0f;

    return bounds;
}
