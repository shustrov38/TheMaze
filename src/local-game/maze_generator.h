#ifndef THEMAZE_MAZE_GENERATOR_H
#define THEMAZE_MAZE_GENERATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>
#include <conio.h>

#define MAZE_SIZE 20

typedef struct {
    int x;
    int y;
} pair;

int **initMaze(int **maze, int width, int height);
int **generator(int **maze, int x1, int x2, int y1, int y2);
char getCharByPattern(const int *pattern);
void printMaze(int **maze, int height, int width);


#endif //THEMAZE_MAZE_GENERATOR_H
