#pragma once
#include "../core/Module.h"
#include "../core/MessageBroker.h"
#include "command.h"
#include <cstdint>
#include <string>
namespace terminal {
    class Terminal : public core::Module {
        public:
            Terminal(core::MessageBroker* mb);
            ~Terminal();
            void InputCommand();
            void SearchCommand(Command* command);
            void Print(char* s);
            void Request(void* msg) override;

    };
}
