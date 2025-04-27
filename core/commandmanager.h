#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
namespace core {
    typedef std::function<int()> func_t;
    typedef std::unordered_map<std::string, func_t> command_map;
    class CommandManager {

        public:
            std::string command;

            command_map commands = {};

            CommandManager();
            ~CommandManager();

            void searchcommand(std::string* command);


    };

}
