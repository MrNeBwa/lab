#include <iostream>
#define ROWS 3
#define COLS 4



int main() {
  int n, comp = 0;
  
  std::cout << "Enter the n amount: ";
  std::cin >> n;
  int* array = new int[n];
  std::cout << "Enter the elements of the array:" << std::endl;
  

    int* ptr = array;
    for (int i = 0; i < n; i++) {
        std::cin >> *ptr;
        ptr++;
    }
    for (int i = 0; i < n; i++) {
        if (*array == -1) {
            comp++;
        }
        array++;
    }
    array -= n;
    delete[] array;

  std::cout << comp << std::endl;
  
  return 0;
}

