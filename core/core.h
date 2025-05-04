#pragma once
#include "terminal.h"
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
            void getroot(std::string* pwd);
            void shutdown();
            std::string* currentDirectory();

        private:
            bool m_shutdown;
            core::User* user;
            core::Terminal* cm;
            fs::FS* fs;
            fs::Folder* m_current_folder;
            //get userdata from config
            User* getUser(std::string path);
            User* newUser(std::string* name, std::string* pwd, std::string* root_password );
    };

    inline Core g_falos_core; // global variable
}
