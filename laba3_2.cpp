
#include <iostream>
#include <cmath>
#include <iomanip>


int main(){
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
