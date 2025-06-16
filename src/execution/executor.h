#pragma once
#include "../core/Module.h"
#include <cstdint>

namespace execution {
    using ExecFunc = void(*)();
    // LINUX ONLY
    class Executor : core::Module {
        public:
        void Request(core::Message msg) override;

        private:
        void* m_handle;
        ExecFunc m_func;

        void LoadExec(unsigned char* data, uint16_t size);
        void* loadFromMemory(const unsigned char* data, size_t size);
        void Run(int argc, char* argv[]);


    };
}
