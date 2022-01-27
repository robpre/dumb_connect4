#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <uchar.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
#define HEIGHT 6
#define WIDTH 7
#define ESC 27
#define LEFT 68
#define RIGHT 67
#define SPACE 32

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

void render_board()
{
    system("clear");
    char prefix[] = "\t  ";

    printf("%s ", prefix);
    for (int i = 0; i < col * colsize; i++) {
        printf(" ");
    }
    printf("V\n");
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

int set_disp_mode(int fd, int option) {
    int err;
    struct termios term;
    if (tcgetattr(fd, &term) == -1) {
        perror("Cannot get the attribution of the terminal");
        return 1;
    }
    if (option) {
        term.c_lflag |= ECHOFLAGS;
    } else {
        term.c_lflag &= ~ECHOFLAGS;
    }
    err = tcsetattr(fd, TCSAFLUSH, &term);
    if (err == -1 && err == EINTR) {
        perror("Cannot set the attribution of the terminal");
        return 1;
    }
    return 0;
}

int getKey() {
    int c;
    system("stty raw");
    set_disp_mode(STDIN_FILENO, 0);
    c = getchar();
    system("stty cooked");
    set_disp_mode(STDIN_FILENO, 1);

    return c;
}

int main() {
    bool exit = false;
    int loops = 0;
    int c;

    while (!exit) {
        render_board();
        c = getKey();

        switch (c) {
        case 3:
        case ESC:
            exit = true;
            break;
        default:
            if (loops++ >= 60) {
                exit = true;
            }
            break;
        }

        printf("\nthe key %i\n", c);
    }
}
