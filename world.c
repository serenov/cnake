// State Machine

#include <stdio.h>
#include <stdbool.h>

#include "world.h"
#include "utils.h"

static World WORLD_INST;


Coordinates DIRECTION_OFFSETS[] = {
    (Coordinates) { 0,  0}, // Neutral
    (Coordinates) { 0,  1}, // North 
    (Coordinates) { 0, -1}, // South 
    (Coordinates) { 1,  0}, // East
    (Coordinates) {-1,  0}  // West 
};

bool boundary_condition(int x, int y) {
    // For now the boundary being generated by a geometrical condition

    return (x == 0) || (y == 0) || (x == X_SIZE - 1) || (y == Y_SIZE - 1);
}


bool init_world() {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {

            WORLD_INST.cell_array[y][x] = boundary_condition(x, y)? BoundaryElement: Empty;
        }
    }   

    const int area = (X_SIZE - 1) * (Y_SIZE - 1);

    if (area < 0) {
        // We don't have enough space to put a snake in the world.

        printf("Game cannot progress further, due to lack of space to put snake in.");

        return false;
    }

    const int x_snake_spawn = X_SIZE / 2;
    const int y_snake_spawn = Y_SIZE / 2;

    WORLD_INST.cell_array[y_snake_spawn][x_snake_spawn] = SnakeElement;
    WORLD_INST.snake_direction_array[y_snake_spawn][x_snake_spawn] = East;

    WORLD_INST.head_coordinates = (Coordinates) {x_snake_spawn, y_snake_spawn};
    WORLD_INST.tail_coordinates = WORLD_INST.head_coordinates;

    WORLD_INST.cell_array[1][1] = FoodElement; 

    return true;
}

Coordinates translate_coordinates(Coordinates current_coords, Direction direction) {

    Coordinates new_coordinates = 
    {
        current_coords.x + DIRECTION_OFFSETS[direction].x,
        current_coords.y + DIRECTION_OFFSETS[direction].y
    };

    return new_coordinates;
}

void update_world_state(Direction direction_input) {

    Coordinates offset = DIRECTION_OFFSETS[direction_input];

    Cell element = WORLD_INST.cell_array[WORLD_INST.head_coordinates.y + offset.y][WORLD_INST.head_coordinates.x + offset.x];

    if(element == SnakeElement || element == BoundaryElement) {
        // Game Over condition

        WORLD_INST.game_state = Ended;

        return;
    }
    else if (element == FoodElement) {
        // When my snake (;P) hits food element. 
        // code repetition but i will bear it.


        WORLD_INST.snake_direction_array[WORLD_INST.head_coordinates.y][WORLD_INST.head_coordinates.x] = direction_input;

        Direction head_direction = WORLD_INST.snake_direction_array[WORLD_INST.head_coordinates.y][WORLD_INST.head_coordinates.x];
        Coordinates new_head_coords = translate_coordinates(WORLD_INST.head_coordinates, head_direction);

        WORLD_INST.cell_array[new_head_coords.y][new_head_coords.x] = SnakeElement;

        WORLD_INST.head_coordinates = new_head_coords;


        bool has_game_ended = true;

        for(int y = 0; y < Y_SIZE; y++){
            for(int x = 0; x < X_SIZE; x++) {
                if(WORLD_INST.cell_array[y][x] == Empty) {
                    has_game_ended = false;
                    break;
                }
            }
        }

        if (has_game_ended) {
            WORLD_INST.game_state = Won;

            return;
        }

        Coordinates random_coordinates;

        while(true) {
            random_coordinates = generate_random_coordinates(X_SIZE, Y_SIZE);

            if(WORLD_INST.cell_array[random_coordinates.y][random_coordinates.x] == Empty) break;
        }

        WORLD_INST.cell_array[random_coordinates.y][random_coordinates.x] = FoodElement; // calculate the new position for food.

        return;
    }

    // Empty Cell

    WORLD_INST.snake_direction_array[WORLD_INST.head_coordinates.y][WORLD_INST.head_coordinates.x] = direction_input;

    for(int y = 0; y < Y_SIZE; y++) {
        for(int x = 0; x < X_SIZE; x++) {
            Direction current_direction = WORLD_INST.snake_direction_array[y][x]; 

            if(current_direction != Neutral) {
                Coordinates destination_coords = translate_coordinates((Coordinates){x, y}, current_direction);

                WORLD_INST.cell_array[destination_coords.y][destination_coords.x] = SnakeElement;
            }
        }
    }

    WORLD_INST.cell_array[WORLD_INST.tail_coordinates.y][WORLD_INST.tail_coordinates.x] = Empty;

    Direction head_direction = WORLD_INST.snake_direction_array[WORLD_INST.head_coordinates.y][WORLD_INST.head_coordinates.x];
    Direction tail_direction = WORLD_INST.snake_direction_array[WORLD_INST.tail_coordinates.y][WORLD_INST.tail_coordinates.x];

    WORLD_INST.snake_direction_array[WORLD_INST.tail_coordinates.y][WORLD_INST.tail_coordinates.x] = Neutral;

    Coordinates new_head_coords = translate_coordinates(WORLD_INST.head_coordinates, head_direction);
    Coordinates new_tail_coords = translate_coordinates(WORLD_INST.tail_coordinates, tail_direction);


    WORLD_INST.head_coordinates = new_head_coords;
    WORLD_INST.tail_coordinates = new_tail_coords;
}

void print_world_state() {

    for(int y = 0; y < Y_SIZE; y++) {
        for(int x = 0; x < X_SIZE; x++) {

            switch (WORLD_INST.cell_array[y][x])
            {
            case SnakeElement:
                printf("X");
                break;
            case BoundaryElement:
                printf("#");
                break;
            case FoodElement:
                printf("O");
                break;
            case Empty:
                printf(" ");
                break;

            default:
                break;
            }
        }

        printf("\n");
    }
}

Direction interpret_direction(char c) {

    switch (c)
    {
    case 'e':
        return East;
    case 'w':
        return West; 
    case 'n':
        return North;
    case 's':
        return South; 

    default:
        return West; 
    }
}

Direction get_opposite_direction(Direction input) {

    switch (input)
    {
    case West:
        return East;
    case South:
        return North;
    case North:
        return South;
    case East:
        return West;
   
    default:
        return West;
    }
}


void game_loop() {

    init_world();

    Direction current_direction = West;

    while(WORLD_INST.game_state != Ended && WORLD_INST.game_state != Won) {

        print_world_state();

        char input = getchar();

        Direction input_direction = interpret_direction(input);

        if(input_direction != get_opposite_direction(current_direction)) {

            current_direction = input_direction;
        }



        if(input == 'e' || input == 'w' || input == 's' || input == 'n') {
            printf("input: %c \n", input);

            update_world_state(current_direction);
        }

    }
}