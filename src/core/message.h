#pragma once
#include <iostream>
#include <string>
#include "../global.h"
namespace core {
    struct Message {
        int module_sender;
        int module_receiver;
        void* payload;
        int message_type;

       #ifdef DEV
            Message(const Message& msg) :
                module_sender(msg.module_sender),
                module_receiver(msg.module_receiver),
                payload(msg.payload),
                message_type(msg.message_type)
            {
                std::cout << "dev: Message Copied\n";
            }
        #endif
    };


    inline Message* CreateMessage(int msender, int mreciever, void* payload, int mtype = 0) {
        Message* msg;
        msg->module_receiver = mreciever;
        msg->module_sender = msender;
        msg->payload = payload;
        msg->message_type = mtype;
        return msg;
    };

}
