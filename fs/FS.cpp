#include "FS.h"
#include "file.h"
#include "folder.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
//#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
using std::pmr::vector;

namespace fs {
    inline bool fileIsExist (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
    inline void sysinfo(std::string s) {
        std::cout << "sys: " << s << "\n";
    }
    FS::FS() {
        meta = "meta.bin";
        data = "data.bin";
        if (!fileIsExist(meta)) {
            sysinfo("meta doesnt exists , start creating file system");
            CreateFS();
        }
    }
    FS::~FS() {}

    void FS::CreateFS() {
        root = createFolder(nullptr,new std::string("root"));
        createFolder(root, new std::string("bin"));
        createFolder(root, new std::string("usr"));
        createFolder(root, new std::string("cfg"));
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
            (int8_t)folder->name.length(),
            folder->name.data()
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

    File_t FileToFile_t(File* f) {
        File_t t {
            f->parent ? (uint16_t)(f->parent->id) : (uint16_t)-1,
            f->is_exec,
            f->root_only,
            (uint32_t)f->memptr,
            (uint16_t)f->filesize,
            (uint8_t)f->name->size(),
            f->name->data()
        };
        return t;
    }

    File File_tToFile(File_t* t) {
        File f = *new File(t->name);
        f.is_exec = t->is_exec;
        f.root_only = t->root_only;
        f.memptr = t->memptr;
        f.filesize = t->filesize;
        //f.parent TODO
        return f;
    }
    inline Folder* getChild(std::string name, Folder* p) {
        for(auto f : *p->folders)
            if (f->name == name)
                return f;
        return nullptr;
    }
    Folder* FS::getFolderByPath(std::string path, Folder* o) {
        std::string sub;
        Folder* f = o ? o : root;
        for(size_t p=0, q=0; p!=path.npos; p=q) {
            sub = path.substr(p+(p!=0), (q=path.find("/", p+1))-p-(p!=0));
            if (f != root) {
                if (sub == "..") {
                    f = f->parent;
                    continue;
                }
            }

            f = getChild(sub, f);
            if (!f) {  // Исправлено условие
                std::cout << "folder not exist";
                return nullptr;
            }
        }
        return f;
    }
    std::string FS::getPathByFolder(Folder* f) {
        std::string path;
        while (f) {
        path.insert(0, f->name + "/");
        f = f->parent;
        }
        return path;
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
    Folder FS::deserializeFolder(vector<uint8_t> data) {
        Folder_t t ;
        size_t offset = 0;

        if (offset + sizeof(t.id) > data.size()) {
            throw std::runtime_error("Invalid buffer: id overflow");
        }
        memcpy(&t.id, data.data() + offset,sizeof(t.id));
        offset += sizeof(t.id);
        if (offset + sizeof(t.parentid) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.parentid, data.data() + offset,sizeof(t.parentid));
        offset += sizeof(t.parentid);
        if (offset + sizeof(t.rootonly) > data.size()) {
            throw std::runtime_error("Invalid buffer: rootbool overflow");
        }
        memcpy(&t.rootonly, data.data() + offset,sizeof(t.rootonly));
        offset += sizeof(t.rootonly);
        if (offset + sizeof(t.namesize) > data.size()) {
            throw std::runtime_error("Invalid buffer: namesize overflow");
        }
        memcpy(&t.namesize, data.data() + offset,sizeof(t.namesize));
        offset += sizeof(t.namesize);
        if (offset + t.namesize > data.size()) {
            throw std::runtime_error("Invalid buffer: name overflow");
        }
        t.name = new char[t.namesize + 1];
        memcpy(t.name, data.data() + offset, t.namesize);
        t.name[t.namesize] = '\0';
        return Folder_tToFolder(&t);
    }

    vector<uint8_t> FS::serializeFile(File &file) {
        File_t f = FileToFile_t(&file);
        vector<uint8_t> buffer;
        uint8_t* ptr;
        ptr = reinterpret_cast<uint8_t*>(&f.parentid);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.parentid));
        ptr = reinterpret_cast<uint8_t*>(&f.is_exec);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.is_exec));
        ptr = reinterpret_cast<uint8_t*>(&f.root_only);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.root_only));
        ptr = reinterpret_cast<uint8_t*>(&f.memptr);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.memptr));
        ptr = reinterpret_cast<uint8_t*>(&f.filesize);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.filesize));
        ptr = reinterpret_cast<uint8_t*>(&f.namesize);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(f.namesize));
        ptr = reinterpret_cast<uint8_t*>(f.name);
        buffer.insert(buffer.end(), ptr, ptr + f.namesize);
        return buffer;
    }

    File_t FS::deserializeFile(vector<uint8_t> data) {
        File_t t ;
        size_t offset = 0;

        if (offset + sizeof(t.parentid) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.parentid, data.data() + offset,sizeof(t.parentid));
        offset += sizeof(t.parentid);
        if (offset + sizeof(t.is_exec) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.is_exec, data.data() + offset,sizeof(t.is_exec));
        offset += sizeof(t.is_exec);
        if (offset + sizeof(t.root_only) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.root_only, data.data() + offset,sizeof(t.root_only));
        offset += sizeof(t.root_only);
        if (offset + sizeof(t.memptr) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.memptr, data.data() + offset,sizeof(t.memptr));
        offset += sizeof(t.memptr);
        if (offset + sizeof(t.filesize) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.filesize, data.data() + offset,sizeof(t.filesize));
        offset += sizeof(t.filesize);
        if (offset + sizeof(t.namesize) > data.size()) {
            throw std::runtime_error("Invalid buffer: pid overflow");
        }
        memcpy(&t.namesize, data.data() + offset,sizeof(t.namesize));
        offset += sizeof(t.namesize);
        t.name = new char[t.namesize + 1];
        memcpy(t.name, data.data() + offset, t.namesize);
        t.name[t.namesize] = '\0';
        return t;
    }
#pragma endregion

#pragma region crud
    Folder* FS::createFolder(Folder* parent, std::string* name) {
        Folder* f = new Folder(*name);
        f->id = ++m_id;
        f->root_only = false;
        f->parent = parent;
        if (parent)
            parent->folders->push_back(f);
        return f;
    }
    Folder* FS::editFolderName(Folder* folder, std::string* new_name) {
        folder->name.replace(0, folder->name.length(), *new_name);
        return folder;
    }
    void FS::deleteFolder(Folder* folder) {
        folder->parent->folders->remove(folder);
        delete folder;
    }

    File* FS::createFile(Folder* parent, std::string* name) {
        File* f = new File(name);
        f->parent = parent;
        f->root_only = false;
        f->is_exec = false;
        parent->files->push_back(f);

        return f;
    }
    File* FS::renameFile(File* file, std::string* new_name) {
        file->name->replace(0, file->name->length(), *new_name);
        return file;
    }
    void FS::deleteFile(File* file) {
        file->parent->files->remove(file);
        delete file;
    }
#pragma endregion

}
