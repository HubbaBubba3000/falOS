#pragma once

#include <any>
#include <cstddef>
#include <string>
namespace core {
    class Executor {

        void* exec_mem;
        unsigned char* code[];

        public:
        // TODO params not implemented
        static Executor* Load(unsigned char* code[], std::any* paramtype = nullptr);
        void Execute(std::any* params = NULL);
        void Unload();
    };

}
