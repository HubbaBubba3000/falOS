#pragma once
#include "commandmanager.h"
#include "user.h"
#include <string>
namespace core {

    class Core {
        public:

            Core();
            ~Core();

            bool authentification();
            void run();
            bool checkroot(std::string* pwd);

            static void shutdown();

        private:
            static inline bool m_shutdown;
            core::User* user;
            core::CommandManager* cm;

            //get userdata from config
            User* getUser(std::string path);
            User* newUser(std::string* name, std::string* pwd, std::string* root_password );
    };
}
