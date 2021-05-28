#include "game.h"

playerInfo *initPlayerInfo(char **name, int count, int *id) {
    playerInfo *players = (playerInfo *) malloc(count * sizeof(playerInfo));
    int k = 1;

    for (int i = 0; i < count; ++i){
        players[i].id = id[i];

        players[i].name = (char*) malloc(100 * sizeof(char));
        strcpy(players[i].name, name[i]);

        if (k == 1){
            players[i].coord.X = 1;
            players[i].coord.Y = 1;
        }
        if (k == 2){
            players[i].coord.X = MAZE_SIZE - 2;
            players[i].coord.Y = 1;
        }
        if (k == 3){
            players[i].coord.Y = MAZE_SIZE - 2;
            players[i].coord.X = 1;
        }
        if (k == 4){
            players[i].coord.X = MAZE_SIZE - 2;
            players[i].coord.Y = MAZE_SIZE - 2;
        }

        players[i].color = k;

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
    int namesCount;

    printf("Enter names count - ");
    scanf("%d", &namesCount);


    names = (char**) malloc(namesCount * sizeof(char*));
    for (int i = 0; i < namesCount; ++i){
        names[i] = (char*) malloc(50 * sizeof(char));
        printf("\nEnter name: ");
        scanf("%s", names[i]);
    }

    id = (int*) malloc(namesCount * sizeof(int));
    for (int i = 0; i < namesCount; ++i){
        printf("\nEnter id: ");
        scanf("%d", &id[i]);
    }

    playerInfo *players = initPlayerInfo(names, namesCount, id);
//players generation


//game
    printMaze(maze, height, width);
    int winFlag = -1;
    SetConsoleCursorPosition(consoleHandle, players[0].coord);
    printf("@");


    while(1){
        players[1].coord = botAction(maze, players[1].coord, consoleHandle);
        SetConsoleCursorPosition(consoleHandle, players[1].coord);
        SetConsoleTextAttribute(consoleHandle, (WORD) ((0 << 4) | players[1].color));
        printf("@");

        players[0].coord = action(maze, players[0].coord, consoleHandle);
        SetConsoleCursorPosition(consoleHandle, players[0].coord);
        SetConsoleTextAttribute(consoleHandle, (WORD) ((0 << 4) | players[0].color));
        printf("@");



        for (int i = 0; i < namesCount; ++i){
            if (maze[players[1].coord.Y][players[1].coord.X] == 5){
                system("cls");
                winFlag = 1;
                break;
            }

            if (maze[players[0].coord.Y][players[0].coord.X] == 5){
                system("cls");
                winFlag = 0;
                break;
            }


            //more players
        }
        if (winFlag != -1){
            break;
        }
    }

    printf("Winner is PLAYER %d", winFlag);

//game

}