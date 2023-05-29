#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const string READBOARD = "LOAD_GAME_BOARD";
const string PRINTBOARD = "PRINT_GAME_BOARD";
const string DOMOVE = "DO_MOVE";

const char WHITEPAWN = 'W';
const char BLACKPAWN = 'B';
const char EMPTYFIELD = '_';
const char BORDER = '+';

const char WHITEPLAYER = 'W';
const char BLACKPLAYER = 'B';

struct Field
{
    char fieldIdChar;
    int fieldIdInt;
    char object; //pawn, empty or border
    Field* LT;
    Field* RT;
    Field* L;
    Field* R;
    Field* BL;
    Field* BR;
};

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

enum BoardState
{
    BOARD_STATE_OK,
    WRONG_WHITE_PAWNS_NUMBER,
    WRONG_BLACK_PAWNS_NUMBER,
    WRONG_BOARD_ROW_LENGTH
};

void readBoard()
{
    Board board;
    scanf("%d", &board.S);
    scanf("%d", &board.K);
    scanf("%d", &board.GW);
    scanf("%d", &board.GB);
    scanf("%d", &board.whitePawns);
    scanf("%d", &board.blackPawns);
    scanf("%c", &board.player);

    cout << board.S << ", " << board.player << endl;

    char letter;
    scanf("%c", &letter);
    cout << "Wczytany enter" << letter << endl;

    int spaceN = board.S - 1;
    int lineN = board.S * 2 - 1;
    bool spaceDecreasing = true;
    int objectN = board.S;

    for (int i = 0; i < lineN; i++)
    {
        for (int j = 0; j < spaceN; j++)
        {
            scanf("%c", &letter);
            cout << "Wczytana spacja: " << letter << endl;
        }

        for (int k = 0; k < objectN; k++)
        {
            scanf("%c", &letter);
            cout << "Wczytany obiekt: " << letter << endl;
            scanf("%c", &letter);
            cout << "Wczytana spacja lub enter: " << letter << endl;
        }

        if (spaceN == 0) {spaceDecreasing = false;}

        if (spaceDecreasing)
        {
            spaceN--;
            objectN++;
        }

        else
        {
            spaceN++;
            objectN--;
        }
    }

}

void executeCommand(string command)
{
    if (command.compare(READBOARD))
    {
        readBoard();
    }

    else if (command.compare(PRINTBOARD))
    {
        cout << "Rozpoznano komende printBoard" << endl;
    }

    else if (command.compare(DOMOVE))
    {
        cout << "Rozpoznano komende doMove" << endl;
    }
}


void readFile()
{
    string command;
    cin >> command;

    while (!cin.eof())
    {
        executeCommand(command);
        cin >> command;
    }
}


int main()
{
    readFile();
}