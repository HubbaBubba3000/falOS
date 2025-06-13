#pragma once
#include "../core/Module.h"
#include "../core/MessageBroker.h"
#include "command.h"
#include <cstdint>
#include <string>
namespace terminal {
    class Terminal : public core::Module {
        public:
            Terminal();
            ~Terminal();
            void InputCommand();
            void SearchCommand(Command* command);
            void Print(char* s);
            void Request(core::Message msg) override;

    };
}
