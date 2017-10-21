#include "filesystem.h"

FileSystem::FileSystem() {
    this->root = Folder("/");
    this->currDir = &this->root;
}

FileSystem::~FileSystem() {

}


/* Please insert your code */

bool FileSystem::createFolder(std::string name) {
    bool output = false;
    for (Folder* f : this->currDir->folders)
        std::cout << "1. " << f->name << std::endl;
    Folder* tempFolder = new Folder(name, this->currDir);
    auto it = std::find_if(this->currDir->folders.begin(), this->currDir->folders.end(),
        [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
    if (it != this->currDir->folders.end()) // Folder found
        delete tempFolder;
    else {
        this->currDir->folders.push_back(tempFolder);
        output = true;
    }
    return output;
}

bool FileSystem::removeFolder(std::string name) {
    bool output = false;
    Folder* tempFolder = new Folder(name);
    auto it = std::find_if(this->currDir->folders.begin(), this->currDir->folders.end(),
        [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
    if (it != this->currDir->folders.end()) { // Folder found
        delete *it;
        this->currDir->folders.erase(it);
        output = true;
    }
    delete tempFolder;
    return output;
}

std::string FileSystem::goToFolder(std::string name, std::string output) {
    Folder* folder = this->currDir;
    std::string save = output;
    while (name != "") {
        size_t pos = name.find_first_of('/');
        if (pos != std::string::npos) {
            std::string part = name.substr(0, pos);
            name = name.substr(pos + 1);
            size_t pos = name.find_first_of('/');
            if (pos != std::string::npos && name[0] == '/')
                name = "";
            if (part == "..") {
                size_t length = output.length() - folder->name.length() - 1;
                output = output.substr(0, length);
                folder = folder->parent;
            } else {
                Folder* tempFolder = new Folder(part);
                auto it = std::find_if(this->currDir->folders.begin(), this->currDir->folders.end(),
                    [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
                if (it != this->currDir->folders.end()) { // Folder found
                    output += "/";
                    output += (*it)->name;
                    folder = *it;
                } else {
                    name = "";
                    folder = this->currDir;
                    output = save;
                }
               delete tempFolder;
            }
        } else {
            Folder* tempFolder = new Folder(name);
            auto it = std::find_if(this->currDir->folders.begin(), this->currDir->folders.end(),
                [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
            if (it != this->currDir->folders.end()) { // Folder found
                output += "/";
                output += (*it)->name;
                folder = *it;
            } else {
                name = "";
                folder = this->currDir;
                output = save;
            }
            delete tempFolder;
        }
    }
    this->currDir = folder;
    return output;
}