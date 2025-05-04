#pragma once
#include "file.h"
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
namespace fs {
    class File;

    struct Folder_t {
        int16_t id;
        int16_t parentid;
        bool rootonly;
        int8_t namesize;
        char* name;
    };

    class Folder {
    public:
    Folder(std::string* name) : name(name) {
        folders = new std::list<Folder*>();
        files = new std::list<File*>();
    }
    Folder(char* str) {
        name = new std::string(str);
        folders = new std::list<Folder*>();
        files = new std::list<File*>();
    }
      ~Folder() {
          folders->erase(folders->cbegin());
          files->erase(files->cbegin());
          delete name;
      }
      int id;
      Folder* parent;
     std::list<Folder*>* folders;
     std::list<File*>* files;

      std::string* name;
      bool root_only;

      std::string getpath();
    };
}
