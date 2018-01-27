#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <algorithm>
#include <typeinfo>
#include <queue>
#include <iostream>
#include <fstream>

class FileSystem {
private:
    MemBlockDevice mMemblockDevice;
    // Here you can add your own data structures
    class File {
    public:
		std::string name;
		std::vector<int> blockIds;
		//Folder* parent;
		File(std::string fileName, std::vector<int> blocks = {}) {
			this->name = fileName;
			this->blockIds = blocks;
		}
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
        ~Folder() {
            for (Folder* f : folders)
                delete f;
            for (File* f : files)
                delete f;
        }
    };

    friend bool operator==(const Folder& lhs, const Folder& rhs) { return lhs.name == rhs.name; }
	friend bool operator==(const File& lhs, const File& rhs) { return lhs.name == rhs.name; }

    Folder root;
    Folder* currDir;

public:
    FileSystem();
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	int createFile(std::string fileName, std::string content, std::string current);

    /* Creates a folder in the filesystem */
    bool createFolder(std::string name);

    /* Removes a file in the filesystem */
    bool removeFile(std::string fileName);

    /* Removes a folder in the filesystem */
    bool removeFolder(std::string name);

    /* Function will move the current location to a specified location in the filesystem */
    std::string goToFolder(std::string name, std::string output);

    /* This function will get all the files and folders in the specified folder */
    std::string listDir(std::string name, std::string current);

	/* Add your own member-functions if needed */

	/* Gets the content of a file */
	std::string getFileContent(std::string fileName, std::string current);

	/* Creates and image of the filesystem */
	void createImage(std::string name);

	/* Recursively fills a file with the filesystem */
	void recCreate(Folder* dir, std::ofstream& file);

	/* Loads a filesystem from a file */
	void loadImage(std::string path);

	/* Recursively loads a filesystem from a file */
	void recLoad(Folder* dir, std::ifstream& file);
};

#endif // FILESYSTEM_H
