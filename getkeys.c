#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "getkeys.h"

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
#define CTRL_C 3
#define C_ESC 27
#define C_ESC2 91
#define C_UP 65
#define C_DOWN 66
#define C_RIGHT 67
#define C_LEFT 68

#define C_SPACE 32

int setDisplayMode(int fd, int option) {
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

enum KeyCode decodeEsqSeq(int k) {
    switch(k) {
        case C_UP:
            return KEY_Up;
        case C_DOWN:
            return KEY_Down;
        case C_RIGHT:
            return KEY_Right;
        case C_LEFT:
            return KEY_Left;
        default:
            return KEY_Unknown;
    }
}

enum KeyCode getKey() {
    uint8_t raw[3];

    setDisplayMode(STDIN_FILENO, 0);
    system("stty raw");
    read(STDIN_FILENO, &raw, 3);
    system("stty cooked");
    setDisplayMode(STDIN_FILENO, 1);

    switch(raw[0]) {
        case C_ESC:
            if (raw[1] == C_ESC2) {
                return decodeEsqSeq(raw[2]);
            }
        case CTRL_C:
            return KEY_Esc;
        case C_SPACE:
            return KEY_Space;
        default:
            return KEY_Unknown;
    }
}
