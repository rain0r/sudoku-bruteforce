#include "sudoku.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <boost/multi_array.hpp>

using namespace std;

typedef boost::multi_array<int,2> barray;
barray start_sudoku_field(boost::extents[9][9]);
list<barray> field_stack;
bool solved;
boost::mutex mtx_;
boost::thread_group sudoku_threads;

void init_multi_dim_array() {
    int init_val[9][9] = {
        { 6, 3, 2, 7, 8, 1, 9, 4, 5 },
        { 4, 8, 7, 5, 9, 6, 2, 1, 3 },
        { 5, 1, 9, 2, 4, 3, 8, 7, 6 },
        { 8, 6, 4, 3, 5, 2, 7, 9, 1 },
        { 7, 5, 1, 9, 6, 8, 3, 2, 4 },
        { 2, 9, 3, 1, 7, 4, 6, 5, 8 },
        { 9, 4, 5, 6, 3, 7, 1, 8, 2 },
        { 1, 7, 6, 8, 2, 5, 4, 3, 9 },
        { 3, 2, 8, 4, 1, 9, 5, 0, 0 }
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
                                // cout << "Duplicates in a quadrat: ";
                                // cout << k << " at (";
                                // cout << x_y_quad[i]+1 << "|";
                                // cout << x_y_quad[j]+1 << ")" << endl;
                                return false;
                            }
                            count++;
                        }
                    }
                }


            }
        }
    }
    return true;
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
            int xxx = 0;
            if (get_row_elem_count(field, j, i) > 1) {
                // cout << "Duplicate Number in row " << j+1 << ": " << i << endl;
                return false;
            }
        }

        for (int k = 0; k < 9; k++) {
            if (get_col_elem_count(field, k, i) > 1) {
                // cout << "Duplicate Number in column " << k+1 << ": " << i << endl;
                return false;
            }
        }
    }

    // Check all quadrat
    if (!check_all_quadrat(field)) {
        return false;
    }

    return true;
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
            cout << " " << field[y][x] << " ";
        }
        cout << endl;
    }
}

void solve(barray field) {
    mtx_.lock();
    int count = 0;
    field_stack.push_back(field);

    list<barray>::iterator iter;
    for (iter = field_stack.begin(); iter != field_stack.end(); iter++) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if ((*iter)[i][j] == 0) {
                    for (int k = 1; k <= 9; k++) {
                        barray copy = *iter;
                        // field_stack.erase(iter);
                        copy[i][j] = k;
                        if (is_solved(copy)) {
                            cout << "Solved" << endl;
                            print_matrix(copy);
                            // sudoku_threads.interrupt_all();
                            // return;
                            exit(0);
                        }
                        field_stack.push_back(copy);
                    }
                }
            }
        }
    }
    mtx_.unlock();
}

int main(int argc, char* argv[]) {
    // boost::timer::auto_cpu_timer timer;
    cout << "Blatt01/Aufgabe 2" << endl;

    int thread_count = 4;

    init_multi_dim_array();
    // solve(start_sudoku_field);

    for (int i = 0; i != thread_count; i++) {
        boost::thread *t1 = new boost::thread(solve, start_sudoku_field);
        sudoku_threads.add_thread(t1);
    }

    sudoku_threads.join_all();

    return 0;
}

