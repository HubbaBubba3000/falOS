
#include "core.h"
#include "commandmanager.h"
#include <iostream>
#include "../fs/FS.h"
#include <string>

using namespace core;

Core::Core() {
    m_shutdown = false;
    boot();
}

Core::~Core() {
    delete cm;
}

void Core::boot() {
    fs = new fs::FS();

    user = getUser("user.cfg");

    cm = new core::CommandManager();
}

//TODO implement hiding iser input (win/linux)
bool Core::authentification() {
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
        std::cout << "sys: u already root";
        return true;
    }
    if (user->root_pwd == *pwd)
        return true;
    std::cout << "sys: incorrect password";
    return false;
}

void Core::run() {

    while (!m_shutdown) {
        std::cout << ">" ;
        std::cin >> cm->command;
        cm->searchcommand(&cm->command);

    }

    std::cout << "falOS shutdown..";
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
