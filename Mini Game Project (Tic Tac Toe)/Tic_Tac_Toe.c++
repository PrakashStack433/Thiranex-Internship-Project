#include <iostream>
#include <vector>

// Function to print the current state of the board
void displayBoard(const std::vector<std::vector<char>>& board) {
    std::cout << "\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << " \n";
        if (i < 2) {
            std::cout << "---|---|---\n";
        }
    }
    std::cout << "\n";
}

// Function to check if a player has won the game
bool checkWin(const std::vector<std::vector<char>>& board, char player) {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

// Function to check if the board is completely full (Draw status)
bool checkDraw(const std::vector<std::vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false; 
            }
        }
    }
    return true;
}

void playGame() {
    // Initializing the grid with numbers 1-9 representing grid spots
    std::vector<std::vector<char>> board = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };
    
    char currentPlayer = 'X';
    bool gameWon = false;
    bool gameDraw = false;

    std::cout << "--- Tic-Tac-Toe Console Game ---\n";
    std::cout << "Player 1 [X]  -  Player 2 [O]\n";

    while (!gameWon && !gameDraw) {
        displayBoard(board);
        int choice;
        std::cout << "Player " << currentPlayer << ", enter a position (1-9): ";
        std::cin >> choice;

        // Input validation for menu streams
        if (std::cin.fail() || choice < 1 || choice > 9) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid spot! Please choose a number between 1 and 9.\n";
            continue;
        }

        // Map choice (1-9) to 2D array indices [row][col]
        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        // Validate if the position is already taken
        if (board[row][col] == 'X' || board[row][col] == 'O') {
            std::cout << "Position already occupied! Try a different square.\n";
            continue;
        }

        // Place the piece
        board[row][col] = currentPlayer;

        // Evaluate state
        gameWon = checkWin(board, currentPlayer);
        if (gameWon) {
            displayBoard(board);
            std::cout << "Congratulations! Player " << currentPlayer << " wins!\n";
            break;
        }

        gameDraw = checkDraw(board);
        if (gameDraw) {
            displayBoard(board);
            std::cout << "It's a draw!\n";
            break;
        }

        // Switch turns
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

int main() {
    char replayChoice;
    do {
        playGame();
        std::cout << "Would you like to play again? (y/n): ";
        std::cin >> replayChoice;
    } while (replayChoice == 'y' || replayChoice == 'Y');

    std::cout << "Thanks for playing!\n";
    return 0;
}