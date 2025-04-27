#pragma once

#include <string>
namespace core {
    class User {
        public:
        std::string name;
        std::string pwd; // todo encryption
        std::string root_pwd;
        bool isroot = false;

    };

}
