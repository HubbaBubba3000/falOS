#pragma once

#include "MessageBroker.h"
#include "../fs/fs.h"
#include "Module.h"
#include <cstdint>
#include <unordered_map>

namespace core {
    typedef std::unordered_map<uint8_t, Module*> modulemap ;
    class Core {
        public:
        Core();
        ~Core();

        static Core& instance() {
            static Core instance;
            return instance;
        }
        void Request(uint8_t mid, void* msg) ;

        private:

        MessageBroker* mb;
        modulemap modules;

    };
}
