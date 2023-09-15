// Purpose: C Programming - Assignment 2
// Date: 22/04/2023
// Name: Jamie Roche
// Student Number: R00151829
// Class: SDH2-B

#include "asteroids.h"

struct ship_action move_ship(int field[FIELD_HEIGHT][FIELD_WIDTH], void *state) {
    struct ship_action action; // Struct of type ship_action that stores the action the ship will take
    int ship_row; // Stores the row of the ship's current position
    int ship_col; // Stores the column of the ship's current position
    int asteroid_col; // Stores the column of the nearest asteroid
    int closest_col = -1; // Stores the column of the nearest asteroid to the ship (-1 if there are no asteroids)
    int closest_distance = FIELD_WIDTH; // Stores the distance between the nearest asteroid and the ship
    int distance_above = FIELD_WIDTH; // Stores the distance between the nearest asteroid in the line above the ship
    int distance_below = FIELD_WIDTH; // Stores the distance between the nearest asteroid in the line below the ship
    int i; // Used as a loop counter for iterating through rows
    int j; // Used as a loop counter for iterating through columns
    int no_row_without_asteroids = -1; // initialise to -1 to indicate no row without asteroids

    // Inspect the matrix to identify the position of the ship and the obstacles
    for (i = 0; i < FIELD_HEIGHT; i++) { // loop through the rows

        int has_asteroids = -1; // Initialise a flag to indicate whether the current row has asteroids or not

        for (j = 0; j < FIELD_WIDTH; j++) { // Loop through the columns of the current row

            if (field[i][j] == SHIP_VAL) { // If the current element is the ship
                ship_row = i; // Store the row of the ship
                ship_col = j; // Store the column of the ship

            } else if (field[i][j] == ASTEROID_VAL) { // If the current element is an asteroid
                asteroid_col = j; // Store the column of the asteroid
                int distance = asteroid_col - ship_col; // Calculate the distance between the asteroid and the ship

                if (distance < closest_distance) { // If the distance is less than the current closest distance
                    closest_distance = distance; // Update the closest distance
                    closest_col = asteroid_col; // Update the closest column
                }

                // If the asteroid is within the ship's width and to the right of the ship
                if (abs(ship_col - asteroid_col) <= SHIP_WIDTH && asteroid_col > ship_col) {

                    // If the asteroid is above the ship and closer to the ship than any previous asteroid above it
                    if (i < ship_row && ship_row - i < distance_above) {
                        distance_above = ship_row - i; // Update the distance to the asteroid above

                    // If the asteroid is below the ship and closer to the ship than any previous asteroid below it
                    } else if (i > ship_row && i - ship_row < distance_below) {
                        distance_below = i - ship_row; // Update the distance to the asteroid below
                    }
                }
                has_asteroids = 1; // Set flag to true if there are any asteroids in the matrix
            }
        }

        // Check if current row has no asteroids and no previous row has been found without asteroids
        if (has_asteroids == -1 && no_row_without_asteroids == -1) {
            no_row_without_asteroids = i; // Store the row index of the current row with no asteroids
        }
    }

    // Implement a greedy path planning algorithm to move the ship to the lane where the first asteroid is farthest

    // Check if there is a row without asteroids and move the ship there
    if (no_row_without_asteroids != -1) {

        // If the row with no asteroids is above the ship, move up
        if (no_row_without_asteroids < ship_row) {
            action.move = MOVE_UP;

        // If the row with no asteroids is below the ship, move down
        } else if (no_row_without_asteroids > ship_row) {
            action.move = MOVE_DOWN;

        // If the ship is already on the row with no asteroids, do not move
        } else {
            action.move = MOVE_NO;
        }
    }

    // If there are no rows without asteroids, check the position of the closest asteroid

    // If there are no asteroids in the matrix, do not move
    else if (closest_col == -1) {
        action.move = MOVE_NO;

    // If the closest asteroid is in the same column as the ship, do not move
    } else if (closest_col == 0) {
        action.move = MOVE_NO;

    // If the closest asteroid is too close to the ship, move up
    } else if (closest_col > 0 && closest_col <= SHIP_WIDTH) {
        action.move = MOVE_UP;

    // If the closest asteroid is farther away and within the field's width, check which line has more space
    } else if (closest_col > SHIP_WIDTH != 0) {

        // If the line above has more space, move up
        if (distance_above < distance_below) {
            action.move = MOVE_UP;

        // If the line below has more space, move down
        } else if (distance_below < distance_above) {
            action.move = MOVE_DOWN;

        // If both lines have the same amount of space, stay in the same lane
        } else {
            action.move = MOVE_NO;
        }

    // If the closest asteroid is past the field, do not move
    } else {
        action.move = MOVE_NO;
    }

    // Set state to 0
    action.state = 0;

    // Return the action to be taken by the ship
    return action;
}