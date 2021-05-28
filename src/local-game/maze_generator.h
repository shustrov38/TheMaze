#ifndef THEMAZE_MAZE_GENERATOR_H
#define THEMAZE_MAZE_GENERATOR_H

#include "utilities.h"

#define MAZE_SIZE 40

typedef struct {
    int x;
    int y;
} pair;

int **initMaze(int **maze, int width, int height);
int **generator(int **maze, int x1, int x2, int y1, int y2);
char getCharByPattern(const int *pattern);
void printMaze(int **maze, int height, int width);
int **createFinishPoint(int **maze, int width, int height);

#endif //THEMAZE_MAZE_GENERATOR_H
