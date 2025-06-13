#pragma once

#include "message.h"
namespace core {
    class Module {
        public:
            virtual void Request(Message msg) {};
    };
}
