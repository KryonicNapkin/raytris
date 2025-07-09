#ifndef GAME_H_
#define GAME_H_

#include "textures.h"

#define WIN_WIDTH         800
#define WIN_HEIGHT        600
#define GAME_TITLE        "RayTris"

#define GRID_ROWS         20
#define GRID_COLS         10
#define CELL_SIZE         TEXTURE_SIZE
#define CELL_SPACING      4
#define GRID_BORDER_WIDTH (CELL_SPACING/2.0f)

#define SHAPES_COUNT      7
#define SHAPE_VALS        4

#define BG_COLOR          0x282C34FF

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x;
    int y;
    Texture_Id texture_id;
    Point blocks[SHAPE_VALS];
} Shape;

typedef struct {
    Rectangle bounds;
    Texture_Id cells[GRID_ROWS*GRID_COLS];
    int cell_spacing;
} Grid;

typedef struct {
    unsigned int score;
    int running;
    Shape active_shape;
    Grid grid;
} Game;

extern Point _shapes[SHAPES_COUNT][SHAPE_VALS];

void game_init(Game* game);
Grid grid_create(void);
void game_update(Game* game);
void game_draw(Game game);
void grid_draw(Grid grid);

Shape shape_create(void);
void shape_draw(Vector2 base_pos, Shape shape);
void shape_move(Grid grid, Shape* shape, int dir);
void shape_rotate(Grid grid, Shape* shape);
void shape_lock(Game* game, Shape* shape);

int check_collisions(Grid grid, int x, int y);
void grid_clear_row(Grid* grid, int row, unsigned int* new_score);
int is_row_full(Grid grid, int row);

Rectangle grid_get_bounds(int grid_rows, int grid_cols, int cell_spacing, int cell_size);

#endif /* GAME_H_ */
