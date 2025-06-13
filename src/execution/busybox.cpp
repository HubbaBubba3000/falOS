#include "busybox.h"
#include "../core/core.h"
#include <iostream>
#include <string>
#include "../global.h"


namespace execution {

    int BusyBox::info(std::string p) {
        core::Message msg;
        msg.module_sender = MODULE_BUSYBOX;
        msg.module_receiver = MODULE_TERMINAL;
        msg.payload = "falOS ver 0.1";
        core::Core::instance().Request(MODULE_TERMINAL, msg);

        return 0;
    }
    int BusyBox::shut(std::string p) {
        core::Core::instance().shutdown = true;
        return 0;
    }
    void BusyBox::Request(core::Message msg) {
        auto m = msg;
        int code;
        if (auto it = commands.find(m.payload); it == nullptr) {
            std::cout << "command not found";
            code = 1;
            return;
        }

        code = commands[m.payload]("");
    }
    BusyBox::BusyBox() {
        //commands[" *URCMD* "] = [this](std::string p) { return this-> *URCMD* (p);};
        commands["shut"] = [this](std::string p) { return this->shut(p);};
        commands["info"] = [this](std::string p) { return this->info(p);};
    }
}
