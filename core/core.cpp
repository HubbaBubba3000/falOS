
#include "core.h"
#include "terminal.h"
#include <iostream>
#include "../fs/FS.h"
#include <string>

using namespace core;

inline void sysinfo(const char* s) {
    std::cout << "sys: " << s << "\n";
}

Core::Core() {
    m_shutdown = false;
    boot();
}

Core::~Core() {
    delete cm;
    delete fs;
}

void Core::boot() {
    fs = new fs::FS(); // generate root not nullable pointer
    m_current_folder = fs->root;
    std::cout << m_current_folder->name << "\n";
    user = getUser("user.cfg");

    cm = new core::Terminal();
}

//TODO implement hiding iser input (win/linux)
bool Core::authentification() {
    std::cout << " _______ _______ _       _______ _________" << std::endl;
    std::cout << " (  ____ (  ___  |\\     (  ___  |  ____ |" << std::endl;
    std::cout << " | (    \\/ (   ) | (     | (   ) | (    \\/" << std::endl;
    std::cout << " | (__   | (___) | |     | |   | | (_____ " << std::endl;
    std::cout << " |  __)  |  ___  | |     | |   | (_____  )" << std::endl;
    std::cout << " | (     | (   ) | |     | |   | |     ) |" << std::endl;
    std::cout << " | )     | )   ( | (____/\\ (___) /\\____) |" << std::endl;
    std::cout << " |/      |/     \(_______(_______)_______)" << std::endl;

    if (!user) {
        std::string name, pwd, rootpwd;
        std::cout << "welcome to falOS \n please enter ur name: ";
        std::cin >> name;
        std::cout << "enter ur password :  ";
        std::cin >> pwd;
        std::cout << "enter ur root password : ";
        std::cin >> rootpwd;

        user = newUser(&name, &pwd, &rootpwd);
        return true;
    }
    std::string pwd;
    std::cout << "welcome back " << user->name;
    std::cout << "enter ur password: ";
    std::cin >> pwd ;
    if (pwd != user->pwd) { // implement hash comparsion
        std::cout << "invalid password";
        return false;
    }

    return true;
}

void Core::shutdown(){
    m_shutdown = true;
}
bool Core::checkroot(std::string* pwd) {
    if (user->isroot) {
        sysinfo("u already root");
        return true;
    }
    if (user->root_pwd == *pwd)
        return true;
    sysinfo("incorrect password");
    return false;
}
void Core::getroot(std::string* pwd) {
    if (checkroot(pwd)) {
        user->isroot = true;
        sysinfo("now you root!");
    }
}

std::string* Core::currentDirectory() {
    return m_current_folder->name;
}
// void Core::setdirectory() {
//     m_current_folder =
// }
void Core::run() {

    while (!m_shutdown) {
        std::cout << ">" ;
        std::getline(std::cin, cm->command);
        cm->searchcommand(&cm->command);

    }

    std::cout << "falOS shutdown.. ";
}

User* Core::getUser(std::string path ) {
    auto u = new User();
        u->name = "user";
        u->pwd = "user";
        u->root_pwd = "root";
    return u;
}

User* Core::newUser(std::string* name, std::string* pwd, std::string* root_password ) {
    auto u = new User();
    u->name = *name;
    u->pwd = *pwd;
    u->root_pwd = *root_password;
    return u;
}
