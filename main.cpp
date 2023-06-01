#include <iostream>
#include <string>

using namespace std;

const string READBOARD = "LOAD_GAME_BOARD";
const string PRINTBOARD = "PRINT_GAME_BOARD";
const string DOMOVE = "DO_MOVE";

const char WHITEPAWN = 'W';
const char BLACKPAWN = 'B';
const char EMPTYFIELD = '_';
const char FRAMEFIELD = '+';

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
    Field* LeftTop;
    Field* RightTop;
    Field* Left; //prev
    Field* Right; //next
    Field* LeftBottom;
    Field* RightBottom;
};


struct Board
{
    int S;
    int K;
    int allWhitePawns;
    int allBlackPawns;
    int whitePawnsReserve;
    int blackPawnsReserve;
    char playerColor;
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
        //cout << field->fieldCharID << field->fieldIntID << " ";
    }

    else
    {
        cout << "0" << " ";
    }
}


void printNeighbours(Field* current)
{
    cout << "{ ";
    printField(current->Left);
    printField(current->LeftTop);
    printField(current->RightTop);
    printField(current->Right);
    printField(current->RightBottom);
    printField(current->LeftBottom);
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
        current = current->Right;
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
        newField->object = FRAMEFIELD;
        newField->Right = head;
        if (head != nullptr) { head->Left = newField; }
        head = newField;
        newField->Left = nullptr;
        newField->RightTop = nullptr;
        newField->RightBottom = nullptr;
        newField->LeftTop = nullptr;
        newField->LeftBottom = nullptr;
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
        newField->object = FRAMEFIELD;
        newField->Left = prev;

        if (head != nullptr) { prev->Right = newField; }
        else { head = newField; }

        newField->Right = nullptr;
        newField->LeftBottom = nullptr;
        newField->RightBottom = nullptr;

        newField->LeftTop = currentPrevLine;
        currentPrevLine->RightBottom = newField;

        newField->RightTop = currentPrevLine->Right;
        currentPrevLine->Right->LeftBottom = newField;

        currentPrevLine = currentPrevLine->Right;
        prev = newField;
    }

    cout << "Stworzono dolny frame" << endl;
    return head;
}


Field* createBoardLine(int objectN, Field* prevHead, bool spaceDecreasing, Board &board)
{
    char letter;
    char sign;

    Field* head = (Field*)malloc(sizeof(Field));
    head->Left = nullptr;
    head->Right = nullptr;
    head->RightBottom = nullptr;
    head->LeftBottom = nullptr;

    Field* currentPrevLine = prevHead;

    if (spaceDecreasing)
    {
        head->LeftTop = nullptr;
        head->RightTop = prevHead;
        prevHead->LeftBottom = head;
    }

    else
    {
        head->LeftTop = prevHead;
        prevHead->RightBottom = head;
        head->RightTop = prevHead->Right;
        prevHead->Right->LeftBottom = head;
        currentPrevLine = prevHead->Right;
    }

    head->object = FRAMEFIELD;

    Field* currentLine = head;

    for (int k = 0; k < objectN; k++)
    {
        scanf("%c", &sign);
        cout << "Wczytany obiekt: " << sign << endl;
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

        Field* nextField = (Field*)malloc(sizeof(Field));
        nextField->object = sign;

        nextField->LeftBottom = nullptr;
        nextField->RightBottom = nullptr;

        nextField->Right = nullptr;
        nextField->Left = currentLine;
        currentLine->Right = nextField;

        nextField->LeftTop = currentPrevLine;
        currentPrevLine->RightBottom = nextField;

        nextField->RightTop = currentPrevLine->Right;
        currentPrevLine->Right->LeftBottom = nextField;

        currentLine = nextField;
        currentPrevLine = currentPrevLine->Right;
    }



    auto* last = (Field*)malloc(sizeof(Field));
    last->object = FRAMEFIELD;

    last->Right = nullptr;
    last->RightBottom = nullptr;
    last->LeftBottom = nullptr;

    last->Left = currentLine;
    currentLine->Right = last;

    if (spaceDecreasing)
    {
        last->RightTop = nullptr;
        last->LeftTop = currentPrevLine;
        currentPrevLine->RightBottom = last;
    }

    else
    {
        last->LeftTop = currentPrevLine;
        currentPrevLine->RightBottom = last;

        last->RightTop = currentPrevLine->Right;
        currentPrevLine->Right->LeftBottom = last;
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
        current = current->RightTop;
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
        current = current->Right;
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
    char space;
    scanf("%c", &space);
    cout << "space: " << space << endl;
    scanf("%c", &board.playerColor);
    cout << "playerColor: " << board.playerColor << endl;
    char letter;
    scanf("%c", &letter);
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

            current = current->Right;
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

        while (last->Right != nullptr)
        {
            last = last->Right;
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

        current = current->Right;
    }


    current = board.heads[board.S * 2];
    while (current != nullptr)
    {
        if (current->fieldCharID == charID && current->fieldIntID == intID)
        {
            return current;
        }

        current = current->Right;
    }


    return nullptr;
}


Direction findNeighbourField(Field* startingField, char endChar, int endInt)
{
    Field* neighbourField = startingField->Left;

    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT;
        }
    }

    neighbourField = startingField->Right;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return RIGHT;
        }
    }

    neighbourField = startingField->LeftTop;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT_TOP;
        }
    }

    neighbourField = startingField->RightTop;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return RIGHT_TOP;
        }
    }

    neighbourField = startingField->LeftBottom;
    if (neighbourField != nullptr)
    {
        if (neighbourField->fieldCharID == endChar && neighbourField->fieldIntID == endInt)
        {
            return LEFT_BOTTOM;
        }
    }

    neighbourField = startingField->RightBottom;
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
            current = current->Right;
        }
    }
    return false;
}


Field* findNeighbourFieldInDirection(Field* startField, Direction direction)
{
    if (direction == LEFT)
    {
        return startField->Left;
    }

    else if (direction == RIGHT)
    {
        return startField->Right;
    }

    if (direction == LEFT_TOP)
    {
        return startField->LeftTop;
    }

    if (direction == RIGHT_TOP)
    {
        return startField->RightTop;
    }

    if (direction == RIGHT_BOTTOM)
    {
        return startField->RightBottom;
    }

    if (direction == LEFT_BOTTOM)
    {
        return startField->LeftBottom;
    }

    return nullptr;
}


bool isEmptyFieldInDirection(Field* startField, Direction direction)
{
    Field* current = findNeighbourFieldInDirection(startField, direction);

    while(current != nullptr && (current->object == WHITEPAWN || current->object == BLACKPAWN))
    {
        current = findNeighbourFieldInDirection(current, direction);
    }

    if (current->object == FRAMEFIELD)
    {
        return false;
    }

    return true;
}


void movePawns(Board board, Field* startingField, Direction direction)
{
    char currentPawn = board.playerColor;
    Field* current = findNeighbourFieldInDirection(startingField, direction);
    cout << "currentPawn: " << currentPawn << endl;

    while (current->object != EMPTYFIELD)
    {
        char tempPawn = current->object;
        current->object = currentPawn;
        currentPawn = tempPawn;
        current = findNeighbourFieldInDirection(current, direction);
    }

    current->object = currentPawn;
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

    Field* startingFieldOnFrame = findFieldOnFrame(board, startChar, startInt);

    if (startingFieldOnFrame == nullptr)
    {
        cout << "BAD_MOVE_" << startChar << startInt << "_IS_WRONG_STARTING_FIELD" << endl;
        return;
    }

    Field* destinationFieldOnFrame = findFieldOnFrame(board, endChar, endInt);
    if (destinationFieldOnFrame != nullptr)
    {
        cout << "BAD_MOVE_" << endChar << endInt << "_IS_WRONG_DESTINATION_FIELD" << endl;
        return;
    }

    Direction neighbourFieldDirection = findNeighbourField(startingFieldOnFrame, endChar, endInt);
    if (neighbourFieldDirection == ERROR)
    {
        cout << "UNKNOWN_MOVE_DIRECTION" << endl;
        return;
    }

    if (!isEmptyFieldInDirection(startingFieldOnFrame, neighbourFieldDirection))
    {
        cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        return;
    }

    movePawns(board, startingFieldOnFrame, neighbourFieldDirection);
    cout << "MOVE_COMITTED" << endl;

    if (board.playerColor == WHITEPAWN)
    {
        board.playerColor = BLACKPAWN;
    }

    else if (board.playerColor == BLACKPAWN)
    {
        board.playerColor = WHITEPAWN;
    }
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

        printBoardState(board.boardState);
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