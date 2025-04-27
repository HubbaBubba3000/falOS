#include "FS.h"
#include "file.h"
#include "folder.h"
#include <fstream>
#include <iostream>

namespace FS {

    FS::FS() {
        meta = "meta.bin";
        data = "data.bin";
    }
    FS::~FS() {

    }

    void FS::loadMetaFromFile() {
        std::ifstream ms(meta);

        if (!ms.is_open()) {
            std::cout << "failed to open meta file";
            return;
        }

        ms.close();
    }
    unsigned char* FS::loadDataFromfile(File* file) {
        std::ifstream ds(data);

        if (!ds.is_open()) {
            std::cout << "failed to open meta file";
            return nullptr;
        }



        ds.close();
        return nullptr; // TODO
    }

    Folder* FS::createFolder(Folder* parent, std::string* name) {
        parent->folders.push_back(new Folder(name));
        return parent->folders.back();
    }
    Folder* FS::editFolderName(Folder* folder, std::string* new_name) {
        folder->name->replace(0, folder->name->length(), *new_name);
        return folder;
    }
    void FS::deleteFolder(Folder* folder) {
        delete folder;
    }

    File* FS::createFile(Folder* parent, std::string* name) {
        parent->files.push_back(new File(name));
        return parent->files.back();
    }
    File* FS::renameFile(File* file, std::string* new_name) {
        file->name->replace(0, file->name->length(), *new_name);
        return file;
    }
    void FS::deleteFile(File* file) {
        delete file;
    }


}
