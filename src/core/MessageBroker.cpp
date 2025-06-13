
#include "MessageBroker.h"
#include "message.h"
#include "Module.h"
#include "../global.h"
#include <iostream>
namespace core {

    MessageBroker::MessageBroker(std::unordered_map<int, Module*>* mm) : modulemap(mm){ };

    void MessageBroker::SendMessage(int mid, Message msg) {
        auto it = modulemap->find(mid) ;
        if (!it->second) perror("module not found");
            it->second->Request(msg);
    }
}
