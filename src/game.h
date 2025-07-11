#ifndef GAME_H_
#define GAME_H_

#include "graphics.h"

#define WIN_WIDTH         800
#define WIN_HEIGHT        600
#define GAME_TITLE        "RayTris"

#define GRID_ROWS         20
#define GRID_COLS         10
#define CELL_SIZE         TEXTURE_SIZE
#define CELL_SPACING      2
#define GRID_BORDER_WIDTH (CELL_SPACING/2.0f)

#define SHAPES_COUNT      7
#define SHAPE_BLOCKS      4

#define BG_COLOR          0x282C34FF

#define GAME_UPDATE_FREQ  0.75f 

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    INPUT_NONE = 0,
    INPUT_MOVE_LEFT = -1,
    INPUT_MOVE_RIGHT = 1,
    INPUT_MOVE_FAST_DOWN = 2,
    INPUT_ROTATE = 3,
} Input;

typedef enum {
    STATE_IN_GAME = 0,
    STATE_GAME_OVER,
} State;

typedef struct {
    int x;
    int y;
    Texture_Id texture_id;
    Point blocks[SHAPE_BLOCKS];
} Shape;

typedef struct {
    Rectangle bounds;
    Texture_Id cells[GRID_ROWS*GRID_COLS];
    int cell_spacing;
} Grid;

typedef struct {
    unsigned int score;
    unsigned int play_time_s;
    State state;
    bool running;
    Shape active_shape;
    Grid grid;
} Game;

extern Point _shapes[SHAPES_COUNT][SHAPE_BLOCKS];

void game_init(Game* game);
Grid grid_create(void);
void game_update(Game* game);
void game_reset(Game* game);
void game_draw(Game game);
void grid_draw(Grid grid);

Shape shape_create(Grid grid, int* result);
void shape_draw(Vector2 base_pos, Shape shape);
void shape_move(Grid grid, Shape* shape, int dir);
void shape_rotate(Grid grid, Shape* shape);
void shape_lock(Game* game, Shape* shape);
void shape_fall(Game* game, Shape* shape);

int check_collisions(Grid grid, int x, int y);
void grid_clear_row(Grid* grid, int row, unsigned int* new_score);
int is_row_full(Grid grid, int row);

Input game_get_input(void);
void handle_input(Game* game, Input input);
void handle_end_game(Game* game, int end_game_feedback);

Rectangle grid_get_bounds(int grid_rows, int grid_cols, int cell_spacing, int cell_size);

#endif /* GAME_H_ */
