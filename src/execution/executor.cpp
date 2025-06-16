#include "executor.h"
#include "../core/core.h"
#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sstream>


namespace execution {

    void Executor::Request(core::Message msg) {
        switch(msg.message_type) {
            case EXEC_LOAD:
            //LoadExec(unsigned char *data, uint16_t size)
                break;
            case EXEC_RUN:
                //Run(int argc, char **argv)
                break;
            case EXEC_LOAD_AND_RUN:
            //LoadExec(unsigned char *data, uint16_t size)
            //Run(int argc, char **argv)
                break;
            default:
                std::cerr << "err: invalid message type\n";
        }
    }

    void Executor::LoadExec(unsigned char* data, uint16_t size) {
        m_handle = loadFromMemory(data, size);
        if (!m_handle) {
            std::cerr << "Error: loading plugin: " << dlerror() << "\n";
            return;
        }

        m_func = (ExecFunc)dlsym(m_handle, "main");
        if (!m_func) {
            std::cerr << "Error: finding function 'main' \n";
            dlclose(m_handle);
            return;
        }
        #ifdef DEV
            std::cout << "dev: Execution loaded successfully!\n";
        #endif
    }
    // idk what its doing, ai generated
    void* Executor::loadFromMemory(const unsigned char* data, size_t size) {
        int fd = memfd_create("plugin", 0);
        write(fd, data, size);
        lseek(fd, 0, SEEK_SET);

        char path[64];
        sprintf(path, "/proc/self/fd/%d", fd);
        void* handle = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
        close(fd);

        return handle;
    }
    void Executor::Run(int argc, char* argv[]) {
        std::streambuf* old_cout = std::cout.rdbuf();
        std::stringbuf new_buf;
        std::cout.rdbuf(&new_buf);

        m_func(); // Вызываем функцию плагина

        // Восстанавливаем старый буфер
        std::cout.rdbuf(old_cout);
        std::string output = new_buf.str();
        if (!output.empty()) {
            size_t pos = 0;
            while (pos != std::string::npos) {
                size_t new_pos = output.find('\n', pos);
                std::string line = output.substr(pos, new_pos - pos);
                if (!line.empty()) {
                    core::Message msg = *core::CreateMessage(MODULE_EXECUTOR,MODULE_TERMINAL, (void*)&line);
                    core::Core::instance().Request(MODULE_TERMINAL, msg);
                }
                pos = (new_pos == std::string::npos) ? new_pos : new_pos + 1;
            }
        }
    }

}
