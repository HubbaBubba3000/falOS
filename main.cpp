#include "core/core.h"
using namespace core;
int main() {

    Core core = *new Core();

    if (core.authentification())
        core.run();

    return 0;
}
