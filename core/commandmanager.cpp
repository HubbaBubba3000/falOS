#include "commandmanager.h"
#include "core.h"
#include <functional>
#include <iostream>
#include <string>
using namespace core;

int command_help() {
    std::cout << "helping.. \n" ;
    return 0;
}
int command_shut() {
    Core::shutdown();
    return 0;
}
int command_gr() {
    std::string pwd;
    std::cout << "enter root password: ";
    std::cin >> pwd;

    return 0;
}
// int command_testexec() {
//     std::ifstream file("test.o", std::ios::binary);
//     file.seekg(TEXT_SELECTION_OFFSET )

//     auto e = Executor::Load((unsigned char**)bytecode);
//     e->Execute();
//     return 0;
// }


CommandManager::CommandManager() {

    commands["help"] = command_help;
    commands["shut"] = command_shut;
    //commands["test"] = command_testexec;
}

CommandManager::~CommandManager() {
    commands.clear();
    command.clear();
}



void CommandManager::searchcommand(std::string* command) {
    if (commands.count(command->c_str()) != 0)
        commands[*command]();
    else
    std::cout << "invalid command \n";
}
