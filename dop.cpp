#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <termios.h>
#include <sys/wait.h>

using std::pair;
using std::string;
using std::vector;

string execute(const char* cmd) {
    string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}

string run_with_input(const string& command, const string& input) {
    int in_pipe[2];
    int out_pipe[2];
    pipe(in_pipe);
    pipe(out_pipe);

    pid_t pid = fork();

    if (pid == 0) {
        dup2(in_pipe[0], STDIN_FILENO);
        dup2(out_pipe[1], STDOUT_FILENO);
        close(in_pipe[1]);
        close(out_pipe[0]);

        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
        perror("execl failed");
        exit(1);
    } else {
        close(in_pipe[0]);
        close(out_pipe[1]);

        write(in_pipe[1], input.c_str(), input.size());
        close(in_pipe[1]);

        string output;
        char buffer[128];
        ssize_t count;
        while ((count = read(out_pipe[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[count] = '\0';
            output += buffer;
        }
        close(out_pipe[0]);
        waitpid(pid, NULL, 0);

        return output;
    }
}

class Data {};

class Task {
private:
    int number;
    pair <bool, int> combined;
public:
    string get_path(int index) const {
        std::stringstream ss;
        ss << "./laba" << number;
        if (combined.second != 1) {
            ss << "_" << (index + 1);
        }
        return ss.str();
    }

    bool execute(const string& input_data = "") {
        if (!combined.first) {
            string path = get_path(0);
            std::cout << "Executing: " << path << "\n";
            string output = run_with_input(path, input_data);
            std::cout << "Output:\n" << output << std::endl;
            return true;
        } else {
            for (int i = 0; i < combined.second; ++i) {
                string path = get_path(i);
                std::cout << "Executing: " << path << "\n";
                string output = run_with_input(path, input_data);
                std::cout << "Output:\n" << output << std::endl;
            }
            return true;
        }
    }
    Task () {}
    Task (int num) : number(num), combined({false, 1}) {}
    Task (int num, int second) : number(num), combined({true, second}) {}
};


char getch_elegant() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int show_menu(const vector<string>& options, int selected = 0) {
    while (true) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Выберете задание:\n";
        for (int i = 0; i < options.size(); ++i) {
            if (i == selected)
                std::cout << " \033[1;32m> " << options[i] << "\033[0m\n";
            else
                std::cout << "   " << options[i] << "\n";
        }
        std::cout << "\n\033[1;36mСтрелки\033[0m - выбор, \033[1;36mEnter\033[0m - подтвердить.\n";
        char c = getch_elegant();
        if (c == 27) {
            getch_elegant();
            switch(getch_elegant()) {
            case 'A':
                selected = (selected - 1 + options.size()) % options.size();
                break;
            case 'B':
                selected = (selected + 1) % options.size();
                break;
            }
        } else if (c == '\n') {
            return selected;
        }
    }
}

int show_submenu(string task_name) {
    vector<string> choices = { "Ввести значения", "Запустить с заданными" };
    int selected = 0;
    while (true) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Задание: \033[1;33m" << task_name << "\033[0m\n";
        for (int i = 0; i < choices.size(); ++i) {
            if (i == selected)
                std::cout << " \033[1;32m> " << choices[i] << "\033[0m\n";
            else
                std::cout << "   " << choices[i] << "\n";
        }
        std::cout << "\n\033[1;36mСтрелки\033[0m - выбор, \033[1;36mEnter\033[0m - подтвердить.\n";
        char c = getch_elegant();
        if (c == 27) {
            getch_elegant();
            switch(getch_elegant()) {
            case 'A':
                selected = (selected - 1 + choices.size()) % choices.size();
                break;
            case 'B':
                selected = (selected + 1) % choices.size();
                break;
            }
        } else if (c == '\n') {
            return selected;
        }
    }
}

string get_data_from_file(const string& filename) {
    std::ifstream inputf ("./data/" + filename);
    if (!inputf) {
        std::cerr << "\033[1;31mError:\033[0m Could not open data file: " << "./data/" + filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << inputf.rdbuf();
    return buffer.str();
}

vector<string> get_executables() {
    string ls_output = execute("ls");
    vector<string> all_files;
    std::stringstream ss(ls_output);
    string p;
    while (ss >> p){
        all_files.push_back(p);
    }

    all_files.erase(std::remove_if(all_files.begin(), all_files.end(),
                   [](const string& s) {
                       return s.find('.') != string::npos;
                   }),
                   all_files.end());

    all_files.erase(std::remove_if(all_files.begin(), all_files.end(),
                   [](const string& s) {
                       return s.find('d') != string::npos;
                   }),
                   all_files.end());
    return all_files;
}

int main(){
    vector<string> executables = get_executables();

    if (executables.empty()) {
        std::cout << "\033[1;31mNo executables found.\033[0m" << std::endl;
        return 1;
    }

    int task_count = 0;
    for (const auto& name : executables) {
        if (name.rfind("laba", 0) == 0) {
            size_t num_start = name.find_first_of("0123456789");
            if (num_start != string::npos) {
                string num_str = name.substr(num_start);
                // Extract number until non-digit or end of string
                size_t non_digit = num_str.find_first_not_of("0123456789");
                if (non_digit != string::npos) {
                    num_str = num_str.substr(0, non_digit);
                }
                
                try {
                    int current_num = std::stoi(num_str);
                    if (current_num > task_count) {
                        task_count = current_num;
                    }
                } catch (const std::exception& e) {
                    // Ignore parsing errors for non-standard names
                }
            }
        }
    }


    std::unique_ptr<Task[]> tasks = std::make_unique<Task[]>(task_count);

    for (int i = 0; i < task_count; ++i) {
        string base_name = "laba" + std::to_string(i + 1);
        bool has_combined = std::find(executables.begin(), executables.end(), base_name + "_1") != executables.end();

        if (has_combined) {
            int count = 0;
            while (std::find(executables.begin(), executables.end(), base_name + "_" + std::to_string(count + 1)) != executables.end()) {
                count++;
            }
            tasks[i] = Task(i + 1, count);
        } else {
            tasks[i] = Task(i + 1);
        }
    }


    int selected_task_index = show_menu(executables);
    string selected_task_name = executables[selected_task_index];
    int submenu_choice = show_submenu(selected_task_name);

    string input_data = "";

    if (submenu_choice == 0) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Введите входные данные для \033[1;33m" << selected_task_name << "\033[0m:\n";
        
        // This is the fix: read the entire input stream, line by line
        std::stringstream user_input_ss;
        string line;
        
        // Temporarily set terminal to canonical mode to allow normal line input
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        while (std::getline(std::cin, line) && !line.empty()) {
            user_input_ss << line << "\n";
        }
        
        // Restore terminal to non-canonical mode for menu
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        input_data = user_input_ss.str();

        // Clear remaining input buffer in case of extra characters
        // while ((line = getch_elegant()) != '\n' && line != EOF); 
        // A simple flush for safety might be better here, but often unnecessary in modern C++ when switching modes
    } else {
        input_data = get_data_from_file(selected_task_name);
        if (input_data.empty()) {
             std::cout << "\033[1;31mError:\033[0m Could not load data or data file is empty. Proceeding with empty input." << std::endl;
        } else {
            std::cout << "\033[2J\033[1;1H";
            std::cout << "\033[1;34mInput data from file:\033[0m\n" << input_data << std::endl;
            std::cout << "\n\033[1;36mPress Enter to continue...\033[0m";
            getch_elegant();
        }
    }

    std::cout << "\033[2J\033[1;1H";

    string final_command = "./" + selected_task_name;
    string output = run_with_input(final_command, input_data);

    std::cout << "\033[1;35m--- Execution Result ---\033[0m\n";
    std::cout << output;
    std::cout << "\033[1;35m------------------------\033[0m\n";
    std::cout << "\n\033[1;36mPress Enter to exit...\033[0m";
    getch_elegant();

    return 0;
}
