#include <iostream>
#include <string>

std::string getLastDirectory(const std::string& fullPath) {
    auto lastsl = fullPath.find_last_of("\\/");

    if (lastsl == std::string::npos) {
        return "";
    }

    if (lastsl == 0) {
        return "/";
    }

    if (lastsl == 2 && fullPath[1] == ':') {
        return "\\";
    }

    auto lastsl1 = fullPath.find_last_of("\\/", lastsl - 1);
    if (lastsl1 == std::string::npos || (lastsl1 == 1 && fullPath[0] == '/')) {
        return "\\";
    }
    return fullPath.substr(lastsl1 + 1, lastsl - lastsl1 - 1);
}

int main() {
    std::string path1;
    //std::cout << "Path: ";
    std::cin >> path1;
    std::cout << " -> Last directory: " << getLastDirectory(path1) << std::endl;
    return 0;
}

