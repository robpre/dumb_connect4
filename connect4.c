#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "getkeys.h"

#define HEIGHT 6
#define WIDTH 7

char board[HEIGHT][WIDTH] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
};

int col = 0;
int colsize = 2;
char player = 'o';
int winner = -1;
bool program_should_exit = false;

void renderBoard() {
    // system("clear");
    char prefix[] = "\t  ";

    // Print the column selector
    printf("%s ", prefix);
    for (int i = 0; i < col * colsize; i++) {
        printf(" ");
    }
    printf("V\n");

    // print piece
    printf("%s ", prefix);
    for (int i = 0; i < col * colsize; i++) {
        printf(" ");
    }
    printf("%c\n", player);

    // ascii art
    printf("%s_______________\n", prefix);
    for (int i = 0; i < HEIGHT; i++) {
        printf("%s|", prefix);
        for (int j = 0; j < WIDTH; j++) {
            printf("%c|", board[i][j] == ' ' ? '_' : board[i][j]);
        }
        printf("\n");
    }
    printf("\t /|=============|\\\n");
    printf("\t/-|-------------|-\\\n");
}

void moveRight() { if (col < WIDTH - 1) { col++; } }
void moveLeft() { if (col > 0) { col--; } }
void togglePlayer() { if (player == 'o') player = 'x'; else player = 'o';}

void placePiece() {
    for (int i = HEIGHT; i >= 0; i--) {
        char target = board[i][col];

        if (target == ' ') {
            board[i][col] = player;
            break;
        }
    }

    togglePlayer();
}

void checkWinner(int placedPieceX) {
    int placedPieceY = 0;
    for (int i = 0; i < HEIGHT; i++) {
        char check = board[i][placedPieceX];

        if (check != ' ') {
            placedPieceY = i;
            break;
        }
    }

    char row[WIDTH];
    char col[HEIGHT];
    char forwardDiag[HEIGHT];
    char backwardDiag[HEIGHT];

    // ROW
    for (int i = 0; i < WIDTH; i++) {
        row[i] = board[placedPieceY][i];
    }
    // col
    for (int i = 0; i < HEIGHT; i++) {
        printf("col '%s'\n", col);
        col[i] = board[i][placedPieceX];
    }

    printf("looking for '%s' in '%s'\n", "xxxx", row);
    printf("looking for '%s' in '%s'\n", "xxxx", col);
    if (strstr(row, "oooo") || strstr(col, "oooo")){
        winner = 1;
    }
    if (strstr(row, "xxxx") || strstr(col, "xxxx")){
        winner = 1;
    }
}

//        V
//        x
//   _______________
//   |_|_|_|_|_|_|_|
//   |_|_|_|_|_|_|_|
//   |_|_|_|x|x|_|_|
//   |_|_|_|x|o|o|_|
//   |_|_|x|o|x|o|_|
//   |_|x|o|o|o|x|_|
//  /|=============|\
// /-|-------------|-\

int main() {
    int loops = 0;
    int c, oldCol;

    while (!program_should_exit) {
        renderBoard();

        if (winner != -1) {
            printf("player %i WINS!!!!!!!!!\n", winner);
            break;
        }

        c = getKey();

        switch (c) {
        case KEY_Esc:
            program_should_exit = true;
            break;
        case KEY_Left:
            moveLeft();
            break;
        case KEY_Right:
            moveRight();
            break;
        case KEY_Space:
            oldCol = col;
            placePiece();
            checkWinner(oldCol);
            break;
        default:
            if (loops++ >= 10) {
                program_should_exit = true;
            }
            break;
        }
    }
}
