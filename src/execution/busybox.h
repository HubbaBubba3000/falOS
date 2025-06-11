#pragma once
#include "../core/Module.h"
#include "../core/MessageBroker.h"
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
namespace execution {
    typedef std::function<int(std::string)> func_t;
    //
    typedef std::unordered_map<std::string, std::function<int(std::string)>> command_map;
    /// standart commands
    class BusyBox : public core::Module {
        public:
            BusyBox(core::MessageBroker* mb);
            void Request(void* msg) override;
        private:
            command_map commands;
            //command list
            int info(std::string p);
    };
}
