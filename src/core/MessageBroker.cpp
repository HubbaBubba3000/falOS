
#include "MessageBroker.h"
#include "Module.h"
#include <cstdint>
#include <iostream>
namespace core {

    MessageBroker::MessageBroker(std::unordered_map<uint8_t, Module*>* mm) : modulemap(mm){};

    void MessageBroker::SendMessage(uint8_t mid, void* msg) {
        std::cout << mid << msg << "\n";
        if (auto it = modulemap->find(mid); it != modulemap->end()) {
                        it->second->Request(msg);
                    }
    }
}
