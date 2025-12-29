// getchar.h
#pragma once  // prevents multiple inclusion
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>  // Windows
inline char mygetch() {
    return _getch(); // Windows getch
}
#else
#include <termios.h>
#include <unistd.h>
inline char getch() {
    char buf = 0;
    struct termios old = {0};
    tcgetattr(0, &old);
    old.c_lflag &= ~ICANON; // disable line buffering
    old.c_lflag &= ~ECHO;   // disable echo
    tcsetattr(0, TCSANOW, &old);
    read(0, &buf, 1);
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &old);
    return buf;
}
#endif
