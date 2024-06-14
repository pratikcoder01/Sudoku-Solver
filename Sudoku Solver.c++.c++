#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
using namespace std;

// Size of Sudoku grid
#define N 9
#define EMPTY 0

// Function prototypes
void printGrid(const vector<vector<int>>& grid);
bool solveSudoku(vector<vector<int>>& grid);
bool isValid(vector<vector<int>>& grid, int row, int col, int num);
bool fillGrid(vector<vector<int>>& grid);
bool findUnassignedLocation(const vector<vector<int>>& grid, int& row, int& col);
void generateRandomSudoku(vector<vector<int>>& grid, int numDigitsToRemove);
void animateLoading();
void solveGeneratedSudoku(vector<vector<int>>& grid);

// Function to print the Sudoku grid
void printGrid(const vector<vector<int>>& grid) {
    cout << " -----------------------" << endl;
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0 && row != 0)
            cout << "|-------+-------+-------|" << endl;
        for (int col = 0; col < N; col++) {
            if (col % 3 == 0)
                cout << "| ";
            if (grid[row][col] == EMPTY)
                cout << ". ";
            else
                cout << grid[row][col] << " ";
        }
        cout << "|" << endl;
    }
    cout << " -----------------------" << endl;
}

// Function to solve Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& grid) {
    int row, col;
    
    // If there are no unassigned positions, Sudoku is solved
    if (!findUnassignedLocation(grid, row, col))
        return true; // success
    
    // Try digits 1 to 9
    for (int num = 1; num <= 9; num++) {
        if (isValid(grid, row, col, num)) {
            // Assign num to current position
            grid[row][col] = num;
            
            // Recur to solve the rest of the grid
            if (solveSudoku(grid))
                return true; // success
            
            // If recursion fails, backtrack and try again
            grid[row][col] = EMPTY;
        }
    }
    return false; // backtrack
}

// Function to find an unassigned position in the grid
bool findUnassignedLocation(const vector<vector<int>>& grid, int& row, int& col) {
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == EMPTY)
                return true;
    return false;
}

// Function to check if a digit can be placed at a given position
bool isValid(vector<vector<int>>& grid, int row, int col, int num) {
    // Check if num is not already present in the current row, column, and 3x3 subgrid
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num || grid[row - row % 3 + x / 3][col - col % 3 + x % 3] == num)
            return false;
    }
    return true;
}

// Function to generate a random Sudoku grid
void generateRandomSudoku(vector<vector<int>>& grid, int numDigitsToRemove) {
    // Fill the grid with a solved Sudoku puzzle
    fillGrid(grid);
    
    // Random number generator
    random_device rd;
    mt19937 gen(rd());
    
    // Shuffle the indices to randomly remove digits
    vector<pair<int, int>> indices;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            indices.emplace_back(i, j);
        }
    }
    shuffle(indices.begin(), indices.end(), gen);
    
    // Remove digits to create the puzzle
    for (int i = 0; i < numDigitsToRemove && i < N * N; ++i) {
        int row = indices[i].first;
        int col = indices[i].second;
        grid[row][col] = EMPTY;
    }
}

// Function to fill the grid with a solved Sudoku puzzle
bool fillGrid(vector<vector<int>>& grid) {
    // Create a solved Sudoku puzzle
    vector<vector<int>> tempGrid(N, vector<int>(N, 0));
    if (!solveSudoku(tempGrid))
        return false;
    
    // Copy the solved puzzle to the grid
    grid = tempGrid;
    return true;
}

// Function to animate loading
void animateLoading() {
    cout << "Generating Sudoku puzzle";
    for (int i = 0; i < 3; ++i) {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << ".";
    }
    cout << endl;
}

// Function to solve the generated Sudoku puzzle
void solveGeneratedSudoku(vector<vector<int>>& grid) {
    vector<vector<int>> copyGrid = grid; // Make a copy of the original grid
    if (solveSudoku(copyGrid)) {
        cout << "Sudoku puzzle solved:" << endl;
        printGrid(copyGrid);
    } else {
        cout << "No solution exists for the generated Sudoku puzzle." << endl;
    }
}

int main() {
    vector<vector<int>> grid(N, vector<int>(N, 0));
    int numDigitsToRemove = 40; // Adjust the number of digits to remove as desired
    cout << "\n\t\t**********************************" << endl;  
    cout << "\t\t*                                *" << endl;
    cout << "\t\t*     Welcome to Sudokusolver    *" << endl;
    cout << "\t\t*                                *" << endl;
    cout << "\t\t*                                *" << endl;
    cout << "\t\t**********************************" << endl;

    animateLoading();
    generateRandomSudoku(grid, numDigitsToRemove);
    printGrid(grid);
    
    cout << "Solving Sudoku puzzle..." << endl;
    solveGeneratedSudoku(grid);
    
    return 0;
}
