// Headers

#include <cstdlib>
#include <iostream>
#include "getchar.h" /* on unix-like systems where default conpiler is usually clang
                        this header doesen't exist, thus i vibe coded a custom getch header. */
#include <fstream>


// Structures

struct Piece{
    std::string colorOrPlacement; // '○' for white, '●' for black and '□' for empty space (btw i used string since this special characters are larger than char type storage.)


};

// Global variables

// Characters
std::string white = "○";
std::string black = "●";
std::string emptySpace = "□";

// Game board
std::string  borad[8][8] = {emptySpace};

// Functions prototype

void initialMenu();
void initializeBoard();

// Main function

int main(){



    // Game Menu :
    while (true) {
        initialMenu();
        char chosenOpt = getch();
        switch (chosenOpt) {
            case '1':

                // Showing newgame menu

                std::cout << "1. Single player \n"
                          << "2. Two player \n";

                // Proccessing game option

                char playerCount;
                playerCount = getch();

                if (playerCount == '1'){
                    // todo
                }
                else if (playerCount == '2') {
                    // todo
                }

                break;
            case '2':
            // todo
                break;
            case '3':
            // todo
                break;
            case '4':
                return 0;
                break;
            default:
                std::cout << "ERROR. \n";
                system("clear");
        }

    }



}

// Functions

void initialMenu(){

    // This function outputs the game menu

    std::cout << "Choose your option : \n"
              << "1. New Game \n"
              << "2. Help \n"
              << "3. Game History \n"
              << "4. Exit \n";

}

void initializeBoard(){

    // This functions sets 4 center pieces
    //
    //

}
