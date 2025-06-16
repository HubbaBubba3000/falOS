#pragma once

#include "MessageBroker.h"
#include "../fs/fs.h"
#include "Module.h"
#include "message.h"
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace core {
    typedef std::unordered_map<int, Module*> modulemap ;
    class Core {
        public:
        Core(const Core&) = delete;
        Core& operator=(const Core&) = delete;
        static Core& instance() {
            static Core instance;
            return instance;
        }
        void Auth();
        void Init();
        void Run();
        void Request(int mid, Message msg) ;
        void Request(Message msg) ;
        bool shutdown = false;
        private:
        ~Core();
        Core();

        std::unique_ptr<MessageBroker> mb;
        modulemap modules;

    };
    //inline std::shared_ptr<Core> g_core; //global
}
