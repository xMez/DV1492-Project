#include "filesystem.h"

FileSystem::FileSystem() {
    this->root = Folder("/");
    this->currDir = &this->root;
}

FileSystem::~FileSystem() {

}


/* Please insert your code */

void FileSystem::createFolder(std::string name) {
    this->currDir->folders.push_back(Folder(name, this->currDir));
}

void FileSystem::removeFolder(std::string name) {
    
}