#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <algorithm>
#include <termios.h>
#include <unistd.h>


using std::pair;
using std::string;


std::string exec(const char* cmd) {
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}
/*
std::string run_command_with_input(const std::string& command, const std::string& input) {
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

        std::string output;
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
*/
class Data{

};

class Task{
private:
    int number;
    pair <bool, char> combined;
public:
    bool execute(){
        string* paths = new string[combined.second];
        auto curpaths = paths;
        for (int i = 0; i < combined.second; i++){
            *curpaths = "./laba" + number + (combined.second != 1) ? + ("_" + (i+1)) : ""; 
            curpaths++;
        }
        curpaths -= combined.second;
    }
    Task () {};
    Task (int num) : number(num), combined({false, 1}) {}; 
    Task (int num, int second) : number(num), combined({true, second}) {};

};


char getch() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int menu(const std::vector<std::string>& options, int selected = 0) {
    while (true) {
        system("clear"); // или std::cout << "\033[2J\033[1;1H";
        std::cout << "Выберете задание:\n";
        for (int i = 0; i < options.size(); ++i) {
            if (i == selected)
                std::cout << " > " << options[i] << "\n";
            else
                std::cout << "   " << options[i] << "\n";
        }
        std::cout << "\nСтрелки - выбор, Enter - подтвердить.\n";
        char c = getch();
        if (c == 27) { // escape
            getch(); // [
            switch(getch()) { // "A"=up "B"=down
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
int submenu(int task_num) {
    std::vector<std::string> choices = { "Ввести значения", "Запустить с заданными" };
    int selected = 0;
    while (true) {
        system("clear");
        std::cout << "Задание: " << task_num << "\n";
        for (int i = 0; i < choices.size(); ++i) {
            if (i == selected)
                std::cout << " > " << choices[i] << "\n";
            else
                std::cout << "   " << choices[i] << "\n";
        }
        std::cout << "\nСтрелки - выбор, Enter - подтвердить.\n";
        char c = getch();
        if (c == 27) {
            getch();
            switch(getch()) {
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



int main(){
    string s = exec("ls"), p;
    std::vector <string> vec;
    std::stringstream t(s);
    while (t >> p){
        vec.push_back(p);
    }
    //clean 1
    vec.erase(std::remove_if(vec.begin(), vec.end(),
               [](const std::string& s) {
                   return s.find('.') != std::string::npos;
               }),
              vec.end());

    //clean 2
    vec.erase(std::remove_if(vec.begin(), vec.end(),
               [](const std::string& s) {
                   return s.find('d') != std::string::npos;
               }),
              vec.end());

    /*
    for (auto c : vec){
        std::cout << c << std::endl;
    }
    */
    int task_num = (vec[size(vec)-1]) [size(vec[size(vec)-1])-1] - '0';
    
    Task* tasks = new Task[task_num];
    
    auto task = tasks;
    
    for (int i = 0; i < task_num; i++){
        if (vec[i].find('_') != std::string::npos)
            *task = Task(i+1);
        else{
            *task = Task(i+1, 2);
        }
        task ++;
    }
    task = tasks; 


    int selected_task = menu(vec);
    int submenu_choice = submenu(selected_task + 1);


    return 0;
}