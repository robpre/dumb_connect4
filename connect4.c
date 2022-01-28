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

enum BoardState {
    STATE_Empty,
    STATE_Player1,
    STATE_Player2,
};
int board[HEIGHT][WIDTH] = {
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
    {STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty, STATE_Empty},
};

int col = 0;
int colsize = 2;
char player = STATE_Player1;
int winner = -1;
bool program_should_exit = false;

char mapState(enum BoardState bs) {
    switch(bs) {
        case STATE_Empty:
            return '_';
        case STATE_Player1:
            return 'o';
        case STATE_Player2:
            return 'x';
    }
}

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
    printf("%c\n", mapState(player));

    // ascii art
    printf("%s_______________\n", prefix);
    for (int i = 0; i < HEIGHT; i++) {
        printf("%s|", prefix);
        for (int j = 0; j < WIDTH; j++) {
            printf("%c|", mapState(board[i][j]));
        }
        printf("\n");
    }
    printf("\t /|=============|\\\n");
    printf("\t/-|-------------|-\\\n");
}

void moveRight() { if (col < WIDTH - 1) { col++; } }
void moveLeft() { if (col > 0) { col--; } }
void togglePlayer() { if (player == STATE_Player1) player = STATE_Player2; else player = STATE_Player1;}

int placePiece() {
    int placedY = -1;
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        char target = board[i][col];

        if (target == STATE_Empty) {
            board[i][col] = player;
            placedY = i;
            break;
        }
    }

    return placedY;
}

void printArr(char arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", arr[i]);
    }
}
        // 1, 0
        // 2, 1
        // 3, 2
        //      V
        //      x
        //   _______________
        //   |_|x|_|_|_|_|_|
        //   |_|_|x|_|_|_|_|
        //   |_|_|_|x|_|_|_|
        //   |_|_|_|_|x|_|_|
        //   |_|_|_|_|_|x|_|
        //   |_|_|_|_|_|_|x|
        //  /|=============|\
        // /-|-------------|-\


        // -2, -3
        // -1, -2
        // 0, -1
        // 1, 0
        // 2, 1
        // 3, 2
        // 4, 3
        // 5, 4
        // 6, 5
        //          V
        //          x
        //   _______________
        //   |_|x|_|_|_|_|_|
        //   |_|_|x|_|_|_|_|
        //   |_|_|_|x|_|_|_|
        //   |_|_|_|_|x|_|_|
        //   |_|_|_|_|_|x|_|
        //   |_|_|_|_|_|_|x|
        //  /|=============|\
        // /-|-------------|-\

void checkWinner(int x, int y) {
    char row[WIDTH];
    char col[HEIGHT];
    char forwardDiag[HEIGHT];
    char backwardDiag[HEIGHT];

    // col
    for (int i = 0; i < HEIGHT; i++) {
        col[i] = mapState(board[i][x]);
    }
    // ROW
    for (int j = 0; j < WIDTH; j++) {
        row[j] = mapState(board[y][j]);
    }

    int topMin = y - HEIGHT;
    int topMax = y + HEIGHT;
    int startXBackwards = x - HEIGHT;
    int startXForwards = x + HEIGHT;
    int offset = 0;
    int forwardsI = 0;
    int backwardsI = 0;
    for (int dy = topMin; dy < topMax; dy++, offset++) {
        int dxForwards = startXForwards - offset;
        int dxBackwards = startXBackwards + offset;
        if (dy >= 0 && dy < HEIGHT)
        {
            if (dxForwards >= 0 && dxForwards < WIDTH)
                forwardDiag[forwardsI++] = mapState(board[dy][dxForwards]);
            if (dxBackwards >= 0 && dxBackwards < WIDTH)
                backwardDiag[backwardsI++] = mapState(board[dy][dxBackwards]);
        }
    }

    // Fill arrays with empty values
    while(forwardsI < HEIGHT) forwardDiag[forwardsI++] = ' ';
    while(backwardsI < HEIGHT) backwardDiag[backwardsI++] = ' ';

    if (strstr(row, "oooo") || strstr(col, "oooo") || strstr(forwardDiag, "oooo") || strstr(backwardDiag, "oooo"))
    {
        winner = STATE_Player1;
    }
    if (strstr(row, "xxxx") || strstr(col, "xxxx") || strstr(forwardDiag, "xxxx") || strstr(backwardDiag, "xxxx")){
        winner = STATE_Player2;
    }
}

int main() {
    int loops = 0;
    int c, oldCol, placedRow;

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
            placedRow = placePiece();

            if (placedRow != -1) {
                togglePlayer();
                checkWinner(oldCol, placedRow);
            }
            break;
        default:
            if (loops++ >= 10) {
                program_should_exit = true;
            }
            break;
        }
    }
    printf("EXITING\n");
}
