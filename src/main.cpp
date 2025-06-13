#include "core/core.h"
int main() {
    core::Core::instance().Init();
    core::Core::instance().Run();
    return 0;
}
