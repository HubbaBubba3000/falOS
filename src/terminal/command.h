#pragma once

#include <string>
#include <vector>
namespace terminal {
    struct Command {
        std::string command;
        std::vector<std::string> params;
    };

}
