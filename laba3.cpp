#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
  std::string s;
  int _n, _a;
  std::cout << "Enter n number: " << std::endl;
  std::getline(std::cin, s);
  
  try{
    _n = std::stoi(s);
  }
  catch (std::exception& e){
    return 1;
  }

  std::cout << "Enter a value: " << std::endl;
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
  
  std::cout << std::endl << "The second task!" << std::endl;

  const double a = 0.01;
  double sum = 0.0;
  int n = 1;
  double term;
  do {
     term = pow(-1, n) * (2.0 * n + 1.0) / (pow(n, 3) * (n + 1.0));
     sum += term;
     std::cout << "n = " << n << ", term = " << std::fixed 
     << std::setprecision(10) << term << ", current sum = " << sum << std::endl;
     n++;

  } while (fabs(term) >= a);

  
  std::cout << "\nFinal sum of the series: " << std::fixed << std::setprecision(10) << sum << std::endl;

  return 0;
}

