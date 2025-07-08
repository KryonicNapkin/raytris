#ifndef GAME_H_
#define GAME_H_

#include "textures.h"

#define GAME_WIDTH  800
#define GAME_HEIGHT 600
#define GAME_TITLE  "RayTris"

#define GRID_ROWS         20
#define GRID_COLS         10
#define CELL_SIZE         TEXTURE_SIZE
#define CELL_SPACING      4
#define GRID_BORDER_WIDTH (CELL_SPACING/2.0f)

#define SHAPES_COUNT      7
#define SHAPE_VALS        4

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
    Vector2 pos;
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
void game_update(Game* game);
void game_draw(Game game);
void grid_draw(Grid grid);

void shape_create(Shape* shape);
void shape_draw(Vector2 base_pos, Shape shape);
void shape_move(Grid grid, Shape* shape, int dir);
void shape_rotate(Grid grid, Shape* shape);
void shape_lock(Game* game, Shape* shape);

int check_collisions(Grid grid, int x, int y);
void grid_clear_row(Grid* grid, int row, unsigned int* new_score);
int is_row_full(Grid grid, int row);

#endif /* GAME_H_ */
