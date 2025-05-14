#pragma once
#include "folder.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>
using std::pmr::vector;
namespace fs {

    class FS {
    public:
        FS();
        ~FS();
        //storage files
        std::string meta;
        std::string data;

        void loadMetaFromFile();
        unsigned char* loadDataFromfile(File* file);

        Folder* getFolderByPath(std::string path, Folder* o = nullptr);
        std::string getPathByFolder(Folder* folder);

        void saveAllMeta();
        void saveMetaFolder(Folder* f);
        void saveMetaFile(File* f);
        void saveDataFile();

        //void trimstorage();

        Folder* root;

        Folder* createFolder(Folder* parent, std::string name);
        Folder* editFolderName(Folder* folder, std::string new_name);
        void deleteFolder(Folder* folder);

        File* createFile(Folder* parent, std::string name);
        File* renameFile(File* file, std::string new_name);
        void deleteFile(File* file);

        vector<uint8_t> serializeFolder(Folder *folder);
        Folder_t deserializeFolder(std::ifstream& in);
        vector<uint8_t> serializeFile(File *file);
        File_t deserializeFile(std::ifstream& in);
    private:
        int m_id = 0;
        void CreateFS();
        Folder* GetFolderById(int id, Folder* p);
        void serialize(Folder* f, std::ofstream &out);
        Folder* deserialize(uint8_t* data, std::ifstream &in);


    };

}
