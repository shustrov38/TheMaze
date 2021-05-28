#ifndef THEMAZE_PLAYER_H
#define THEMAZE_PLAYER_H

#include "maze_generator.h"
#include "utilities.h"

/*
enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};
*/



COORD action(int **maze, COORD coord, HANDLE handle);
COORD botAction(int **maze, COORD coord, HANDLE consoleHandle);
int catch();

#endif //THEMAZE_PLAYER_H
