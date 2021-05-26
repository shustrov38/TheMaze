#include "game.h"
#include "maze_generator.h"
/*
Arrow Left	0x25	37
Arrow Up	0x26	38
Arrow Right	0x27	39
Arrow Down	0x28	40
*/

int catch(){
    DWORD mode, cc;
    HANDLE h = GetStdHandle( STD_INPUT_HANDLE );

    if (h == NULL) {
        return 0; // console not found
    }

    GetConsoleMode( h, &mode );
    SetConsoleMode( h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT) );
    TCHAR c = 0;
    ReadConsole( h, &c, 1, &cc, NULL );
    SetConsoleMode( h, mode );
    return c;
}

void action(int **maze, int height, int width) {
    HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;


    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    coord.Y = 1; coord.Y = 1;
    printMaze(maze, height, width);

    while(coord.X != MAZE_SIZE-1 && coord.Y != MAZE_SIZE-1) {

        char button;
        button = catch();
        int printFlag = 0;


        if (button == 'w') {
            if (maze[coord.Y - 1][coord.X] == 0) {
                SetConsoleCursorPosition(handler, coord);
                printf(" ");
                printFlag = 1;
                coord.Y--;
            }
        } else if (button == 's') {
            if (maze[coord.Y + 1][coord.X] == 0) {
                SetConsoleCursorPosition(handler, coord);
                printf(" ");
                printFlag = 2;
                coord.Y++;
            }
        } else if (button == 'a') {
            if (maze[coord.Y][coord.X - 1] == 0) {
                SetConsoleCursorPosition(handler, coord);
                printf(" ");
                printFlag = 3;
                coord.X--;
            }
        } else if (button == 'd') {
            if (maze[coord.Y][coord.X + 1] == 0) {
                SetConsoleCursorPosition(handler, coord);
                printf(" ");
                printFlag = 4;
                coord.X++;
            }
        }


        if (printFlag) {
            SetConsoleCursorPosition(handler, coord);
            printf("@");
        }

    }
}

int main() {
    srand(time(NULL));
    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;


    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);

    //printMaze(maze, height, width);
    action(maze, height, width);
}