#include <iostream>
#define ROWS 3
#define COLS 4




int main(){
    int matrix[ROWS][COLS];

    std::cout << "Enter " << ROWS * COLS << " integers for the matrix:\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << "Enter element [" << i << "][" << j << "]: ";
            std::cin >> matrix[i][j];
        }
    }
    std::cout << std::endl;
    int result_array[ROWS];
    for (int i = 0; i < ROWS; ++i) {
        long long production = 1;
        bool found = false; 

        for (int j = 0; j < COLS; ++j) {
            int current = matrix[i][j];
            if (current > 0 && current % 2 == 0) {
                production *= current;
                found = true;
            }
        }

        if (found) {
            result_array[i] = production;
        }
        else {
            result_array[i] = 0; 
        }
    }

    std::cout << "Resulting 1D array:\n";
    for (long long element : result_array) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
