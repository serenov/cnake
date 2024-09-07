#include<stdlib.h>
#include<time.h>

#include "utils.h"
#include"world.h"

Coordinates generate_random_coordinates(int x_range, int y_range) {
    // Initialize the random number generator
    srand(time(NULL));

    Coordinates randomCoords;

    // Generate two random numbers within the range [0, range)
    randomCoords.x = rand() % x_range;
    randomCoords.y = rand() % y_range;

    return randomCoords;
}