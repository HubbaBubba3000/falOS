#include "FS.h"
#include "file.h"
#include "folder.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <variant>
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
            saveAllMeta();
        }
        else {
            loadMetaFromFile();
        }
        if(!fileIsExist(data)){
            sysinfo("data doesnt exist, creating data file");

            std::ofstream df(data);
        }
    }
    FS::~FS() {}

    void FS::CreateFS() {
        root = createFolder(nullptr,"root");
        createFolder(root, "bin");
        createFolder(root, "usr");
        createFolder(root, "cfg");
        createFolder(root->folders.back(), "test");// to cfg
        createFile(root->folders.front(), "fil.e"); // bin
    }

    Folder_t folderToFolder_t(Folder* folder) {
        Folder_t f {
            (uint16_t)(folder->id),
            folder->parent ? (uint16_t)(folder->parent->id) : (uint16_t)-1,
            folder->root_only,
            (uint8_t)(folder->name.length()),
            folder->name.data(),
            folder->fsptr
        };
        return f;
    }
    Folder* Folder_tToFolder(Folder_t* folder) {
        Folder* f = new Folder(folder->name);
        f->id = folder->id;
        f->root_only = folder->rootonly;
        f->fsptr = folder->fsptr;
        return f;
    }

    File_t FileToFile_t(File* f) {
        File_t t {
            f->parent ? (uint16_t)(f->parent->id) : (uint16_t)-1,
            f->is_exec,
            f->root_only,
            (uint32_t)f->memptr,
            (uint16_t)f->filesize,
            (uint8_t)f->name.size(),
            f->name.data(),
            f->fsptr
        };
        return t;
    }

    File* File_tToFile(File_t* t) {
        File* f = new File(t->name);
        f->is_exec = t->is_exec;
        f->root_only = t->root_only;
        f->memptr = t->memptr;
        f->filesize = t->filesize;
        f->fsptr = t->fsptr;
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
        vector<std::variant<Folder_t, File_t>> data;
        while (!ms.eof()) {
            ms.read(reinterpret_cast<char*>(&type), sizeof(type));
            switch (type) {
                case 1:
                    data.push_back(deserializeFile(ms));
                    break;
                case 2:
                    data.push_back(deserializeFolder(ms));
                    break;
                    default: continue;
            }
        }
        std::unordered_map<int, Folder*> idToFolder;
        for (const auto& item : data) {
            if (std::holds_alternative<Folder_t>(item)) {
                Folder_t f = std::get<Folder_t>(item);
                Folder* folder = Folder_tToFolder(&f);
                idToFolder[f.id] = folder;

                if (f.parentid == (uint16_t)-1) {
                    root = folder;
                }
            }
        }
        for (const auto& item : data) {
            if (std::holds_alternative<Folder_t>(item)) {
                const auto& folder_t = std::get<Folder_t>(item);
                if (folder_t.parentid != (uint16_t)-1) { // Не корневая папка
                    if (auto parentIt = idToFolder.find(folder_t.parentid);
                        parentIt != idToFolder.end()) {
                        parentIt->second->folders.push_back(idToFolder[folder_t.id]);
                    }
                }
            }
            else { // File_t
                auto file_t = std::get<File_t>(item);
                if (auto parentIt = idToFolder.find(file_t.parentid);
                    parentIt != idToFolder.end()) {
                    parentIt->second->files.push_back(File_tToFile(&file_t));
                }
            }
        }
        ms.close();
    }

    void FS::serialize(Folder* f, std::ofstream& out) {
        if (!f) return;
        auto folderData = serializeFolder(f);
        for (auto d : folderData) {
            std::cout << stoi(std::to_string(d)) << " ";
        }
        std::cout << "\n";
        out.write(reinterpret_cast<const char*>(folderData.data()), folderData.size());
        if (!f->folders.empty())
            for (auto f : f->folders) {
                serialize(f, out);
            }
        if (!f->files.empty())
            for (auto f : f->files) {
                auto s = serializeFile(f);
                out.write(reinterpret_cast<const char*>(s.data()), s.size());
            }
    }
    void FS::saveAllMeta() {
        std::ofstream out(meta, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open file");
        serialize(root,out);
    }


    //it cant be implement yet
    void FS::saveMetaFolder(Folder* f) {
        std::ofstream out(meta, std::ios::binary | std::ios::app);
        if (!out) throw std::runtime_error("Cannot open file");
        auto data = serializeFolder(f);
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        out.close();
    }
     void FS::saveMetaFile(File* f) {
         std::ofstream out(meta, std::ios::binary | std::ios::app);
         if (!out) throw std::runtime_error("Cannot open file");
         auto data = serializeFile(f);
         out.write(reinterpret_cast<const char*>(data.data()), data.size());
         out.close();
     }
    void FS::saveDataFile(File* f, vector<uint8_t> d) {
        std::fstream out;
        if (!f) {  // Проверка на нулевой указатель
            throw std::invalid_argument("File pointer is null");
        }
        if (d.empty()) {  // Проверка на пустые данные
            return;  // или можно бросить исключение
        }
        if (f->memptr == 0) {
            out.open(data,  std::ios::binary | std::ios::app);
            out.seekp(0,std::ios::end);
            f->memptr = out.tellp();
            f->filesize = d.size();
           // std::cout << f->memptr << " /n";
        }
        else {
            out.open(data, std::ios::binary | std::ios::out);
            out.seekp(f->memptr);
        }
        out.write(reinterpret_cast<char*>(d.data()), d.size());
        out.close();
    }

    std::string FS::GetFileData(File* f) {
        std::ifstream in(data, std::ios::binary);

        in.seekg(f->memptr);
        char* buf = new char[f->filesize + 1];
        in.read(buf, f->filesize);
        buf[f->filesize] = '\0';

        in.close();
        return std::string(buf);
    }
#pragma endregion

#pragma region serializing

    // void FS::removeObjectFromData(File* f) {
    //     std::ofstream out(data, std::ios::binary | std::ios::out);
    //     out.seekp(f->fsptr);
    //     out.write(0,f->filesize);
    // }
    void FS::removeFromMeta(Folder* folder) {
        std::ofstream out(meta, std::ios::binary | std::ios::out);
        size_t s = 2+2+1+1+2+folder->name.length(); //folder_t size
        out.seekp(0, std::ios::end);
        size_t filesize = out.tellp();
        out.seekp(folder->fsptr);
        if (s + folder->fsptr > filesize) {
            std::runtime_error("size too high");
        }
        //todo
        out.close();
    }
    void FS::removeFromMeta(File* file) {
        std::ofstream out(meta, std::ios::binary | std::ios::out);
        out.seekp(file->fsptr);
        size_t s = file->name.length()+2+1+1+4+2+1+2; //folder_t size
        out.write(0,s);
        out.close();
    }
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
           ptr = reinterpret_cast<const uint8_t*>(&f.parentid);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.parentid));
           ptr = reinterpret_cast<const uint8_t*>(&f.rootonly);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.rootonly));
           ptr = reinterpret_cast<const uint8_t*>(&f.namesize);
           buffer.insert(buffer.end(), ptr, ptr + sizeof(f.namesize));
           buffer.insert(buffer.end(), f.name, f.name + f.namesize);
        return buffer;
    }
    Folder_t FS::deserializeFolder(std::ifstream& in) {
        Folder_t t ;
        t.fsptr = in.tellg();
        in.read(reinterpret_cast<char*>(&t.id), sizeof(t.id));
        in.read(reinterpret_cast<char*>(&t.parentid), sizeof(t.parentid));
        in.read(reinterpret_cast<char*>(&t.rootonly), sizeof(t.rootonly));
        in.read(reinterpret_cast<char*>(&t.namesize), sizeof(t.namesize));
        t.name = new char[t.namesize + 1];
        in.read(t.name, t.namesize);
        t.name[t.namesize] = '\0';
        return t;
    }

    vector<uint8_t> FS::serializeFile(File *file) {
        File_t f = FileToFile_t(file);
        vector<uint8_t> buffer; // file type
        uint8_t* ptr;
        buffer.push_back(1);
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
        buffer.insert(buffer.end(), f.name, f.name + f.namesize);

        // for (auto b : buffer)
        // std::cout << std::to_string(b) << " ";
        return buffer;
    }

    File_t FS::deserializeFile(std::ifstream& in) {
        File_t t ;
        t.fsptr = in.tellg();
        in.read(reinterpret_cast<char*>(&t.parentid), sizeof(t.parentid));
        in.read(reinterpret_cast<char*>(&t.is_exec), sizeof(t.is_exec));
        in.read(reinterpret_cast<char*>(&t.root_only), sizeof(t.root_only));
        in.read(reinterpret_cast<char*>(&t.memptr), sizeof(t.memptr));
        in.read(reinterpret_cast<char*>(&t.filesize), sizeof(t.filesize));
        in.read(reinterpret_cast<char*>(&t.namesize), sizeof(t.namesize));
        t.name = new char[t.namesize + 1];
        in.read(t.name, t.namesize);
        t.name[t.namesize] = '\0';
        //std::cout << t.parentid  << " " << t.is_exec << " " << t.root_only << " " << t.memptr << " " << t.filesize << " " << t.namesize;
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
    Folder* FS::editFolderName(Folder* folder, std::string new_name) {
        folder->name.replace(0, folder->name.length(),new_name);
        return folder;
    }
    void FS::deleteFolder(Folder* folder) {
        folder->parent->folders.remove(folder);
        delete folder;
    }

    File* FS::createFile(Folder* parent, std::string name) {
        File* f = new File(name);
        f->parent = parent;
        f->root_only = false;
        f->is_exec = false;
        parent->files.push_back(f);
        return f;
    }
    File* FS::renameFile(File* file, std::string new_name) {
        file->name.replace(0, file->name.length(), new_name);
        return file;
    }
    void FS::deleteFile(File* file) {
        file->parent->files.remove(file);
        delete file;
    }
#pragma endregion

}
