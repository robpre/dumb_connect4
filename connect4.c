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

char16_t board[HEIGHT][WIDTH] = {
    {'_', '_', '_', '_', '_', '_', '_'},
    {'_', '_', '_', '_', '_', '_', '_'},
    {'_', '_', '_', '_', '_', '_', '_'},
    {'_', '_', '_', '_', '_', '_', '_'},
    {'_', '_', '_', '_', '_', '_', '_'},
    {'_', '_', '_', '_', '_', '_', '_'},
};

int col = 0;
int colsize = 2;
char player = 'o';
bool exit = false;

void render_board() {
    // system("clear");
    char prefix[] = "\t  ";

    printf("%s ", prefix);
    for (int i = 0; i < col * colsize; i++) {
        printf(" ");
    }
    printf("V\n");

    printf("%s ", prefix);
    for (int i = 0; i < col * colsize; i++) {
        printf(" ");
    }
    printf("%c\n", player);

    printf("%s_______________\n", prefix);
    for (int i = 0; i < HEIGHT; i++) {
        printf("%s|", prefix);
        for (int j = 0; j < WIDTH; j++) {
            printf("%c|", board[i][j]);
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

        if (target == '_') {
            board[i][col] = player;
            break;
        }
    }

    togglePlayer();
}

void checkWinner() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char check = board[i][j];

            if (check != '_') {

            }
        }
    }
}

int main() {
    int loops = 0;
    int c;

    while (!exit) {
        render_board();
        c = getKey();

        switch (c) {
        case KEY_Esc:
            exit = true;
            break;
        case KEY_Left:
            moveLeft();
            break;
        case KEY_Right:
            moveRight();
            break;
        case KEY_Space:
            placePiece();
        default:
            if (loops++ >= 10) {
                exit = true;
            }
            break;
        }
    }
}
