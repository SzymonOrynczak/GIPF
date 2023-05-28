#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Board
{
    int S;
    int K;
    int GW;
    int GB;
    int whitePawns;
    int blackPawns;
    char player;
};

enum BoardState {
    BOARD_STATE_OK,
    WRONG_WHITE_PAWNS_NUMBER,
    WRONG_BLACK_PAWNS_NUMBER,
    WRONG_BOARD_ROW_LENGTH
};


void readBoard()
{

}



int main()
{
    readBoard();
}