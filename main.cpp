#include <iostream>
#include <string>

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


enum BoardState
{
    BOARD_STATE_OK,
    WRONG_WHITE_PAWNS_NUMBER,
    WRONG_BLACK_PAWNS_NUMBER,
    WRONG_BOARD_ROW_LENGTH
};

enum Direction
{
    LEFT_BOTTOM,
    LEFT,
    LEFT_TOP,
    RIGHT_TOP,
    RIGHT,
    RIGHT_BOTTOM,
    ERROR
};



struct Field
{
    char fieldCharID;
    int fieldIntID;
    char object; //pawn, empty or border
    Field* LT;
    Field* RT;
    Field* L; //prev
    Field* R; //next
    Field* LB;
    Field* RB;
};


struct Board
{
    int S;
    int K;
    int allWhitePawns;
    int allBlackPawns;
    int whitePawnsReserve;
    int blackPawnsReserve;
    char player;
    Field** heads;
    BoardState boardState;
};


void printBoardState(BoardState state)
{
    if (state == BOARD_STATE_OK)
    {
        cout << "BOARD_STATE_OK";
    }

    else if (state == WRONG_WHITE_PAWNS_NUMBER)
    {
        cout << "WRONG_WHITE_PAWNS_NUMBER";
    }

    else if (state == WRONG_BLACK_PAWNS_NUMBER)
    {
        cout << "WRONG_BLACK_PAWNS_NUMBER";
    }

    else if (state == WRONG_BOARD_ROW_LENGTH)
    {
        cout << "WRONG_BOARD_ROW_LENGTH";
    }
}


void printField(Field* field)
{
    if (field != nullptr)
    {
        cout << field->fieldCharID << field->fieldIntID << " ";
    }

    else
    {
        cout << "0" << " ";
    }
}


void printNeighbours(Field* current)
{
    cout << "{ ";
    printField(current->L);
    printField(current->LT);
    printField(current->RT);
    printField(current->R);
    printField(current->RB);
    printField(current->LB);
    cout << " }";
}


void printBoardLine(Field* head)
{
    Field* current = head;

    while(current != nullptr)
    {
        printField(current);
        cout << current->object << " ";
        //printNeighbours(current);
        current = current->R;
    }

    cout << endl;
}


void printSpaces(int n)
{
    for (int i = 0; i < n; ++i)
    {
        cout << " ";
    }
}


void printBoard(Board board)
{
    bool spaceDeceasing = true;
    int spaceAmount = board.S;

    for (int lineN = 0; lineN < board.S * 2 + 1; ++lineN)
    {
        printSpaces(spaceAmount);
        printBoardLine(board.heads[lineN]);

        if (spaceAmount == 0)
        {
            spaceDeceasing = false;
        }

        if (!spaceDeceasing)
        {
            spaceAmount++;
        }

        else
        {
            spaceAmount--;
        }
    }
}


void readSpaces(int spaceN)
{
    char letter;

    for (int j = 0; j < spaceN; j++)
    {
        scanf("%c", &letter);
        cout << "Wczytana spacja: " << letter << endl;
    }
}


Field* createTopFrameLine(int size)
{
    Field* head = nullptr;

    for (int i = 0; i < size; ++i)
    {
        auto* newField = (Field*)malloc(sizeof(Field));
        newField->object = BORDER;
        newField->R = head;
        if (head != nullptr) { head->L = newField; }
        head = newField;
        newField->L = nullptr;
        newField->RT = nullptr;
        newField->RB = nullptr;
        newField->LT = nullptr;
        newField->LB = nullptr;
    }

    cout << "Stworzono gorny frame" << endl;
    return head;
}


Field* createBottomFrameLine(int size, Field* prevHead)
{
    Field* head = nullptr;
    Field* prev = nullptr;
    Field* currentPrevLine = prevHead;

    for (int i = 0; i < size; ++i)
    {
        auto* newField = (Field*)malloc(sizeof(Field));
        newField->object = BORDER;
        newField->L = prev;

        if (head != nullptr) { prev->R = newField; }
        else { head = newField; }

        newField->R = nullptr;
        newField->LB = nullptr;
        newField->RB = nullptr;

        newField->LT = currentPrevLine;
        currentPrevLine->RB = newField;

        newField->RT = currentPrevLine->R;
        currentPrevLine->R->LB = newField;

        currentPrevLine = currentPrevLine->R;
        prev = newField;
    }

    cout << "Stworzono dolny frame" << endl;
    return head;
}


Field* createBoardLine(int objectN, Field* prevHead, bool spaceDecreasing, Board &board)
{
    char letter;

    auto* head = (Field*)malloc(sizeof(Field));
    head->L = nullptr;
    head->R = nullptr;
    head->RB = nullptr;
    head->LB = nullptr;

    Field* currentPrevLine = prevHead;

    if (spaceDecreasing)
    {
        head->LT = nullptr;
        head->RT = prevHead;
        prevHead->LB = head;
    }

    else
    {
        head->LT = prevHead;
        prevHead->RB = head;
        head->RT = prevHead->R;
        prevHead->R->LB = head;
        currentPrevLine = prevHead->R;
    }

    head->object = BORDER;

    Field* currentLine = head;

    for (int k = 0; k < objectN; k++)
    {
        scanf("%c", &letter);
        cout << "Wczytany obiekt: " << letter << endl;
        scanf("%c", &letter);
        cout << "Wczytana spacja lub enter: " << letter << endl;

        if (k == objectN - 1)
        {
            if (letter != '\n')
            {
                board.boardState = WRONG_BOARD_ROW_LENGTH;
            }
        }

        else if (letter != ' ')
        {
            board.boardState = WRONG_BOARD_ROW_LENGTH;
        }

        auto* nextField = (Field*)malloc(sizeof(Field));
        nextField->object = letter;

        nextField->LB = nullptr;
        nextField->RB = nullptr;

        nextField->R = nullptr;
        nextField->L = currentLine;
        currentLine->R = nextField;

        nextField->LT = currentPrevLine;
        currentPrevLine->RB = nextField;

        nextField->RT = currentPrevLine->R;
        currentPrevLine->R->LB = nextField;

        currentLine = nextField;
        currentPrevLine = currentPrevLine->R;
    }



    auto* last = (Field*)malloc(sizeof(Field));
    last->object = BORDER;

    last->R = nullptr;
    last->RB = nullptr;
    last->LB = nullptr;

    last->L = currentLine;
    currentLine->R = last;

    if (spaceDecreasing)
    {
        last->RT = nullptr;
        last->LT = currentPrevLine;
        currentPrevLine->RB = last;
    }

    else
    {
        last->LT = currentPrevLine;
        currentPrevLine->RB = last;

        last->RT = currentPrevLine->R;
        currentPrevLine->R->LB = last;
    }

    return head;
}


void createBoard(Board &board)
{
    int spaceN = board.S - 1;
    int lineN = board.S * 2 - 1;
    bool spaceDecreasing = true;
    int objectN = board.S;

    board.heads = (Field**)malloc((lineN + 2) * sizeof(Field*));
    board.heads[0] = createTopFrameLine(board.S + 1);

    for (int i = 1; i <= lineN; i++)
    {
        cout << "Zaczynami czytanie linii nr " << i << endl;
        readSpaces(spaceN);
        board.heads[i] = createBoardLine(objectN, board.heads[i - 1], spaceDecreasing, board);
        cout << "Stworzona linia planszy nr " << i << endl;

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

    board.heads[lineN + 1] = createBottomFrameLine(board.S + 1, board.heads[lineN]);
}


void numerateTiltedLine(Field* tiltedLineHead, char letter)
{
    Field* current = tiltedLineHead;
    int index = 1;

    while (current != nullptr)
    {
        current->fieldCharID = letter;
        current->fieldIntID = index;
        index++;
        current = current->RT;
    }
}


void nameFields(Board &board)
{
    char startingLetter = 'a';

    for (int i = board.S ; i < board.S * 2; i++)
    {
        numerateTiltedLine(board.heads[i], startingLetter);
        startingLetter++;
    }

    Field* current = board.heads[board.S * 2];

    while (current != nullptr)
    {
        numerateTiltedLine(current, startingLetter);
        startingLetter++;
        current = current->R;
    }
}


void readBoard(Board &board)
{
    cout << "Rozpoczynamy wczytywanie planszy" << endl;

    scanf("%d", &board.S);
    scanf("%d", &board.K);
    scanf("%d", &board.allWhitePawns);
    scanf("%d", &board.allBlackPawns);
    scanf("%d", &board.whitePawnsReserve);
    scanf("%d", &board.blackPawnsReserve);
    scanf("%c", &board.player);

    cout << board.S << ", " << board.player << endl;

    char letter;
    scanf("%c", &letter);
    cout << "Wczytany enter" << letter << endl;
    createBoard(board);
    nameFields(board);
}


void checkPawns(Board board)
{
    int whiteCounter = 0;
    int blackCounter = 0;

    for (int i = 1; i < board.S * 2; i++)
    {
        Field* current = board.heads[i];

        while (current != nullptr)
        {
            if (current->object == WHITEPAWN)
            {
                whiteCounter++;
            }

            else if (current->object == BLACKPAWN)
            {
                blackCounter++;
            }

            current = current->R;
        }
    }

    if (whiteCounter != board.allWhitePawns - board.whitePawnsReserve)
    {
        board.boardState = WRONG_WHITE_PAWNS_NUMBER;
    }

    else if (blackCounter != board.allBlackPawns - board.blackPawnsReserve)
    {
        board.boardState = WRONG_BLACK_PAWNS_NUMBER;
    }

    else
    {
        board.boardState = BOARD_STATE_OK;
    }
}


Field* findFieldOnFrame(Board board, char charID, int intID)
{

    Field* last;
    for (int i = 0; i < board.S * 2 + 1; ++i)
    {
        if (board.heads[i]->fieldCharID == charID && board.heads[i]->fieldIntID == intID)
        {
            return board.heads[i];
        }

        last = board.heads[i];

        while (last->R != nullptr)
        {
            last = last->R;
        }

        if (last->fieldCharID == charID && last->fieldIntID == intID)
        {
            return last;
        }
    }


    Field* current = board.heads[0];
    while (current != nullptr)
    {
        if (current->fieldCharID == charID && current->fieldIntID == intID)
        {
            return current;
        }

        current = current->R;
    }


    current = board.heads[board.S * 2];
    while (current != nullptr)
    {
        if (current->fieldCharID == charID && current->fieldIntID == intID)
        {
            return current;
        }

        current = current->R;
    }


    return nullptr;
}


Direction findNeighbourField(Field* startingField, char endChar, int endInt)
{
    Field* neighbourField = startingField->L;

    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT;
        }
    }

    neighbourField = startingField->R;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return RIGHT;
        }
    }

    neighbourField = startingField->LT;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT_TOP;
        }
    }

    neighbourField = startingField->RT;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return RIGHT_TOP;
        }
    }

    neighbourField = startingField->LB;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT_BOTTOM;
        }
    }

    neighbourField = startingField->RB;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return RIGHT_BOTTOM;
        }
    }

    return ERROR;
}


bool isFieldOnBoard(Board board, char fCharID, int fIntID)
{
    for (int i = 0; i < board.S * 2 + 1; ++i)
    {
        cout << board.heads[i]->fieldCharID << board.heads[i]->fieldIntID << endl;
        if (fCharID == board.heads[i]->fieldCharID && fIntID == board.heads[i]->fieldIntID)
        {
            return true;
        }

        Field* current = board.heads[i];

        while (current != nullptr)
        {
            if (fCharID == current->fieldCharID && fIntID == current->fieldIntID)
            {
                return true;
            }
            current = current->R;
        }
    }

    return false;
}


void checkMove(Board board, char startChar, int startInt, char endChar, int endInt)
{
    if (!isFieldOnBoard(board, startChar, startInt))
    {
        cout << "BAD_MOVE_" << startChar << startInt << "_IS_WRONG_INDEX" << endl;
        return;
    }

    if (!isFieldOnBoard(board, endChar, endInt))
    {
        cout << "BAD_MOVE_" << endChar << endInt << "_IS_WRONG_INDEX" << endl;
        return;
    }

    Field* fieldOnFrame = findFieldOnFrame(board, startChar, startInt);
    if (fieldOnFrame == nullptr)
    {
        cout << "BAD_MOVE_" << startChar << startInt << "_IS_WRONG_STARTING_FIELD" << endl;
        return;
    }

    Direction neighbourFieldDirection = findNeighbourField(fieldOnFrame, endChar, endInt);

    // TODO: rozważyć pozostałe 3 przypadki błędów

    cout << "MOVE_COMITTED" << endl;
}


void doMove(Board &board)
{
    char space;
    scanf("%c", &space);

    char startChar;
    scanf("%c", &startChar);
    cout << "startChar = " << startChar << endl;

    int startInt;
    scanf("%d", &startInt);
    cout << "startInt = " << startInt << endl;

    char sign;
    scanf("%c", &sign);
    cout << "sign = " << sign << endl;

    char endChar;
    scanf("%c", &endChar);
    cout << "endCharr = " << endChar << endl;

    int endInt;
    scanf("%d", &endInt);
    cout << "endInt = " << endInt << endl;

    checkMove(board, startChar, startInt, endChar, endInt);
}


void executeCommand(const string& command, Board &board)
{
    if (command == READBOARD)
    {
        readBoard(board);

        if (board.boardState != WRONG_BOARD_ROW_LENGTH)
        {
            checkPawns(board);
        }

        cout << board.boardState << endl;
    }

    else if (command == PRINTBOARD)
    {
        printBoard(board);
    }

    else if (command == DOMOVE)
    {
        doMove(board);
    }
}


void readFile()
{
    string command;
    cin >> command;
    Board board{};

    while (!cin.eof())
    {
        executeCommand(command, board);
        cin >> command;
    }
}


int main()
{
    readFile();
}