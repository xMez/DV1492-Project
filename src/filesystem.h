#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"

class FileSystem {
private:
    // Here you can add your own data structures
    class File {
    public:

    };

    class Folder {
    public:
        Folder* parent;
        std::string name;

        Folder(std::string name, Folder* parent) {
            this->name = name;
            this->parent = parent;
        }

        ~Folder() {}


    };

    MemBlockDevice mMemblockDevice;
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
    void createFolder(std::string name);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
