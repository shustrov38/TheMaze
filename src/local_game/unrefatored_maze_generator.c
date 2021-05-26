#include "utilities.h"
#include "maze_generator.h"



//height - высота
//width - ширина



int **initMaze(int **maze, int width, int height) {
    maze = (int **) malloc(width * sizeof(int *));
    for (int i = 0; i < width; ++i) {
        maze[i] = (int *) malloc(height * sizeof(int));
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                maze[i][j] = 1;
            } else
                maze[i][j] = 0;
        }
    }

    return maze;
}


int **generator(int **maze, int x1, int x2, int y1, int y2) {

    int cantSetXwallFlag = 0;  //стена с фиксированным x
    int cantSetYwallFlag = 0;  //стена с фиксированным y, если на пути стены проход, и эта координата единственная возможная для стены то не ставим стену
    if (x2 - x1 >= 4 && y2 - y1 >= 4) {

        pair pos;
        if (x2 - x1 <= 6) {
            pos.x = rand() % (x2 - x1 - 3) + x1 + 2;
        } else pos.x = (x2 - x1 - 3) / 2 + x1 + 2;

        if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
            if (x2 - x1 < 4) {
                cantSetXwallFlag = 1;
            } else {
                while (x2 - pos.x > 2) {
                    pos.x++;
                    if (!(maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2)) {
                        break;
                    }
                }
                if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
                    while (pos.x - x1 > 2) {
                        pos.x--;
                        if (!(maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2)) {
                            break;
                        }
                    }
                }
            }
        }
        if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
            cantSetXwallFlag = 1;
        }
        if (y2 - y1 <= 6) {
            pos.y = rand() % (y2 - y1 - 3) + y1 + 2;
        } else pos.y = (y2 - y1 - 3) / 2 + y1 + 2;


        if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
            if (y2 - y1 < 4) {
                cantSetYwallFlag = 1;
            } else {
                while (y2 - pos.y > 2) {
                    pos.y++;
                    if (!(maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2)) {
                        break;
                    }
                }
                if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
                    while (pos.y - y1 > 2) {
                        pos.y--;
                        if (!(maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2)) {
                            break;
                        }
                    }
                }
            }

        }
        if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
            cantSetYwallFlag = 1;
        }

        pair hole;
        hole.x = rand() % (pos.x - x1 - 1) + x1 + 1;        //по x слева пересечения
        hole.y = rand() % (x2 - pos.x - 1) + pos.x + 1;     //по x справа пересечения


        if (!cantSetYwallFlag) {
            for (int i = x1; i < x2; ++i) {
                maze[i][pos.y] = 1;
            }
            maze[hole.x][pos.y] = 2;
            maze[hole.y][pos.y] = 2;

        }


        hole.x = rand() % (pos.y - y1 - 1) + y1 + 1;             //по y сверху пересечения
        hole.y = rand() % (y2 - pos.y - 1) + pos.y + 1;     //по y снизу пересечения

        if (!cantSetXwallFlag) {
            for (int i = y1; i < y2; ++i) {
                maze[pos.x][i] = 1;
            }
            maze[pos.x][hole.x] = 2;
            maze[pos.x][hole.y] = 2;

        }


        maze = generator(maze, x1, pos.x, y1, pos.y);
        maze = generator(maze, x1, pos.x, pos.y, y2);
        maze = generator(maze, pos.x, x2, y1, pos.y);
        maze = generator(maze, pos.x, x2, pos.y, y2);

    } else if (x2 - x1 >= 4 && y2 - y1 < 4) {

        pair pos;
        pair hole;
        if (x2 - x1 <= 6) {
            pos.x = rand() % (x2 - x1 - 3) + x1 + 2;
        } else {
            pos.x = (x2 - x1 - 3) / 2 + x1 + 2;
        }

        if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
            if (x2 - x1 < 4) {
                cantSetXwallFlag = 1;
            } else {
                while (x2 - pos.x > 2) {
                    pos.x++;
                    if (!(maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2)) {
                        break;
                    }
                }
                if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
                    while (pos.x - x1 > 2) {
                        pos.x--;
                        if (!(maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2)) {
                            break;
                        }
                    }
                }
            }
        }
        if (maze[pos.x][y1] == 2 || maze[pos.x][y2] == 2) {
            cantSetXwallFlag = 1;
        }

        hole.y = rand() % (y2 - y1 - 1) + y1 + 1;
        if (!cantSetXwallFlag) {
            for (int i = y1; i < y2; ++i) {
                maze[pos.x][i] = 1;
            }
            maze[pos.x][hole.y] = 2;

        }

        maze = generator(maze, x1, pos.x, y1, y2);
        maze = generator(maze, pos.x, x2, y1, y2);


    } else if (y2 - y1 >= 4 && x2 - x1 < 4) {
        pair pos;
        pair hole;
        if (y2 - y1 <= 6) {
            pos.y = rand() % (y2 - y1 - 3) + y1 + 2;
        } else pos.y = (y2 - y1 - 3) / 2 + y1 + 2;

        if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
            if (y2 - y1 < 4) {
                cantSetYwallFlag = 1;
            } else {
                while (y2 - pos.y > 2) {
                    pos.y++;
                    if (!(maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2)) {
                        break;
                    }
                }
                if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
                    while (pos.y - y1 > 2) {
                        pos.y--;
                        if (!(maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2)) {
                            break;
                        }
                    }
                }
            }

        }

        if (maze[x1][pos.y] == 2 || maze[x2][pos.y] == 2) {
            cantSetYwallFlag = 1;
        }
        hole.x = rand() % (x2 - x1 - 1) + x1 + 1;
        if (!cantSetYwallFlag) {
            for (int i = x1; i < x2; ++i) {
                maze[i][pos.y] = 1;
            }
            maze[hole.x][pos.y] = 2;

        }

        maze = generator(maze, x1, x2, y1, pos.y);
        maze = generator(maze, x1, x2, pos.y, y2);
    } else {
        return maze;
    }

}


char getCharByPattern(const int *pattern) {
    if (!pattern[2]) return ' ';

    int val = 0;
    for (int i = 0; i < 5; ++i) {
        val += (1 << (4 - i)) * pattern[i];
    }

    if (val == 31) return 206;
    if (val == 30) return 202;
    if (val == 29) return 185;
    if (val == 28) return 188;

    if (val == 23) return 204;
    if (val == 22) return 200;
    if (val == 21) return 186;
    if (val == 20) return 186; // тупик сверху вниз

    if (val == 15) return 203;
    if (val == 14) return 205;
    if (val == 13) return 187;
    if (val == 12) return 205; // тупик слева направо
    if (val == 11) return 203;

    if (val == 7) return 201;
    if (val == 6) return 205; // тупик справа налево
    if (val == 5) return 186; // тупик снизу вверх
    if (val == 4) return 254; // точка в центре

    return ' ';
}


const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

void go(int **used, int size, int x, int y, int *flag) {
    if (*flag) return;
    used[x][y] = 2;
    for (int i = 0; i < 4; ++i) {
        int xi = x + dx[i], yi = y + dy[i];
        if (0 <= xi && xi < size && 0 <= yi && yi < size && !used[xi][yi]) {
            if (xi == 1 && yi == 1) {
                *flag = 1;
                used[xi][yi] = 2;
                return;
            }
            go(used, size, xi, yi, flag);
        }
    }
}


void printMaze(int **maze, int height, int width) {
    int **used;
    used = initMaze(used, width, height);

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (maze[i][j] == 2) {
                maze[i][j] = 0;
            }
            used[i][j] = maze[i][j];
        }
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (maze[i][j] == 2) {
                printf(".");
                continue;
            }


            //  0
            // 123
            //  4
            int pattern[5] = {0, 0, maze[i][j] == 1 ? 1 : 0, 0, 0};

            if (i - 1 >= 0) pattern[0] = maze[i - 1][j] == 1 ? 1 : 0;
            if (j - 1 >= 0) pattern[1] = maze[i][j - 1] == 1 ? 1 : 0;
            if (j + 1 < height) pattern[3] = maze[i][j + 1] == 1 ? 1 : 0;
            if (i + 1 < width) pattern[4] = maze[i + 1][j] == 1 ? 1 : 0;

            printf("%c", getCharByPattern(pattern));
        }
        printf("\n");
    }


#ifdef __DFS_DEBUG__
    go(maze, SIZE, width - 2, height - 2, &flag);
#endif
}
