#pragma once
#include "Module.h"
#include <cstdint>
#include <unordered_map>
namespace core {
    class MessageBroker {
        public:
        MessageBroker(std::unordered_map<uint8_t, Module*>* mm);

        void SendMessage(uint8_t mid, void* msg); // todo void* to msg struct

        private:
            std::unordered_map<uint8_t, Module*>* modulemap;
    };
}
