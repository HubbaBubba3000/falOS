#include "FS.h"
#include "file.h"
#include "folder.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
//#include <ostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <utility>
#include <vector>
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
        else {
            loadMetaFromFile();
        }
    }
    FS::~FS() {}

    void FS::CreateFS() {
        root = createFolder(nullptr,"root");
        createFolder(root, "bin");
        createFolder(root, "usr");
        createFolder(root, "cfg");
        createFolder(root->folders.back(), "test");// to cfg
    }

    Folder_t folderToFolder_t(Folder* folder) {
        Folder_t f {
            (uint16_t)(folder->id),
            folder->parent ? (uint16_t)(folder->parent->id) : (uint16_t)-1,
            folder->root_only,
            (uint8_t)(folder->name.length()),
            folder->name.data()
        };
        return f;
    }
    Folder* Folder_tToFolder(Folder_t* folder) {
        Folder* f = new Folder(folder->name);
        f->id = folder->id;
        f->root_only = folder->rootonly;
        //f.parent = getFolderByID(folder->parentid);
        return f;
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
        return f;
    }

    Folder* FS::GetFolderById(int id, Folder* p) {
        if (root->id == id) return root;
        for (Folder* t : root->folders) {
            if (t->id == id) return t;
            return GetFolderById(id,p);
        }
        return nullptr;
    }

#pragma region file


    void FS::loadMetaFromFile() {
        std::ifstream ms(meta);
        if (!ms.is_open()) {
            std::cout << "failed to open meta file";
            return;
        }
        uint8_t type;
        vector<Folder_t> folders;
        while (!ms.eof()) {
        ms.read(reinterpret_cast<char*>(&type), sizeof(type));
        switch (type) {
            case 2:
                folders.push_back(deserializeFolder(ms));
                break;
            case 1:
            std::cout << "is file \n";
                //deserializeFile(vector<uint8_t> data);
                break;
        }
        }
        // for (auto f : folders) {
        //     std::cout << f.id << " " << f.parentid<< " "  << f.rootonly << " " << std::string(f.name) << "\n";
        // }
        // TODO optimize
        root = Folder_tToFolder(&folders.front());
        for(Folder_t p : folders)
            for(Folder_t f : folders) {
                if(p.id == f.id) return;
                if (p.id == f.parentid) {
                    Folder* f = GetFolderById(p.id, root);
                    f->folders.push_back(std::move(f));
                }

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

    void FS::serialize(Folder* f, std::ofstream& out) {
        if (!f) return;
        auto folderData = serializeFolder(f);
        for (auto d : folderData) {
            std::cout << stoi(std::to_string(d)) << " ";
        }
        std::cout << std::endl;
        out.write(reinterpret_cast<const char*>(folderData.data()), folderData.size());
        if (!f->folders.empty())
        for (auto f : f->folders) {
            serialize(f, out);
        }
    }

    void FS::saveAllMeta() {
        std::ofstream out(meta, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open file");

        serialize(root,out);

    }


    //it cant be implement yet
    // void FS::saveMetaFolder(Folder* f) {

    // }
    // void FS::saveMetaFile(File* f) {}
    void FS::saveDataFile() {}
#pragma endregion

#pragma region serializing


    inline Folder* getChild(std::string name, Folder* p) {
        for(auto f : p->folders)
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
            if (!f) {
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

    vector<uint8_t> FS::serializeFolder(Folder *folder) {
        Folder_t f = folderToFolder_t(folder);
        vector<uint8_t> buffer;
        uint8_t i = 2;
        buffer.push_back(i);
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&f.id);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.id));

           // Добавляем parentid
           ptr = reinterpret_cast<const uint8_t*>(&f.parentid);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.parentid));

           // Добавляем rootonly
           ptr = reinterpret_cast<const uint8_t*>(&f.rootonly);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.rootonly));

           // Добавляем namesize
           ptr = reinterpret_cast<const uint8_t*>(&f.namesize);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.namesize));
           std::cout << *ptr;
           // Добавляем имя
           buffer.insert(buffer.end(), f.name, f.name + f.namesize);
        return buffer;
    }
    Folder_t FS::deserializeFolder(std::ifstream& in) {
        Folder_t t ;
        in.read(reinterpret_cast<char*>(&t.id), sizeof(t.id));
        in.read(reinterpret_cast<char*>(&t.parentid), sizeof(t.parentid));
        in.read(reinterpret_cast<char*>(&t.rootonly), sizeof(t.rootonly));
        in.read(reinterpret_cast<char*>(&t.namesize), sizeof(t.namesize));
        // std::string n;
        // n.resize(t.namesize);
        // in.read(&n[0], t.namesize);
        // t.name = n.data();
        t.name = new char[t.namesize + 1];
           in.read(t.name, t.namesize);
           t.name[t.namesize] = '\0';  // Добавляем нуль-терминатор
       // return Folder_tToFolder(&t);
        return t;
    }

    vector<uint8_t> FS::serializeFile(File &file) {
        File_t f = FileToFile_t(&file);
        vector<uint8_t> buffer; // file type
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
    Folder* FS::createFolder(Folder* parent, std::string name) {
        Folder* f = new Folder(name);
        f->id = ++m_id;
        f->root_only = false;
        f->parent = parent;
        if (parent)
            parent->folders.push_back(f);
        return f;
    }
    Folder* FS::editFolderName(Folder* folder, std::string* new_name) {
        folder->name.replace(0, folder->name.length(), *new_name);
        return folder;
    }
    void FS::deleteFolder(Folder* folder) {
        folder->parent->folders.remove(folder);
        delete folder;
    }

    File* FS::createFile(Folder* parent, std::string* name) {
        File* f = new File(name);
        f->parent = parent;
        f->root_only = false;
        f->is_exec = false;
        parent->files.push_back(f);

        return f;
    }
    File* FS::renameFile(File* file, std::string* new_name) {
        file->name->replace(0, file->name->length(), *new_name);
        return file;
    }
    void FS::deleteFile(File* file) {
        file->parent->files.remove(file);
        delete file;
    }
#pragma endregion

}
