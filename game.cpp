// Headers

#include <cstdlib>
#include <iostream>
#include "getchar.h" /* on unix-like systems where default conpiler is usually clang
                        this header doesen't exist, thus i vibe coded a custom getch header. */
#include <fstream>
#include <unistd.h>


// Structures

struct Location{
    int x;
    int y;
};

struct Piece{
    std::string colorOrPlacement; // '○' for white, '●' for black and '□' for empty space (btw i used string since this special characters are larger than char type storage.)
    Location location;

};

struct Player{
    std::string name;
    int no; // 1 for player1 and 2 for player2
    char playerColor; // b for Black and w for White.
    bool playersTurn;
};

// Global variables

// Characters
std::string white = "○";
std::string black = "●";
std::string emptySpace = "□";
std::string block = "■";
// Game board

std::string board[20][20];

// Functions prototype

void initialMenu();
void initializeBoard(int boardSize);
void boardPrint(int boardSize);
void proccecingInput(Player player, int boardSize);

// Main function

int main(){

    // DEVELOPMENT BLOCK

    Player someo;



    // Game Menu :
    while (true) {
        initialMenu();
        char chosenOpt = getch();
        switch (chosenOpt) {
            case '1':

                // Clearing the terminal screen

                system("clear");

                // Showing newgame menu

                std::cout << "1. Single player \n"
                          << "2. Two player \n";

                // Proccessing game option

                char playerCount;
                playerCount = getch();

                if (playerCount == '1'){
                    // asking for board size
                    int boardSize;
                    std::cout << "Enter your board size. (maximum 20) \n";
                    std::cin >> boardSize;

                    while (boardSize % 2 == 1){
                        std::cout << "Board size can only be even. Please enter your board size again. (maximum 20)";
                        std::cin >> boardSize;
                    }

                    // initializing the board

                    initializeBoard(boardSize);

                    // outputing the board

                    boardPrint(boardSize);

                }
                else if (playerCount == '2') {
                    // asking for board size
                    int boardSize;
                    std::cout << "Enter your board size. (maximum 20) \n";
                    std::cin >> boardSize;

                    while (boardSize % 2 == 1){
                        std::cout << "Board size can only be even. Please enter your board size again. (maximum 20)";
                        std::cin >> boardSize;
                    }

                    // initializing the board

                    initializeBoard(boardSize);

                    // Determining the players.

                    Player player1, player2;

                    // Determining first player name and color

                    std::cout << "Enter first player's name : \n";
                    std::cin >> player1.name;


                    std::cout << "Enter first player's color (b for Black, w for White): \n";
                    player1.playerColor = getch();

                    while ((player1.playerColor != 'b') && (player1.playerColor != 'w')) {
                        std::cout << "Enter a valid color. \n";
                        player1.playerColor = getch();
                    }

                    // Determining second player name and color

                    std::cout << "Enter second player's name : \n";
                    std::cin >> player2.name;

                    if (player1.playerColor == 'b'){
                        player2.playerColor = 'w';
                    }
                    else if (player1.playerColor == 'w') {
                        player2.playerColor = 'b';
                    }


                    // outputing the board

                    proccecingInput(player1, boardSize);

                    // boardPrint(boardSize);


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
            case '5':
                someo.playerColor = 'b';
                initializeBoard(6);
                proccecingInput(someo, 6);

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
              << "4. Exit \n"
              << "5. Dev Menu \n";

}

void boardPrint(int boardSize){
    system("clear");

    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if (j != 0){
                std::cout << " ";
            }
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}

void initializeBoard(int boardSize){

    // This functions sets 4 center pieces to wbbw and other cells to empty char

    // setting cells to empty chars

    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            board[i][j] = emptySpace;
        }
    }

    // 4 center cells

    int firstQuarterIndex = (boardSize - 1) / 2;

    /* Game starts with this layout in center :

            ○ ●
            ● ○

    */

    board[firstQuarterIndex][firstQuarterIndex] = white;
    board[firstQuarterIndex + 1][firstQuarterIndex] = black;
    board[firstQuarterIndex][firstQuarterIndex + 1] = black;
    board[firstQuarterIndex + 1][firstQuarterIndex + 1] = white;
}

void proccecingInput(Player player, int boardSize){

    Location navigator;
    navigator.x = 0;
    navigator.y = 0;

    while (true){

        // Placing the block

        bool isBlack = false;
        bool isWhite = false;

        // Copying unchange navigator location to place temp at it

        Location blocksLastLocation;
        blocksLastLocation.x = navigator.x;
        blocksLastLocation.y = navigator.y;

        // Storing what block is at navigator location and placing block on nav's location

        std::string temp = board[navigator.y][navigator.x];
        board[navigator.y][navigator.x] = block;

        // Printing the board

        boardPrint(boardSize);

        // Proccessing input

        char inputChar = getch();

        if ((navigator.y > 0) &&
            (inputChar == 'w')){
            navigator.y--;
        }
        else if ((navigator.x > 0) &&
            (inputChar == 'a')){
            navigator.x--;
        }
        else if ((navigator.y < (boardSize - 1)) &&
            (inputChar == 's')){
            navigator.y++;
        }
        else if ((navigator.x < (boardSize - 1)) &&
            (inputChar == 'd')){
            navigator.x++;
        }
        else if ((player.playerColor == 'b') &&
            (inputChar == '\n' || inputChar == '\r')){
            isBlack = true;
        }
        else if ((player.playerColor == 'w') &&
            (inputChar == '\n' || inputChar == '\r')) {
            isWhite = true;
        }
        else {
            system("clear");
            std::cout << "Error! invalid input. \n";
            sleep(3);
            system("clear");
        }

        // Placing back the temp on its location

        board[blocksLastLocation.y][blocksLastLocation.x] = temp;

        // Proccessing "Enter"

        if (isBlack) {
            board[navigator.y][navigator.x] = black;
        }
        else if (isWhite) {
            board[navigator.y][navigator.x] = white;
        }

        // Printing the board after

        boardPrint(boardSize);

    }
}
