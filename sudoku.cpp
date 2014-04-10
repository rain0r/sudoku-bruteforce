#include "sudoku.h"
#include <iostream>

int matrix[9][9]= { {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0, 0}
                  };

void sudoku() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            if (matrix[i][j] == 0) {
                for (int k = 1; k <= 9; k++) {
                    if (matrix[i][j] != k) {
                        matrix[i][j] = k;
                    }
                }
            }

        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Blatt01/Aufgabe 2" << std::endl;

    sudoku();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << matrix[i][j] << std::endl;
        }
    }

    return 0;
}

