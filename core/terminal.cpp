#include "terminal.h"
#include "core.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <variant>
using namespace core;

inline void sysinfo(const char* s) {
    std::cout << "sys: " << s << "\n";
}

#pragma region commands

    int shut(std::string p) {
        g_falos_core.shutdown();
        return 0;
    }
    int gr(std::string p) {
        std::string pwd;
        std::cout << "enter root password: ";
        std::cin >> pwd;
        g_falos_core.getroot(&pwd);
        return 0;
    }
    int wherei(std::string p) {
        std::cout << *g_falos_core.currentDirectory() << "\n";
        return 0;
    }
    int echo(std::string p) {
        std::cout << p << "\n";
        return 0;
    }
    int ls(std::string p) {
        auto folderlist = g_falos_core.m_current_folder->folders;
        std::cout << " -- Folders -- \n";
        if (!folderlist.empty())
            for(auto f : folderlist)
                std::cout << f->name << " " << f;
        std::cout << "\n -- Files -- \n";
        auto filelist = g_falos_core.m_current_folder->files;
        if (!filelist.empty())
            for(auto f : filelist)
                std::cout << f->name << " " << f;
        std::cout << "\n";

        return 0;
    }

    int mkdir(std::string p) {
        auto f = g_falos_core.fs->createFolder(g_falos_core.m_current_folder, p);
        g_falos_core.fs->saveMetaFolder(f);
        return 0;
    }
    int cd(std::string p) {
        fs::Folder* f = g_falos_core.fs->getFolderByPath(p,g_falos_core.m_current_folder);
        if (!f)
            return 1;
        g_falos_core.m_current_folder = f;
        return 0;
    }
    int rm(std::string p) {
        fs::Folder* f =  g_falos_core.fs->getFolderByPath(p, g_falos_core.m_current_folder);
        g_falos_core.fs->removeFromMeta(f);
        g_falos_core.fs->deleteFolder(f);

        return 0;
    }
    inline fs::File* getFileByName(fs::Folder* folder,std::string name) {
        for (fs::File* f : folder->files) {
            if (f->name == name)
                return f;
        }
        return nullptr;
    }
    int info(std::string p) {
        if (p.empty() || p == " ") {
            std::cout << "FalOS v0.2 \n";
            return 0;
        }
        if (auto f = g_falos_core.fs->getFolderByPath(p)) {
            std::cout << "Name: " << f->name;
            std::cout << "\n id: " << f->id;
            std::cout << "\n root only: " << f->root_only;
            std::cout << "\n fsptr: " << f->fsptr;
            return 0;
        }
        if (auto f = getFileByName(g_falos_core.m_current_folder, p)) {
            std::cout << "Name: " << f->name;
            std::cout << "\n size (kib): " << f->filesize / 1024;
            std::cout << "\n executable: " << f->is_exec;
            std::cout << "\n root only: " << f->root_only;
            std::cout << "\n fsptr: " << f->fsptr;
            return 0;
        }
        std::cout << "error: params not valid ("<<p<<")";
        return 1;
    }
    int touch(std::string p) {
        fs::File* f = g_falos_core.fs->createFile(g_falos_core.m_current_folder, p);
        g_falos_core.fs->saveMetaFile(f);
        return 0;
    }
    int save(std::string p) {
        g_falos_core.fs->saveAllMeta();
        return 0;
    }
    int cat(std::string p) {
        fs::File* file;
        for(auto f : g_falos_core.m_current_folder->files) {
            if (f->name == p) {
                file = f;
            }
        }
        std::cout << g_falos_core.fs->GetFileData(file);
        return 0;
    }
    int write(std::string p) {
        fs::File* file;
        for(auto f : g_falos_core.m_current_folder->files) {
            if (f->name == p.substr(0,p.find(" "))) {
                file = f;
            }
        }
        vector<uint8_t> buf;
        std::string text = p.substr(p.find(" "), p.length()-1);
        buf.insert(buf.end(), text.data(),text.data()+ text.length());
        g_falos_core.fs->saveDataFile(file, buf);
        return 0;
    }


#pragma endregion

Terminal::Terminal() {
    commands["write"] = write;
    commands["cat"] = cat;
    commands["shut"] = shut;
    commands["echo"] = echo;
    commands["gr"] = gr;
    commands["wherei"] = wherei;
    commands["ls"] = ls;
    commands["mkdir"] = mkdir;
    commands["cd"] = cd;
    commands["rm"] = rm;
    commands["touch"] = touch;
    commands["info"] = info;
}

Terminal::~Terminal() {
    commands.clear();
    command.clear();
}

void Terminal::searchcommand(std::string* command) {
    int i = command->find(" ");
    std::string c = ((i == std::variant_npos) ? *command : command->substr(0, i));
    if (commands.count( c.c_str()) != 0) {
            int code = commands[c]((i == std::variant_npos) ? " " : command->substr(i+1));
            if (code != 0)
                sysinfo(("return code : "+ std::to_string(code)).data());
    }
    else
    std::cout << "invalid command \n";
}
