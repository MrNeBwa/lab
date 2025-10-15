#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> getMonthsForSeason(const std::string& season) {
    std::string lowerSeason;
    for (char c : season) {
        lowerSeason += tolower(c);
    }

    if (lowerSeason == "spring") {
        return { "March", "April", "May" };
    }
    else if (lowerSeason == "summer") {
        return { "June", "July", "August" };
    }
    else if (lowerSeason == "autumn") {
        return { "September", "October", "November" };
    }
    else if (lowerSeason == "winter") {
        return { "December", "January", "February" };
    }
    else {
        return {}; 
    }
}
void writeMonthsToFile(const std::vector<std::string>& months, const std::string& filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << "Months:\n";
        for (const std::string& month : months) {
            file << "- " << month << "\n";
        }
        std::cout << "Months succsesfully writed down" << filename << std::endl;
        file.close();
    }
    else {
        std::cerr << "Error: can`t open a file for writing\n";
    }
}

int main() {
    std::string seasonInput;
    std::cout << "Write a season (spring, summer, autumn, winter): ";
    std::cin >> seasonInput;

    std::vector<std::string> months = getMonthsForSeason(seasonInput);

    if (!months.empty()) {
        std::cout << "Write months to file? (Y/n): ";
        char c;
        std::cin >> c;
        if (c == 'Y' || c == 'y')
            writeMonthsToFile(months, "months.txt");
        std::cout << "\nMonths:\n";
        for (auto c : months) {
            std::cout << "--> " << c << std::endl;
        }
    }
    else {
        std::cout << "Write valid season\n";
    }

    return 0;
}

