#include "game.h"

playerInfo *initPlayerInfo(char **name, int count, int *id) {
    playerInfo *players = (playerInfo *) malloc(count * sizeof(playerInfo));
    int k = 1;

    for (int i = 0; i < count; ++i){
        players[i].id = id[i];

        players[i].name = (char*) malloc(100 * sizeof(char));
        strcpy(players[i].name, name[i]);

        if (i == 0){
            players[i].coord.X = 1;
            players[i].coord.Y = 1;
        }
        if (i == 1){
            players[i].coord.X = MAZE_SIZE - 2;
            players[i].coord.Y = 1;
        }
        if (i == 2){
            players[i].coord.Y = MAZE_SIZE - 2;
            players[i].coord.X = 1;
        }
        if (i == 3){
            players[i].coord.X = MAZE_SIZE - 2;
            players[i].coord.Y = MAZE_SIZE - 2;
        }

        if (i == 0){
            players[i].color = 2;
        }
        if (i == 1){
            players[i].color = 4;
        }
        if (i == 2){
            players[i].color = 14;
        }
        if (i == 3){
            players[i].color = 3;
        }

        k++;
    }

    return players;
}

int main() {
    system("cls");
    srand(time(NULL));

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);


//maze generation
    int height = MAZE_SIZE;
    int width = MAZE_SIZE;
    int **maze;

    maze = initMaze(maze, width, height);
    maze = generator(maze, 0, width - 1, 0, height - 1);

    maze = createFinishPoint(maze, width, height);
//maze generation


//players generation
    char **names;
    int *id;
    int playersCount;

    printf("Enter names count - ");
    scanf("%d", &playersCount);


    names = (char**) malloc(playersCount * sizeof(char*));
    for (int i = 0; i < playersCount; ++i){
        names[i] = (char*) malloc(50 * sizeof(char));
        printf("\nEnter name: ");
        scanf("%s", names[i]);
    }

    id = (int*) malloc(playersCount * sizeof(int));
    for (int i = 0; i < playersCount; ++i){
        printf("\nEnter id: ");
        scanf("%d", &id[i]);
    }

    playerInfo *players = initPlayerInfo(names, playersCount, id);
//players generation


//game
    printMaze(maze, height, width);
    int winFlag = -1;

    for (int i = 0; i < playersCount; ++i) {
        SetConsoleCursorPosition(consoleHandle, players[i].coord);
        SetConsoleTextAttribute(consoleHandle, (WORD) ((0 << 4) | players[i].color));
        printf("@");
    }


    while(1){
        players[0].coord = action(maze, players[0].coord, consoleHandle);
        SetConsoleCursorPosition(consoleHandle, players[0].coord);
        SetConsoleTextAttribute(consoleHandle, (WORD) ((0 << 4) | players[0].color));
        printf("@");



        for (int i = 0; i < playersCount; ++i){

            if (maze[players[i].coord.Y][players[i].coord.X] == 5){
                system("cls");
                winFlag = i;
                break;
            }

        }

        if (winFlag != -1){
            break;
        }
    }

    SetConsoleTextAttribute(consoleHandle, (WORD) ((0 << 4) | 15));
    printf("Winner is PLAYER %s", players[winFlag].name);

//game

}