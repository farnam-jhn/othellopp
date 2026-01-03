// Headers

#include <cstdlib>
#include <iostream>
#include "getchar.h" /* on unix-like systems where default conpiler is usually clang
                        this header doesen't exist, thus i vibe coded a custom getch header. */
#include <fstream>
#include <string>
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


// Functions prototype

void initialMenu();
void initializeBoard(int boardSize);
void boardPrint(int boardSize, Player player);
void proccessingInput(Player &player1,Player &player2, int boardSize);
void gameReport(int boardSize,Player p1,Player p2);

std::string timeDate();

bool isPossible(Player &playerToPlay, Location navigator, int boardSize, bool justChecking = false);

// Main function

int main(){

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

                    // boardPrint(boardSize);

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
            // todo
                break;
            case '3':
            // todo
                break;
            case '4':
                return 0;
                break;
            case '5':
                someo1.playerColor = 'b';
                someo2.playerColor = 'w';
                initializeBoard(10);
                proccessingInput(someo1, someo2, 10);

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

        boardPrint(boardSize, player);

    }
}

std::string timeDate(){

    /*
       This function gets the time from system
       and writes it to time.txt with system commands
       after that it oppens the file, reads out the line
       and changes the format of it.
    */


    system("rm -rf time.txt"); // to delete old time.txts if exist.
    system("touch time.txt");
    system("echo $(timedatectl | grep \"Local time\") >> time.txt");

    std::ifstream file("time.txt");
    std::string unformatedTime;
    std::getline(file, unformatedTime);

    unformatedTime.erase(0,12); // Removes "Local time: " from the start

    // Removing " +0330" from the last

    int length = unformatedTime.length();
    unformatedTime.erase(length - 6, 6);

    return unformatedTime;

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

    std::cout << "Game finished. \n";
    sleep(1);
    std::cout << p1.name << "\'s piece count : " << p1PieceCount << std::endl;
    std::cout << p2.name << "\'s piece count : " << p2PieceCount << std::endl;

    // Determining winner

    if (p1PieceCount > p2PieceCount){
        std::cout << p1.name << " won !";
    }
    else if (p2PieceCount > p1PieceCount){
        std::cout << p2.name << " won !";
    }
    else {
        std::cout << "The game was draw.";
    }

    sleep(4);
}
