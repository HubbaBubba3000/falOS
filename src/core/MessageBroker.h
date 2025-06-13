#pragma once
#include "Module.h"
#include "message.h"
#include <cstdint>
#include <unordered_map>
namespace core {
    class MessageBroker {
        public:
        MessageBroker(std::unordered_map<int, Module*>* mm);

        void SendMessage(int mid, Message msg); // todo void* to msg struct

        private:
            std::unordered_map<int, Module*>* modulemap;
    };
}
