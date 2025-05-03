#pragma once
#include "commandmanager.h"
#include "user.h"
#include "../fs/FS.h"
#include <string>
namespace core {

    class Core {
        public:

            Core();
            ~Core();
            void boot();
            bool authentification();
            void run();
            bool checkroot(std::string* pwd);

            static void shutdown();

        private:
            static inline bool m_shutdown;
            core::User* user;
            core::CommandManager* cm;
            fs::FS* fs;
            //get userdata from config
            User* getUser(std::string path);
            User* newUser(std::string* name, std::string* pwd, std::string* root_password );
    };
}
