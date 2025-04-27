#pragma once
#include "folder.h"
#include <cstdint>
#include <string>
namespace FS {
    class Folder;

    struct File_t {
        int16_t parentid;
        std::string name;
        int8_t namesize;
        int32_t memptr;
        bool is_exec;
        bool root_only;
        int16_t filesize;
    };

    class File {
        public:
        File(std::string* name) : name(name) {}
        ~File() {
            delete name;
            //TODO delete data file from storage
        }
        Folder* parent;
        std::string* name;
        bool is_exec = false;
        bool root_only = false;
        int memptr;
        std::string* getpath();

    };
}
