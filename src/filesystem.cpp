#include "filesystem.h"

FileSystem::FileSystem() {
    this->root = Folder("/");
    this->currDir = &this->root;
}

FileSystem::~FileSystem() {

}


/* Please insert your code */

bool FileSystem::createFolder(std::string name) {
    Folder* tempFolder = new Folder(name, this->currDir);
    auto it = std::find_if(this->currDir->folders.begin(), this->currDir->folders.end(),
        [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
    if (it != this->currDir->folders.end()) // Folder found
        return false;
    else   
        this->currDir->folders.push_back(tempFolder);
    return true;
}

bool FileSystem::removeFolder(std::string name) {
    
}