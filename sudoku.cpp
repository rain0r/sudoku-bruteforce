#include "sudoku.h"
#include <iostream>

int matrix[9][9];

void print_matrix() {
    std::cout << endl;

    int x,y;
    for(y=0;y<=8;y++)
    {

        if (y%3 == 0 && y != 0) printf("-----------\n");

        for(x=0;x<=8;x++)
        {

            if (x%3 == 0 && x!=0) printf("|");


            printf("");
            if(feld[x][y] == 0) printf(" ");
            else printf("%i",feld[x][y]);
            printf("");
        }

        //Neue Zeile
        printf("\n");

}

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


    return 0;
}

