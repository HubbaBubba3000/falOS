#pragma once
#include "folder.h"
#include <cstddef>
#include <string>
#include <vector>
using std::pmr::vector;
namespace FS {

    class FS {
    public:
        FS();
        ~FS();
        //storage files
        std::string meta;
        std::string data;

        void loadMetaFromFile();
        unsigned char* loadDataFromfile(File* file);


        void saveAllMeta();
        void saveMetaFolder(Folder* f);
        void saveMetaFile(File* f);
        void saveDataFile();

        //void trimstorage();

        Folder* root;

        Folder* createFolder(Folder* parent, std::string* name);
        Folder* editFolderName(Folder* folder, std::string* new_name);
        void deleteFolder(Folder* folder);

        File* createFile(Folder* parent, std::string* name);
        File* renameFile(File* file, std::string* new_name);
        void deleteFile(File* file);

        vector<uint8_t> serializeFolder(Folder &folder);
        Folder_t deserializeFolder(vector<uint8_t> data);

    private:


    // TODO implement serialization
        size_t  serializeFile(File* file);

        File_t deserializeFile(char* data);
    };

}
