#pragma once
#include "file.h"
#include <cstdarg>
#include <cstdint>
#include <list>
#include <string>
namespace FS {
    class File;

    struct Folder_t {
        int16_t parentid;
        std::string name;
        int8_t namesize;
        int16_t id;
        bool rootonly;
    };

    class Folder {
    public:
      Folder(std::string* name) : name(name) {}
      ~Folder() {
          folders.erase(folders.cbegin());
          files.erase(files.cbegin());
          delete name;
      }
      Folder* parent;
      std::list<Folder*> folders;
      std::list<File*> files;

      std::string* name;
      bool root_only;

      std::string getpath();
    };
}
