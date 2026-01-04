:/*
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


// Headers

#include <cstdlib>
#include <ios>
#include <iostream>
#include "getchar.h" /* on unix-like systems where default conpiler is usually clang
                        this header doesen't exist, thus i vibe coded a custom getch header. */
#include <fstream>
#include <ostream>
#include <string>
#include <unistd.h> // for sleep function
#include <ctime> // To get system time
// #include <chrono>

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
    bool isBot = false;
    char playerColor; // b for Black and w for White.
    bool playersTurn = false;
    int pieceCount = 0;
};

// Global variables

std::string lastPlayer;

// Characters

std::string white = "●";
std::string black = "○";
std::string emptySpace = "□";
std::string block = "■";

// Game board

std::string board[20][20];

// Player pieces

int p1PieceCount = 0;
int p2PieceCount = 0;

// Time and date

std::string timeAndDate;

// Functions prototype

void initialMenu();
void initializeBoard(int boardSize);
void boardPrint(int boardSize, Player player);
void proccessingInput(Player &player1,Player &player2, int boardSize);
void gameReport(int boardSize,Player p1,Player p2);
void gameHistory();

Location bot(int boardSize, Player bot);

std::string timeDate();

bool isPossible(Player &playerToPlay, Location navigator, int boardSize, bool justChecking = false);

// Main function

int main(){

    // For random number

    srand(static_cast<unsigned int>(time(nullptr)));

    // DEVELOPMENT BLOCK

    Player someo1, someo2;


    // Clearing the screen

    system("clear");

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

                // SINGLE PLAYER

                if (playerCount == '1'){
                    // asking for board size

                    int boardSize;
                    std::cout << "Enter your board size. (maximum 20) \n";
                    std::cin >> boardSize;
                    while (boardSize <= 4){
                        std::cout << "Board size should be greater than 4. Please enter your board size again. (minimum 4)";
                        std::cin >> boardSize;
                    }
                    while (boardSize > 20){
                        std::cout << "Board size should be less than 20. Please enter your board size again. (maximum 20)";
                        std::cin >> boardSize;
                    }
                    while (boardSize % 2 == 1){
                        std::cout << "Board size can only be even. Please enter your board size again. (maximum 20)";
                        std::cin >> boardSize;
                    }

                    // initializing the board

                    initializeBoard(boardSize);

                    // Determining the players.

                    Player player, bot;
                    bot.isBot = true;

                    // Determining first player name and color

                    std::cout << "Enter your name : \n";
                    std::cin >> player.name;

                    std::cout << "Enter your color (b for Black, w for White): \n";
                    player.playerColor = getch();
                    std::cout << player.playerColor << std::endl;

                    if (player.playerColor == 'b'){
                        bot.playerColor = 'w';
                    }
                    else {
                        bot.playerColor = 'b';
                    }

                    bot.name = "Bot";

                    // Determining first player

                    if (player.playerColor == 'b'){
                        lastPlayer = "player2";
                    }
                    else {
                        lastPlayer = "player1";
                    }

                    proccessingInput(player, bot, boardSize);

                    // boardPrint(boardSize);

                }

                // MULTIPLAYER

                else if (playerCount == '2') {
                    // asking for board size

                    int boardSize;
                    std::cout << "Enter your board size. (maximum 20) \n";
                    std::cin >> boardSize;
                    while (boardSize <= 4){
                        std::cout << "Board size should be greater than 4. Please enter your board size again. (minimum 4)";
                        std::cin >> boardSize;
                    }
                    while (boardSize > 20){
                        std::cout << "Board size should be less than 20. Please enter your board size again. (maximum 20)";
                        std::cin >> boardSize;
                    }
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

                    std::cout << player1.playerColor << std::endl;

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

                    // Determining first player

                    if (player1.playerColor == 'b'){
                        lastPlayer = "player2";
                    }
                    else {
                        lastPlayer = "player1";
                    }

                    proccessingInput(player1, player2, boardSize);


                }

                break;
            case '2':

                system("clear");

                // Telling player what to do

                std::cout << "Othello is a simple board game. \n"
                          << "  Game rules : \n \n"
                          << "    The game starts with four pieces at the center and each player has a color; \n"
                          << "    players can place piece in a place if this two conditions are met : \n \n"
                          << "      1. Player's piece should at least bound one of the opponent pieces (without space and player's own piece in the middle)\n"
                          << "      2. Player's selected location should be empty. \n \n"
                          << "    Game finishes when there is no legal move left and the player with more pieces on the board wins.\n \n"
                          << "  How to play : \n \n"
                          << "    You can navigate through the board with W A S D keys \n"
                          << "    and you can place a block by hitting enter when navigator is at the desired location. \n \n"
                          << "PRESS ANY KEY TO RETURN TO THE MAIN MENU. \n";

                getch();

                break;
            case '3':
                gameHistory();
                std::cout << std::endl << "PRESS ANY KEY TO RETURN TO THE MAIN MENU. \n";
                getch();
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

    system("clear");

    std::cout << "Choose your option : \n"
              << "1. New Game \n"
              << "2. Help \n"
              << "3. Game History \n"
              << "4. Exit \n"
              << "5. Dev Menu \n";

}

void boardPrint(int boardSize, Player player){
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
    std::cout << player.name << " to play. \n";
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

            ● ○
            ○ ●

    */

    board[firstQuarterIndex][firstQuarterIndex] = white;
    board[firstQuarterIndex + 1][firstQuarterIndex] = black;
    board[firstQuarterIndex][firstQuarterIndex + 1] = black;
    board[firstQuarterIndex + 1][firstQuarterIndex + 1] = white;

    // Getting the start time

    timeAndDate = timeDate();

}

void proccessingInput(Player &player1,Player &player2, int boardSize){

    Location navigator;
    navigator.x = 0;
    navigator.y = 0;


    while (true){

        // Determining which player should move
        while (true) {
            int player1PlacablePoses = 0;
            int player2PlacablePoses = 0;

            for (int i = 0; i < boardSize; i++){
                for (int j = 0; j < boardSize; j++){
                    Location temp;
                    temp.y = i;
                    temp.x = j;
                    // Check if player 1 can move anywhere
                    if (isPossible(player1, temp, boardSize, true)){
                            player1PlacablePoses++;
                    }
                }
            }

            for (int i = 0; i < boardSize; i++){
                for (int j = 0; j < boardSize; j++){
                    Location temp;
                    temp.y = i;
                    temp.x = j;
                    if ((board[i][j] == emptySpace) &&
                        (isPossible(player2, temp, boardSize, true))){
                            player2PlacablePoses++;
                    }
                }
            }

            // Game finished

            if ((player1PlacablePoses == 0)&&
                (player2PlacablePoses == 0)){
                    gameReport(boardSize, player1, player2);
                    return;
            }

            // If not finished determinig the player

            if (lastPlayer == "player1"){
                if (player2PlacablePoses != 0){
                    player2.playersTurn = true;
                    player1.playersTurn = false;
                    break;
                }
                else {
                    player1.playersTurn = true;
                    player2.playersTurn = false;
                    std::cout << player2.name << " has no moves. \n";
                    sleep(2);
                    break;
                }
            }

            if (lastPlayer == "player2"){
                if (player1PlacablePoses != 0){
                    player1.playersTurn = true;
                    player2.playersTurn = false;
                    break;
                }
                else {
                    player2.playersTurn = true;
                    player1.playersTurn = false;
                    std::cout << player1.name << " has no moves. \n";
                    sleep(2);
                    break;
                }
            }

        }



        Player &player = (player1.playersTurn) ? player1 : player2;

        bool piecePlaced = false;

        while (!piecePlaced){

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

            boardPrint(boardSize, player);

            // Proccessing input

            if (!player.isBot){
                char inputChar = getch();
                if (inputChar == 'w'){
                    if (navigator.y > 0){
                        navigator.y--;
                    }
                }
                else if (inputChar == 'a'){
                    if (navigator.x > 0){
                        navigator.x--;
                    }
                }
                else if (inputChar == 's'){
                    if (navigator.y < boardSize - 1){
                        navigator.y++;
                    }
                }
                else if (inputChar == 'd'){
                    if (navigator.x < boardSize - 1){
                        navigator.x++;
                    }
                }
                else if (inputChar == '\n' || inputChar == '\r'){
                    board[blocksLastLocation.y][blocksLastLocation.x] = temp;

                    bool posibilityToPlace = isPossible(player, navigator, boardSize);

                    if (posibilityToPlace){
                        if (player.playerColor == 'b') {
                            board[navigator.y][navigator.x] = black;
                        }
                        else {
                            board[navigator.y][navigator.x] = white;
                        }

                        lastPlayer = (player.name == player1.name) ? "player1" : "player2";
                        piecePlaced = true;
                    }
                    else {
                        system("clear");
                        std::cout << "Invalid move. \n";
                        sleep(1);
                    }
                }
                else {
                    system("clear");
                    std::cout << "Error! invalid input. \n";
                    sleep(1);
                    system("clear");
                }

                // Placing back the temp on its location

                if (!piecePlaced) {
                    board[blocksLastLocation.y][blocksLastLocation.x] = temp;
                }
            }
            else {

                // Storing navigator value

                board[blocksLastLocation.y][blocksLastLocation.x] = temp;

                // Getting the bot move and executing it

                Location botLocation = bot(boardSize, player);

                // Placing and flipping the pieces

                usleep(500000); // bot calculates its placement too fast , in order to make it easier for player to understand i put this command here so it looks like bot is thinking

                isPossible(player, botLocation, boardSize);

                piecePlaced = true;
                lastPlayer = (player.name == player1.name) ? "player1" : "player2";
            }


        }

        boardPrint(boardSize, player);

    }
}

std::string timeDate() {
    std::time_t now = std::time(nullptr);
    std::string timeStr = std::ctime(&now);

    // Remove \n from end
    timeStr.erase(timeStr.length() - 1);

    return timeStr;
}

bool isPossible(Player &playerToPlay, Location navigator, int boardSize, bool justChecking){

    // This function check if it's posiible to place a peace at somewhere or not and flips the piece in between if needed.

    // Checking if selected location is empty or not
    if (board[navigator.y][navigator.x] != emptySpace){
        return false;
    }
    /* We have to check in 8 directions , diagnols, vertical and horizantal */

    // Determining player color

    std::string opposColor;
    std::string playerColor;
    if (playerToPlay.playerColor == 'b'){
        playerColor = black;
        opposColor = white;
    }
    else {
        playerColor = white;
        opposColor = black;
    }

    // For flipping mechanism

    Location tempFlip[400];
    int toFLipCount = 0;

    Location currentDirFlips[20];


    // up right

    int dx = 1, dy = -1;
    bool sandwich = false;
    int currentDirFlipCount = 0;

    if ((navigator.y + dy >= 0 && navigator.x + dx < boardSize) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        // Saving oppos color location

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        // Moving to next block

        dx++;
        dy--;

        while ((navigator.x + dx < boardSize) &&
               (navigator.y + dy >= 0)) {

            // This loop scans for opposite color in direction if first one was oppos color.

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            // If an element is not empty space nor player color, it's opposite color for sure, thus we have to save it in order to flip later.

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            // Moving along the direction
            dx++;
            dy--;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }

            // Adding this direction flip to tempFlip inorder to flip them all at last.

            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }

        }
    }


    // down left

    dx = -1;
    dy = 1;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.y + dy < boardSize && navigator.x + dx >= 0) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dx--;
        dy++;

        while ((navigator.x + dx >= 0 )&&
               (navigator.y + dy < boardSize)){

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dx--;
            dy++;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }


    // up left

    dx = -1;
    dy = -1;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.y + dy >= 0 && navigator.x + dx >= 0) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dx--;
        dy--;

        while (navigator.x + dx >= 0 && navigator.y + dy >= 0) {

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dx--;
            dy--;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }


    // down right

    dx = 1;
    dy = 1;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.y + dy < boardSize && navigator.x + dx < boardSize)&&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dx++;
        dy++;

        while ((navigator.x + dx < boardSize) &&
               (navigator.y + dy < boardSize)){

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dx++;
            dy++;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }


    // right

    dx = 1;
    dy = 0;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.x + dx < boardSize) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dx++;

        while (navigator.x + dx < boardSize){

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dx++;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }


    // left

    dx = -1;
    dy = 0;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.x + dx >= 0)&&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dx--;

        while (navigator.x + dx >= 0) {

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dx--;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }


    // up

    dx = 0;
    dy = -1;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.y + dy >= 0) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dy--;

        while (navigator.y + dy >= 0) {

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dy--;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }



    // down

    dx = 0;
    dy = 1;
    sandwich = false;
    currentDirFlipCount = 0;

    if ((navigator.y + dy < boardSize) &&
        (board[navigator.y + dy][navigator.x + dx] == opposColor)){

        currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
        currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
        currentDirFlipCount++;

        dy++;

        while (navigator.y + dy < boardSize) {

            std::string currentPiece = board[navigator.y + dy][navigator.x + dx];

            if (currentPiece == playerColor){
                sandwich = true;
                break;
            }
            else if (currentPiece == emptySpace){
                break;
            }

            currentDirFlips[currentDirFlipCount].x = navigator.x + dx;
            currentDirFlips[currentDirFlipCount].y = navigator.y + dy;
            currentDirFlipCount++;

            dy++;
        }

        if (sandwich) {
            if (justChecking){
                return true;
            }
            for (int k = 0; k < currentDirFlipCount; k++) {
                tempFlip[toFLipCount].x = currentDirFlips[k].x;
                tempFlip[toFLipCount].y = currentDirFlips[k].y;
                toFLipCount++;
            }
        }
    }

    // Flipping pieces if player did a valid move.

    if (toFLipCount > 0) {
        if (!justChecking) {
            // Player's move is valid, place the piece first.
            board[navigator.y][navigator.x] = playerColor;

            // Now flip the saved pieces.
            for (int k = 0; k < toFLipCount; k++){
                board[tempFlip[k].y][tempFlip[k].x] = playerColor;
            }
        }
        return true;
    }

    return false;
}

void gameReport(int boardSize,Player p1,Player p2){

    // This fcuntion reports the game at end of the game.

    // Determining players color

    std::string p1Color;
    std::string p2Color;
    if (p1.playerColor == 'b'){
        p1Color = black;
        p2Color = white;
    }
    else {
        p1Color = white;
        p2Color = black;
    }

    p1PieceCount = 0;
    p2PieceCount = 0;

    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if (board[i][j] == p1Color) {
                p1PieceCount++;
            }
            else if (board[i][j] == p2Color){
                p2PieceCount++;
            }
        }
    }

    // Reporting

    system("clear");

    std::cout << "Game finished. \n";
    sleep(1);
    std::cout << p1.name << "\'s piece count : " << p1PieceCount << std::endl;
    std::cout << p2.name << "\'s piece count : " << p2PieceCount << std::endl;

    // Determining winner

    std::string result;

    if (p1PieceCount > p2PieceCount){
        result = p1.name + " won.";
    }
    else if (p2PieceCount > p1PieceCount){
        result = p2.name + " won.";
    }
    else {
        result = "Draw.";
    }

    std::cout << result;

    // Saving into game history

    std::ofstream file("history.txt", std::ios::app);

    std::string firstPlayerColor = (p1.playerColor == 'b')? "Black" : "White";
    std::string secondPlayerColor = (p2.playerColor == 'b')? "Black" : "White";

    if (p2.isBot){
        file << std::endl
             << "   Game date : " << timeAndDate << std::endl
             << "   Player's name and color : " << p1.name << ", " << firstPlayerColor << std::endl
             << "   Player's piece count : " << p1PieceCount << std::endl
             << "   Bot's piece count : " << p2PieceCount << std::endl
             << "   Result : " << result << std::endl;
    }
    else {
        file << std::endl
             << "   Game date : " << timeAndDate << std::endl
             << "   First player name and color : " << p1.name << ", " << firstPlayerColor << std::endl
             << "   Second player name and color : " << p2.name << ", " << secondPlayerColor << std::endl
             << "   First player piece count : " << p1PieceCount << std::endl
             << "   Second player piece count : " << p2PieceCount << std::endl
             << "   Result : " << result << std::endl;
    }

    file.close();


    sleep(4);
    system("clear");
}

Location bot(int boardSize, Player bot){
    Location possiblePlacements[400];
    int possiblePlacementsCount = 0;
    Location tempBlock;
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            tempBlock.x = j;
            tempBlock.y = i;
            if (isPossible(bot, tempBlock, boardSize, true)){
                possiblePlacements[possiblePlacementsCount].x = tempBlock.x;
                possiblePlacements[possiblePlacementsCount].y = tempBlock.y;
                possiblePlacementsCount++;
            }
        }
    }

    // Choosing the block randomally

    int randBlock = rand() % possiblePlacementsCount;

    return possiblePlacements[randBlock];

}

void gameHistory(){

    system("clear");

    std::ifstream file("history.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();

}
