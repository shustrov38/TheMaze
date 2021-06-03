#include "player.h"
#include <windows.h>

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

COORD action(int **maze, COORD coord, HANDLE consoleHandle) {

    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    int printFlag = 0;

    char button;
    button = catch();


    if (button == 'w') {
        if (maze[coord.Y - 1][coord.X] == 0 || maze[coord.Y - 1][coord.X] == 5) {
            printFlag = 1;
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.Y--;
        }
    } else if (button == 's') {
        if (maze[coord.Y + 1][coord.X] == 0 || maze[coord.Y + 1][coord.X] == 5) {
            printFlag = 1;
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.Y++;
        }
    } else if (button == 'a') {
        if (maze[coord.Y][coord.X - 1] == 0 || maze[coord.Y][coord.X - 1] == 5) {
            printFlag = 1;
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.X--;
        }
    } else if (button == 'd') {
        if (maze[coord.Y][coord.X + 1] == 0 || maze[coord.Y][coord.X + 1] == 5) {
            printFlag = 1;
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.X++;
        }
    }

    return coord;
}


COORD botAction(int **maze, COORD coord, HANDLE consoleHandle){
    srand(time(NULL));
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    int button;
    int printFlag = 0;
    button = catch();

    button = rand() % 4;

    if (button == 0) {
        if (maze[coord.Y - 1][coord.X] == 0 || maze[coord.Y - 1][coord.X] == 5) {
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.Y--;
        }
    } else if (button == 1) {
        if (maze[coord.Y + 1][coord.X] == 0 || maze[coord.Y + 1][coord.X] == 5) {
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.Y++;
        }
    } else if (button == 2) {
        if (maze[coord.Y][coord.X - 1] == 0 || maze[coord.Y][coord.X - 1] == 5) {
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.X--;
        }
    } else if (button == 3) {
        if (maze[coord.Y][coord.X + 1] == 0 || maze[coord.Y][coord.X + 1] == 5) {
            SetConsoleCursorPosition(consoleHandle, coord);
            printf(" ");
            coord.X++;
        }
    }

    return coord;
}