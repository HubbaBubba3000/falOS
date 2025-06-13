#pragma once
#include <string>
namespace core {
    struct Message {
        int module_sender;
        int module_receiver;
        std::string payload;
    };
}
