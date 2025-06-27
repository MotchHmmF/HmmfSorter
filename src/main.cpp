#include <raylib.h>
#include "gameBoard.h"


int main() {
    // GameBoard board(3,3,100,1,true);
    // GameBoard board(5,5,50,1,true);
    // GameBoard board;
    GameBoard board(100,100,10,0,false);
    board.Start();

    return 0;
}