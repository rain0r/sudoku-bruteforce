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

void init_multi_dim_array() {
    int init_val[9][9] = {
        { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        { 2, 3, 4, 5, 6, 7, 8, 9, 1 },
        { 3, 4, 5, 6, 7, 8, 9, 1, 2 },
        { 4, 5, 6, 7, 8, 9, 1, 2, 3 },
        { 5, 6, 7, 8, 9, 1, 2, 3, 4 },
        { 6, 7, 8, 9, 1, 2, 3, 4, 5 },
        { 7, 0, 0, 5, 0, 0, 4, 0, 0 },
        { 8, 9, 1, 2, 3, 4, 5, 6, 7 },
        { 9, 1, 2, 3, 4, 5, 6, 7, 0 }
    };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            start_sudoku_field[i][j] = init_val[i][j];
        }
    }

}

// Count the appearance of no in row
int get_row_elem_count(barray field, int row, int no) {
    int count = 0;

    for (int i = 0; i < 9; i++) {
        if (field[row][i] == no) {
            count++;
        }
    }

    return count;
}

// Count the appearance of no in col
int get_col_elem_count(barray field, int col, int no) {
    int count = 0;

    for (int i = 0; i < 9; i++) {
        if (field[i][col] == no) {
            count++;
        }
    }

    return count;
}

// checks for duplicates in each quadrat
// returns true if there arent any
bool check_all_quadrat(barray field) {
    int x_y_quad[3] = { 0, 3, 6 };
    int count = 0;

    for (int k = 1; k <= 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                count = 0;
                for (int y = x_y_quad[i]; y <= (x_y_quad[i]+2); y++) {
                    for (int x = x_y_quad[j]; x <= (x_y_quad[j]+2); x++) {
                        if (k == field[x][y]) {
                            if (count > 0) {
                                cout << "Duplicates in a quadrat: ";
                                cout << k << " at (";
                                cout << x_y_quad[i]+1 << "|";
                                cout << x_y_quad[j]+1 << ")" << endl;
                                return false;
                            }
                            count++;
                        }
                    }
                }


            }
        }
    }
}

bool is_solved(barray field) {
    // checks for zeroed fields
    // any field[x][y] == 0 means the game is not yet finished
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (field[i][j] == 0) {
                return false;
            }
        }
    }

    // check all cols and rows
    for (int i = 1; i <= 9; i++) {

        for (int j = 0; j < 9; j++) {
            if (get_row_elem_count(field, j, i) > 1) {
                cout << "Duplicate Number in row " << j+1 << ": " << i << endl;
                return false;
            }
        }

        for (int k = 0; k < 9; k++) {
            if (get_col_elem_count(field, k, i) > 1) {
                cout << "Duplicate Number in column " << k+1 << ": " << i << endl;
                return false;
            }
        }
    }

    // Check all quadrat
    if (!check_all_quadrat(field)) {
        return false;
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
    int count = 0;
    field_stack.push_back(field);

    list<barray>::iterator iter;
    for (iter = field_stack.begin(); iter != field_stack.end(); iter++) {
        if (is_solved(*iter)) {
            cout << "Solved" << endl;
            return;
        }

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if ((*iter)[i][j] == 0) {
                    for (int k = 1; k <= 9; k++) {
                        barray copy = *iter;
                        copy[i][j] = k;
                        field_stack.push_back(copy);
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    cout << "Blatt01/Aufgabe 2" << endl;

    boost::lockfree::queue<int> queue(128);
    int thread_count = 1;
    boost::thread_group sudoku_threads;

    init_multi_dim_array();
    algo(start_sudoku_field);

    for (int i = 0; i != thread_count; i++) {
        sudoku_threads.add_thread(new boost::thread(algo, start_sudoku_field));
    }

    return 0;
}

