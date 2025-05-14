#pragma once
#include "file.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <string>
namespace fs {
    class File;

    struct Folder_t {
        uint16_t id;
        uint16_t parentid;
        bool rootonly;
        uint8_t namesize;
        char* name;
    };

    class Folder {
    public:
    Folder(std::string name) : name(name) {
        folders = *new std::list<Folder*>();
        files = *new std::list<File*>();
    }
    Folder(const char* str) : name(str) {
        folders = *new std::list<Folder*>();
        files = *new std::list<File*>();
    }
   // Folder(Folder_t t) : id(t.id) :
      ~Folder() {
          // folders.erase(folders.cbegin());
          // files.erase(files.cbegin());
      }
      int id;
      Folder* parent;
     std::list<Folder*> folders;
     std::list<File*> files;

      std::string name;
      bool root_only;

      std::string getpath();
    };
}
