#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <algorithm>

class FileSystem {
private:
    MemBlockDevice mMemblockDevice;
    // Here you can add your own data structures
    class File {
    public:
        File() {}
        ~File() {}

    };

    class Folder {
    public:
        std::string name;
        Folder* parent;
        std::vector<Folder*> folders;
        std::vector<File*> files;
    
        Folder() {}

        Folder(std::string name, Folder* parent = nullptr) {
            this->name = name;
            this->parent = parent;
        }
        ~Folder() {}
    };

    friend bool operator==(const Folder& lhs, const Folder& rhs) { return lhs.name == rhs.name; }

    Folder root;
    Folder* currDir;

public:
    FileSystem();
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
    // createFile(...)

    /* Creates a folder in the filesystem */
    bool createFolder(std::string name);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    bool removeFolder(std::string name);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
