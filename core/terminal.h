#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
namespace core {
    typedef std::function<int(std::string)> func_t;
    typedef std::unordered_map<std::string, func_t> command_map;

    class Terminal {

        public:
            std::string command;

            command_map commands = {};

            Terminal();
            ~Terminal();

            void searchcommand(std::string* command);


    };

}
