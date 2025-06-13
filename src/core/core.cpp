#include "core.h"
#include "MessageBroker.h"
#include "../terminal/terminal.h"
#include "../execution/busybox.h"
#include "../global.h"
#include "message.h"
#include <memory>

namespace core {

    Core::Core() {}
    void Core::Init() {
        modules[MODULE_TERMINAL] = new terminal::Terminal();
        modules[MODULE_BUSYBOX] = new execution::BusyBox();

        mb = std::make_unique<MessageBroker>(&modules);
    }
    void Core::Run() {
        while (!shutdown) {
            ((terminal::Terminal*)modules[MODULE_TERMINAL])->InputCommand();
        }

    }
    void Core::Request(int mid, Message msg) {
        mb->SendMessage(mid, msg);
    }
    Core::~Core() {}

}
