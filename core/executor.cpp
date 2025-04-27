#include "executor.h"
#include <any>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
using namespace core;
Executor* Executor::Load(unsigned char* code[], std::any* paramtype) {
    Executor* e = new Executor();

    e->exec_mem = mmap(0, sizeof(&code), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (e->exec_mem == MAP_FAILED) {
        std::cout << "memory allocation error! \n";
        return nullptr;
    }

    memcpy(e->exec_mem, code, sizeof(&code));

    return e;
}

void Executor::Execute(std::any* params) {
    using exec = int(*)();
    exec func = reinterpret_cast<exec>(exec_mem);

    std::cout << func() << "\n";

    munmap(exec_mem, sizeof(*code));

}

void Executor::Unload() {
    //pass
}
