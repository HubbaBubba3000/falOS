#pragma once
#include "folder.h"
#include <string>
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



    private:
    // TODO implement serialization
        void serializeFolder(Folder* folder);
        void serializeFile(File* file);
        void serializeFS();

        Folder* deserializeFolder();
        File* deserializeFile();
        void deserializeFS();
    };

}
