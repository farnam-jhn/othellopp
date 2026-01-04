/*
 * Copyright (C) 2026 Farnam Jahangard
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
