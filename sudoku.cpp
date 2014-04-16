#include "sudoku.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/multi_array.hpp>

using namespace std;

typedef boost::multi_array<int,2> barray;
barray start_sudoku_field(boost::extents[9][9]);
list<barray> field_stack;
bool end = false;

void init_multi_dim_array() {
    int init_val[9][9] = {
        { 1, 0, 0, 4, 0, 0, 5, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 4, 0, 0, 8, 0, 0, 9, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 7, 0, 0, 5, 0, 0, 4, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            start_sudoku_field[i][j] = init_val[i][j];
        }
    }

}

bool solved(barray field) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (field[i][j] == 0) {
                return false;
            }
        }
    }


}

void print_matrix(barray field) {
    cout << endl;

    int x,y;
    for(y=0; y<=8; y++) {
        if (y%3 == 0 && y != 0) {
            cout << "---------------------------------" << endl;
        }
        for(x=0; x<=8; x++) {
            if (x%3 == 0 && x!=0) {
                cout << " | ";
            }
            cout << " " << field[x][y] << " ";
        }
        cout << endl;
    }
}

void algo(barray field) {


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (field[i][j] == 0) {
                for (int k = 1; k <= 9; k++) {
                    barray copy = field;
                    copy[i][j] = k;
                    field_stack.push_back(copy);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    cout << "Blatt01/Aufgabe 2" << endl;

    init_multi_dim_array();
    field_stack.push_back(start_sudoku_field);

    while (!end) {
        algo(start_sudoku_field);
        print_matrix( field_stack.back() );
    }

    return 0;
}

