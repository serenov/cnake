#pragma once

#define X_SIZE 40
#define Y_SIZE 40

#define SIZE (X_SIZE * Y_SIZE)

typedef enum {
    Empty,
    SnakeElement,
    FoodElement,
    BoundaryElement
} Cell;

typedef enum {
    Neutral,
    North,
    South,
    East,
    West,
} Direction;

typedef enum {
    Running,
    Won,
    Ended
} GameState;

typedef struct {
    int x;
    int y;
} Coordinates;

typedef struct {
    Cell cell_array[Y_SIZE][X_SIZE];

    Direction snake_direction_array[Y_SIZE][X_SIZE];

    Coordinates head_coordinates;
    Coordinates tail_coordinates;
    Coordinates food_coordinates;

    GameState game_state;
} World;


void game_loop(); 