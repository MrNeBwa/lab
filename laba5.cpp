#include <iostream>
#define ROWS 3
#define COLS 4

int main() {
    long long tmp, sum = 1;
    bool find = false;
    
    int** matrix_start = new int*[ROWS];

    for (int i = 0; i < ROWS; i++){
        matrix_start[i] = new int[COLS];
    }
    
    for (int i = 0; i < ROWS; i++){
            int* current_row_ptr = matrix_start[i]; 
        
        for (int j = 0; j < COLS; j++){
            //std::cout << "Enter element [" << i << "] [" << j << "]: ";
            std::cin >> tmp;
               *current_row_ptr = tmp;
            current_row_ptr++; 
        }
    }

    for (int i = 0; i < ROWS; i++){
        int* current_row_ptr = matrix_start[i];
        
        for (int j = 0; j < COLS; j++){

            if ((*current_row_ptr) % 2 == 0 && (*current_row_ptr) > 0){
                sum *= (*current_row_ptr);
                find = true;
            }
            current_row_ptr++; 
        }
    }
    
    if (find){
        std::cout << sum << "|" << sum << "|" << sum << std::endl;
    }
    else{
        std::cout << "There is no non-negative, even elements" << std::endl;
    }

    for (int i = 0; i < ROWS; i++){
        delete[] matrix_start[i];
    }
    delete[] matrix_start;

    return 0;
}
