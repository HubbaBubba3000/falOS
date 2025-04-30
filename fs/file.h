#pragma once
#include "folder.h"
#include <cstddef>
#include <cstdint>
#include <string>
namespace FS {
    class Folder;

    struct File_t {
        uint16_t parentid;
        bool is_exec;
        bool root_only;
        uint32_t memptr;    // max storage 4 gib
        uint16_t filesize; //max 64kib
        uint8_t namesize;
        char* name;
    };

    class File {
        public:
        File(std::string* name) : name(name) {}
        File(char* name) : name(new std::string(name)) {}
        ~File() {
            delete name;
            //TODO delete data file from storage
        }
        Folder* parent;
        std::string* name;
        bool is_exec = false;
        bool root_only = false;
        int memptr;
        size_t filesize;
        std::string* getpath();

    };
}
