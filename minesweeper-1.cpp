#include <bits/stdc++.h>
using namespace std;

#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526 // (25 * 25 - 99)

int SIDE;   // side length of the board
int MINES;  // number of mines on the board

// A Utility Function to check whether given cell (row, col) is valid
bool isValid(int row, int col)
{
    return (row >= 0 && row < SIDE && col >= 0 && col < SIDE);
}

// A Utility Function to check whether the cell (row, col) has a mine
bool isMine(int row, int col, char board[][MAXSIDE])
{
    return (board[row][col] == '*');
}

// A Function to get the user's move
void makeMove(int* x, int* y)
{
    // Loop until we get valid input
    while (true)
    {
        cout << "Enter your move (row, column) -> ";
        if (!(cin >> *x >> *y))
        {
            // Clear error flags and discard invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter two integers.\n";
            continue;
        }

        // Check if the coordinates are within the valid range
        if (*x < 0 || *x >= SIDE || *y < 0 || *y >= SIDE)
        {
            cout << "Coordinates out of range [0.." << SIDE - 1 << "]. Try again.\n";
            continue;
        }

        // If we reach here, the move is valid
        break;
    }
}

// A Function to print the board in a grid format
void printBoard(char myBoard[][MAXSIDE])
{
    // Print column headers
    cout << "     ";
    for (int c = 0; c < SIDE; c++)
        cout << setw(2) << c << "  ";
    cout << "\n";

    // Print top border
    cout << "   +";
    for (int c = 0; c < SIDE; c++)
        cout << "---+";
    cout << "\n";

    // Print rows
    for (int r = 0; r < SIDE; r++)
    {
        // Row number at the start
        cout << setw(2) << r << " |";

        // Each cell
        for (int c = 0; c < SIDE; c++)
        {
            cout << " " << myBoard[r][c] << " |";
        }
        cout << "\n";

        // Separator after each row
        cout << "   +";
        for (int c = 0; c < SIDE; c++)
            cout << "---+";
        cout << "\n";
    }
}

// A Function to count the number of mines adjacent to (row, col)
int countAdjacentMines(int row, int col, int mines[][2], char realBoard[][MAXSIDE])
{
    int count = 0;

    // Offsets for the 8 possible adjacent cells
    int rowDelta[] = {-1, -1, -1,  0, 0,  1,  1,  1};
    int colDelta[] = {-1,  0,  1, -1, 1, -1,  0,  1};

    // Check all 8 neighbors
    for (int i = 0; i < 8; i++)
    {
        int newRow = row + rowDelta[i];
        int newCol = col + colDelta[i];

        if (isValid(newRow, newCol) && isMine(newRow, newCol, realBoard))
            count++;
    }

    return count;
}

// A Recursive Function to process the player's move
bool playMinesweeperUtil(char myBoard[][MAXSIDE], char realBoard[][MAXSIDE],
                         int mines[][2], int row, int col, int* movesLeft)
{
    // If this cell is already revealed, return false
    if (myBoard[row][col] != '-')
        return false;

    // If the cell is a mine, the player loses
    if (realBoard[row][col] == '*')
    {
        // Reveal all mines
        myBoard[row][col] = '*';
        for (int i = 0; i < MINES; i++)
            myBoard[mines[i][0]][mines[i][1]] = '*';

        printBoard(myBoard);
        cout << "\nYou lost!\n";
        return true; // gameOver = true
    }
    else
    {
        // Count adjacent mines and update the board
        int count = countAdjacentMines(row, col, mines, realBoard);
        (*movesLeft)--;

        myBoard[row][col] = char(count + '0');

        // If no adjacent mines, reveal neighbors recursively
        if (count == 0)
        {
            int rowDelta[] = {-1, -1, -1,  0, 0,  1,  1,  1};
            int colDelta[] = {-1,  0,  1, -1, 1, -1,  0,  1};

            for (int i = 0; i < 8; i++)
            {
                int newRow = row + rowDelta[i];
                int newCol = col + colDelta[i];

                if (isValid(newRow, newCol) && !isMine(newRow, newCol, realBoard))
                {
                    playMinesweeperUtil(myBoard, realBoard, mines, newRow, newCol, movesLeft);
                }
            }
        }

        return false; // gameOver = false
    }
}

// A Function to randomly place mines on the board
void placeMines(int mines[][2], char realBoard[][MAXSIDE])
{
    bool mark[MAXSIDE * MAXSIDE];
    memset(mark, false, sizeof(mark));

    // Place all mines randomly
    for (int i = 0; i < MINES; )
    {
        int random = rand() % (SIDE * SIDE);
        int x = random / SIDE;
        int y = random % SIDE;

        if (!mark[random])
        {
            // Place mine
            mines[i][0] = x;
            mines[i][1] = y;
            realBoard[x][y] = '*';

            mark[random] = true;
            i++;
        }
    }
}

// A Function to initialize the game boards
void initialise(char realBoard[][MAXSIDE], char myBoard[][MAXSIDE])
{
    // Seed the random number generator
    srand((unsigned)time(nullptr));

    // Set all cells to '-'
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            myBoard[i][j] = '-';
            realBoard[i][j] = '-';
        }
    }
}

// A Function to cheat by displaying the real board
void cheatMinesweeper(char realBoard[][MAXSIDE])
{
    cout << "The mines locations are-\n";
    printBoard(realBoard);
}

// A Function to ensure the first move is always safe
// If the first move is on a mine, move that mine elsewhere
void replaceMine(int row, int col, char board[][MAXSIDE])
{
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            // Find the first location without a mine
            if (board[i][j] != '*')
            {
                board[i][j] = '*';
                board[row][col] = '-';
                return;
            }
        }
    }
}

// A Function to run the Minesweeper game
void playMinesweeper()
{
    bool gameOver = false;

    char realBoard[MAXSIDE][MAXSIDE], myBoard[MAXSIDE][MAXSIDE];

    int movesLeft = SIDE * SIDE - MINES;
    int x, y;
    int mines[MAXMINES][2]; // stores (x,y) coordinates of all mines

    initialise(realBoard, myBoard);

    // Place the mines randomly
    placeMines(mines, realBoard);

    /*
    // If you want to cheat and know where mines are, uncomment:
    // cheatMinesweeper(realBoard);
    */

    int currentMoveIndex = 0;
    while (!gameOver)
    {
        cout << "Current Status of Board:\n";
        printBoard(myBoard);

        makeMove(&x, &y);

        // First move is guaranteed to be safe
        if (currentMoveIndex == 0 && isMine(x, y, realBoard))
        {
            replaceMine(x, y, realBoard);
        }
        currentMoveIndex++;

        // Process the move
        gameOver = playMinesweeperUtil(myBoard, realBoard, mines, x, y, &movesLeft);

        // If no mines remain, user wins
        if (!gameOver && movesLeft == 0)
        {
            cout << "\nYou won!\n";
            gameOver = true;
        }
    }
}

// A Function to choose the difficulty level of the game
void chooseDifficultyLevel()
{
    /*
        BEGINNER = 9 x 9 cells, 10 mines
        INTERMEDIATE = 16 x 16 cells, 40 mines
        ADVANCED = 24 x 24 cells, 99 mines
    */

    cout << "Enter the Difficulty Level:\n";
    cout << "Press 0 for BEGINNER (9 x 9 cells, 10 mines)\n";
    cout << "Press 1 for INTERMEDIATE (16 x 16 cells, 40 mines)\n";
    cout << "Press 2 for ADVANCED (24 x 24 cells, 99 mines)\n";

    int level;
    while (true)
    {
        cin >> level;

        // Validate input
        if (!cin || level < 0 || level > 2)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please try again.\n";
            continue;
        }
        break;
    }

    if (level == BEGINNER)
    {
        SIDE = 9;
        MINES = 10;
    }
    else if (level == INTERMEDIATE)
    {
        SIDE = 16;
        MINES = 40;
    }
    else // ADVANCED
    {
        SIDE = 24;
        MINES = 99;
    }
}

// The Driver Program
int main()
{
    chooseDifficultyLevel();
    playMinesweeper();
    return 0;
}
