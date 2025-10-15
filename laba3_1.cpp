#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
  std::string s;
  int _n, _a;
  //std::cout << "Enter n number: " << std::endl;
  std::getline(std::cin, s);
  
  try{
    _n = std::stoi(s);
  }
  catch (std::exception& e){
    return 1;
  }

  //std::cout << "Enter a value: " << std::endl;
  std::getline(std::cin, s);
  try{
    _a = std::stoi(s);
  }
  catch (std::exception& e){
    return 1;
  }
  int tmp = _a;
  for (int i = 1; i < _n; i++){
    _a += tmp;
  }
  std::cout << "The summed up a value: " << _a << std::endl;

  return 0;
}

