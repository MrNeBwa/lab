#include <iostream>
#include <string>

int main(){
  std::string s;
  std::cin >> s;
  std::string out = (s[0] - '0' + s[1] - '0' == s[2] - '0' + s[3] - '0') ? "The sum of 1 and 2 element and of the 3 and the 4 element are equal" : "The sum is not equal";
  std::cout << out << std::endl;
  return 0;
}
