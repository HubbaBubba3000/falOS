#include "core.h"
#include "MessageBroker.h"
#include "../terminal/terminal.h"
#include "../execution/busybox.h"
#include "../global.h"

namespace core {

    Core::Core() {
        mb = new MessageBroker(&modules);
        modules[MODULE_TERMINAL] = new terminal::Terminal(mb);
        modules[MODULE_BUSYBOX] = new execution::BusyBox(mb);

        while (true) {
            ((terminal::Terminal*)modules[MODULE_TERMINAL])->InputCommand();
        }

    }
    void Core::Request(uint8_t mid, void* msg) {
        mb->SendMessage(mid, msg);
    }
    Core::~Core() {}
}
