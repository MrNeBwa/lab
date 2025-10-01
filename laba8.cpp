#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm> 


class Complex {
public:
    double real;
    double imag;

    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    Complex operator+=(const Complex& other)  {
        real += other.real; imag += other.imag;
    }
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }
    Complex operator-=(const Complex& other) {
        real -= other.real; imag -= other.imag;
    }
};

int add(int a, int b) {
    return a + b;
}

Complex add(Complex a, Complex b) {
    return a + b;
}

std::string removeSpaces(const std::string& str) {
    std::string ans = str;
    ans.erase(
        std::remove(ans.begin(), ans.end(), ' '),
        ans.end()
    );
    return ans;
}

Complex parseComplex(const std::string& input) {
    double real = 0.0;
    double imag = 0.0;

    size_t i_pos = input.find_last_of("iI");
    bool is_complex = (i_pos != std::string::npos);

    if (is_complex) {
        size_t sign_pos = input.find_first_of("+-", 1);
        if (sign_pos != std::string::npos) {
            real = std::stod(input.substr(0, sign_pos));
            std::string imag_str = input.substr(sign_pos, i_pos - sign_pos);
            imag = std::stod(imag_str);
        }
        else {
            imag = std::stod(input.substr(0, i_pos));
        }
    }
    else {
        real = std::stod(input);
    }

    return Complex(real, imag);
}

int main() {
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
  










  // TEST
    std::string i1, i2;

    std::cout << "Enter the first number: ";
    std::getline(std::cin, i1);

    std::cout << "Enter the second number: ";
    std::getline(std::cin, i2);

    std::string clean1 = removeSpaces(i1);
    std::string clean2 = removeSpaces(i2);

    bool is_comp1 = (clean1.find('i') != std::string::npos);
    bool is_comp2 = (clean2.find('i') != std::string::npos);

    try {
        if (is_comp1 || is_comp2) {
            Complex c1 = parseComplex(clean1);
            Complex c2 = parseComplex(clean2);
            Complex result = add(c1, c2);
            std::cout << "Result: " << result.real << " + " << result.imag << "i" << std::endl;
        }
        else {
            int i1 = std::stoi(clean1);
            int i2 = std::stoi(clean2);
            int result = add(i1, i2);
            std::cout << "Result: " << result << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Please enter valid numbers" << std::endl;
    }

    return 0;
}

