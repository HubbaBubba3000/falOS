#include "terminal.h"
#include "command.h"
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include "../global.h"
#include "../core/core.h"

namespace terminal {
    Terminal::Terminal() {
        std::ios_base::sync_with_stdio(false);
    }
    Terminal::~Terminal() {

    }
    void Terminal::InputCommand() {
        std::string line;
        std::cout << "\n>" ;
        std::getline(std::cin, line);
        Command c;
        int i = line.find(" "); // end command pos
        c.command = ((i == std::variant_npos) ? line : line.substr(0, i));
        c.params = *new std::vector<std::string>(1);
        std::string ps = (i == std::variant_npos) ? " " : line.substr(i+1);
        SearchCommand(&c);
    }
    void Terminal::SearchCommand(Command* command) {
        core::Message* msg = core::CreateMessage(MODULE_TERMINAL, MODULE_BUSYBOX, (void*)command);
        core::Core::instance().Request(*msg);
    }

    void Terminal::Print(char* s) {
        std::cout << s;
    }

    void Terminal::Request(core::Message msg) {
        std::cout << (char*)msg.payload;
    }

}
