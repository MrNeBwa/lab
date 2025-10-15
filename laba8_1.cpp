#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm> 
#include <cmath>
#define ll long long
#define ld long double




int main(){
  ll n;
  std::cout << "Enter natural number: ";
  std::cin >> n;
  if (n <= 0) {
       std::cout << "Only natural numbers" << std::endl;
       return 1;
  }
  ld cbrt_n = cbrtl(n);
  ll k_approx = static_cast<ll>(round(cbrt_n));
  bool found = false;
  for (ll k = k_approx - 3; k <= k_approx + 3; ++k) {
      if (k > 0) {
          ll product = k;
          product *= (k + 1);
          product *= (k + 2);
          if (product == n) {
              std::cout << n << " = " << k << " * " << (k + 1) << " * " << (k + 2) << std::endl;
              found = true;
              break;
          }
      }
  }
  if (!found) {
      std::cout << "This task is impossible" << std::endl;
  }
  
}
