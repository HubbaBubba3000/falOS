#include "busybox.h"
#include "../core/MessageBroker.h"
#include "../terminal/command.h"
#include "../core/core.h"
#include <string>
#include "../global.h"


namespace execution {

    int BusyBox::info(std::string p) {
        core::Core::instance().Request(MODULE_TERMINAL, (void*)"falOS ver 0.1 \n");

        return 0;
    }
    void BusyBox::Request(void* msg) {
        auto m = (terminal::Command*)msg;
        int code = commands[m->command](m->params[0]);
    }
    BusyBox::BusyBox(core::MessageBroker* mb) {
        //commands[" *URCMD* "] = [this](std::string p) { return this-> *URCMD* (p);};

        commands["info"] = [this](std::string p) { return this->info(p);};
    }
}
