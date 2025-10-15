#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <sstream> // Добавлено для безопасного парсинга
#define ll long long
#define ld long double

class Complex {
public:
    double real;
    double imag;

    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Операторы остались прежними
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    Complex& operator+=(const Complex& other) { // Изменено на возврат ссылки
        real += other.real; imag += other.imag;
        return *this;
    }
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }
    Complex& operator-=(const Complex& other) { // Изменено на возврат ссылки
        real -= other.real; imag -= other.imag;
        return *this;
    }
};

int add(int a, int b) {
    return a + b;
}

Complex add(Complex a, Complex b) {
    return a + b;
}

// Убран removeSpaces, так как std::cin пропускает пробелы между токенами

Complex parseComplex(const std::string& input) {
    double real = 0.0;
    double imag = 0.0;

    size_t i_pos = input.find_last_of("iI");
    bool is_complex = (i_pos != std::string::npos);

    // Упрощенная логика парсинга для работы с токенами без пробелов
    if (is_complex) {
        // Проверяем, есть ли знак '+' или '-' для разделения действительной и мнимой частей
        // Начинаем поиск со 2-го символа, чтобы не спутать с отрицательным числом
        size_t sign_pos = input.find_first_of("+-", 1); 
        
        if (sign_pos != std::string::npos) {
            // Если есть и действительная и мнимая части (например, 3.0+4.0i)
            // Действительная часть - все до знака
            real = std::stod(input.substr(0, sign_pos));
            // Мнимая часть - от знака до 'i'
            std::string imag_str = input.substr(sign_pos, i_pos - sign_pos);
            // Если imag_str - это просто "+" или "-", stold даст 0, что неправильно. 
            // Это требует более надежного парсинга.
            if (imag_str == "+") {
                imag = 1.0;
            } else if (imag_str == "-") {
                imag = -1.0;
            } else {
                 imag = std::stod(imag_str);
            }

        } else if (input.find_first_of("0123456789.") == std::string::npos) {
            // Если введен только 'i' или '-i' (т.е. 1i или -1i)
            if (input == "i" || input == "I") imag = 1.0;
            else if (input == "-i" || input == "-I") imag = -1.0;
        }
        else {
            // Только мнимая часть (например, 4.0i) или только действительная часть (не должно попадать сюда, но на всякий случай)
            // Поскольку std::stod(input.substr(0, i_pos)) может быть "4.0", реальная часть остается 0.0
            imag = std::stod(input.substr(0, i_pos));
        }
    }
    else {
        real = std::stod(input);
    }

    return Complex(real, imag);
}

int main() {
    std::string s1, s2;

    //std::cout << "Enter the first number: ";
    if (!(std::cin >> s1)) return 1;

    //std::cout << "Enter the second number: ";
    if (!(std::cin >> s2)) return 1;


    bool is_comp1 = (s1.find('i') != std::string::npos || s1.find('I') != std::string::npos);
    bool is_comp2 = (s2.find('i') != std::string::npos || s2.find('I') != std::string::npos);

    try {
        if (is_comp1 || is_comp2) {
            Complex c1 = parseComplex(s1);
            Complex c2 = parseComplex(s2);
            Complex result = add(c1, c2);
            
            std::cout << "Result: " << result.real;
            if (result.imag >= 0) {
                std::cout << " + " << result.imag << "i" << std::endl;
            } else {
                std::cout << " - " << std::fabs(result.imag) << "i" << std::endl;
            }
        }
        else {
            int i1 = std::stoi(s1);
            int i2 = std::stoi(s2);
            int result = add(i1, i2);
            std::cout << "Result: " << result << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Please enter valid numbers (Note: Complex numbers must not contain spaces, e.g., '3.0+4.0i')." << std::endl;
    }

    return 0;
}
