#include "FS.h"
#include "file.h"
#include "folder.h"
// #include <cmath>
#include <algorithm>
#include <cstddef>
#include <cstdint>
// #include <cstring>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
using std::pmr::vector;

namespace FS {

    FS::FS() {
        meta = "meta.bin";
        data = "data.bin";
    }
    FS::~FS() {

    }
//not implemented
#pragma region file

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

    void FS::saveAllMeta() {

    }
    //it cant be implement yet
    // void FS::saveMetaFolder(Folder* f) {

    // }
    // void FS::saveMetaFile(File* f) {}
    void FS::saveDataFile() {}
#pragma endregion
#pragma region serializing

Folder_t folderToFolder_t(Folder* folder) {
    Folder_t f {
        (int16_t)(folder->id),
        folder->parent ? (int16_t)(folder->parent->id) : (int16_t)-1,
        folder->root_only,
        (int8_t)folder->name->length(),
        folder->name->data()
    };
    return f;
}
Folder Folder_tToFolder(Folder_t* folder) {
    Folder* f = new Folder(folder->name);
    f->id = folder->id;
    f->root_only = folder->rootonly;
    //f.parent = getFolderByID(folder->parentid);
    return *f;
}

vector<uint8_t> FS::serializeFolder(Folder &folder) {
    Folder_t f = folderToFolder_t(&folder);
    vector<uint8_t> buffer;
    uint8_t* ptr;
    ptr = reinterpret_cast<uint8_t*>(&f.id);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(f.id));
    ptr = reinterpret_cast<uint8_t*>(&f.parentid);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(f.parentid));
    ptr = reinterpret_cast<uint8_t*>(&f.rootonly);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(f.rootonly));
    ptr = reinterpret_cast<uint8_t*>(&f.namesize);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(f.namesize));
    ptr = reinterpret_cast<uint8_t*>(f.name);
    buffer.insert(buffer.end(), ptr, ptr + f.namesize);
    return buffer;
}
Folder_t FS::deserializeFolder(vector<uint8_t> data) {
    Folder_t t ;
    size_t offset = 0;

    if (offset + sizeof(t.id) > data.size()) {
        throw std::runtime_error("Invalid buffer: id overflow");
    }
    memcpy(&t.id, data.data() + offset,sizeof(t.id));
    std::cout << t.id;
    offset += sizeof(t.id);
    if (offset + sizeof(t.parentid) > data.size()) {
        throw std::runtime_error("Invalid buffer: pid overflow");
    }
    memcpy(&t.parentid, data.data() + offset,sizeof(t.parentid));
    std::cout << t.parentid;
    offset += sizeof(t.parentid);
    if (offset + sizeof(t.rootonly) > data.size()) {
        throw std::runtime_error("Invalid buffer: rootbool overflow");
    }
    memcpy(&t.rootonly, data.data() + offset,sizeof(t.rootonly));
    std::cout << t.rootonly;
    offset += sizeof(t.rootonly);
    if (offset + sizeof(t.namesize) > data.size()) {
        throw std::runtime_error("Invalid buffer: namesize overflow");
    }
    memcpy(&t.namesize, data.data() + offset,sizeof(t.namesize));
    std::cout << t.namesize;
    offset += sizeof(t.namesize);
    if (offset + t.namesize > data.size()) {
        throw std::runtime_error("Invalid buffer: name overflow");
    }
    t.name = new char[t.namesize + 1]; // +1 для нуль-терминатора
       memcpy(t.name, data.data() + offset, t.namesize);
       t.name[t.namesize] = '\0'; // Добавляем нуль-терминатор
       offset += t.namesize;
   std::cout << t.name;
    return t;
}

//пускай диса сам пишет мне лень
size_t  FS::serializeFile(File* file) {
    // конвертировать file в file_t
    // вычислить размер типа
    // инициализировать буффер и указатель на память
    // поочередно копировать поля в буффер
    // вернуть размер буфера
    //
    return 0;
}

File_t FS::deserializeFile(char* data) {
    File_t t;
    return t;
}

#pragma endregion

#pragma region crud
    Folder* FS::createFolder(Folder* parent, std::string* name) {
        Folder* f = new Folder(name);
        f->id = 1;
        f->root_only = false;
        f->parent = parent;
        return f;
    }
    Folder* FS::editFolderName(Folder* folder, std::string* new_name) {
        folder->name->replace(0, folder->name->length(), *new_name);
        return folder;
    }
    void FS::deleteFolder(Folder* folder) {
        delete folder;
    }

    File* FS::createFile(Folder* parent, std::string* name) {
        File* f = new File(name);
        f->parent = parent;
        f->root_only = false;
        f->is_exec = false;
        return f;
    }
    File* FS::renameFile(File* file, std::string* new_name) {
        file->name->replace(0, file->name->length(), *new_name);
        return file;
    }
    void FS::deleteFile(File* file) {
        delete file;
    }
#pragma endregion

}
