#include "terminal.h"
#include "core.h"
#include <cstdio>
#include <functional>
#include <iostream>
#include <string>
#include <variant>
using namespace core;

int help(std::string p) {
    std::cout << "helping.. \n" ;
    return 0;
}
int shut(std::string p) {
    g_falos_core.shutdown();
    return 0;
}
int gr(std::string p) {
    std::string pwd;
    std::cout << "enter root password: ";
    std::cin >> pwd;
    g_falos_core.getroot(&pwd);
    return 0;
}
int wherei(std::string p) {
    std::cout << *g_falos_core.currentDirectory() << "\n";
    return 0;
}
int echo(std::string p) {
    std::cout << p << "\n";
    return 0;
}
// int cd(std::string p) {

//     return 0;
// }

Terminal::Terminal() {
    commands["help"] = help;
    commands["shut"] = shut;
    commands["echo"] = echo;
    commands["gr"] = gr;
    commands["wherei"] = wherei;

}

Terminal::~Terminal() {
    commands.clear();
    command.clear();
}

void Terminal::searchcommand(std::string* command) {
    int i = command->find(" ");
    std::string c = ((i == std::variant_npos) ? *command : command->substr(0, i));
    if (commands.count( c.c_str()) != 0) {
            commands[c]((i == std::variant_npos) ? " " : command->substr(i));
    }
    else
    std::cout << "invalid command \n";
}
