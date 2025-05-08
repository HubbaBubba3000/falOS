#include "core/core.h"
using namespace core;
int main() {
    //g_falos_core = *new Core();

    if (g_falos_core.authentification())
        g_falos_core.run();

    return 0;
}
