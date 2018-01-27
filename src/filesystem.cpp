#include "filesystem.h"

FileSystem::FileSystem() {
    this->root = Folder("/");
    this->currDir = &this->root;
}

FileSystem::~FileSystem() {

}

/* Please insert your code */

int FileSystem::createFile(std::string fileName, std::string content, std::string current) {
	int output = -2;
	size_t size = content.length() / 512 + 1;
	if (size <= mMemblockDevice.spaceLeft()) {
		Folder* origFolder = this->currDir;
		size_t pos = fileName.find_last_of('/');
		if (pos != std::string::npos) {
			std::string dir = fileName.substr(0, pos);
			fileName = fileName.substr(pos + 1);
			pos = fileName.find_first_of('/');
			if (pos != std::string::npos)
				output = 0;
			else {
				std::string temp = goToFolder(dir, current);
				if (temp == current)
					output = 0;
			}
		}
		if (output != 0) {
			std::vector<std::string> split(size);
			std::vector<int> blocks;
			for (size_t i = 0; i < content.length(); i += 512) {
				split.at(i / 512) = content.substr(i, 512);
				split.at(i / 512).append(512 - split.at(i / 512).length(), '\n');
				blocks.push_back(mMemblockDevice.getFreeBlock());
			}
			File* tempFile = new File(fileName, blocks);
			auto it = std::find_if(this->currDir->files.begin(), this->currDir->files.end(),
				[tempFile](const File* f) -> bool { return *f == *tempFile; });
			if (it != this->currDir->files.end()) { // File found
				delete tempFile;
				for (const auto i : blocks)
					mMemblockDevice.releaseBlock(i);
				output = -1;
			}
			else {
				for (size_t i = 0; i < blocks.size(); i++)
					mMemblockDevice.writeBlock(blocks[i], split[i]);
				this->currDir->files.push_back(tempFile);
				output = 1;
			}
		}
		this->currDir = origFolder;
	}
	return output;
}

bool FileSystem::createFolder(std::string name) {
    bool output = false;
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

bool FileSystem::removeFile(std::string fileName) {
	bool output = false;
	File* tempFile = new File(fileName);
	auto it = std::find_if(this->currDir->files.begin(), this->currDir->files.end(),
		[tempFile](const File* f) -> bool { return *f == *tempFile; });
	if (it != this->currDir->files.end()) { // Folder found
		for (const auto i : (*it)->blockIds)
			mMemblockDevice.releaseBlock(i);
		delete *it;
		this->currDir->files.erase(it);
		output = true;
	}
	delete tempFile;
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
			if (part == "") {
				output = "/";
				folder = &this->root;
			}
            else if (part == "..") {
				if (folder->name != "/") {
					if (folder->parent->name == "/")
						output = "/";
					else
						output = output.substr(0, output.find_last_of('/'));
					folder = folder->parent;
				}
            } else {
                Folder* tempFolder = new Folder(part);
                auto it = std::find_if(folder->folders.begin(), folder->folders.end(),
                    [tempFolder](const Folder* f) -> bool{ return *f == *tempFolder; });
                if (it != folder->folders.end()) { // Folder found
					if (output.back() != '/')
						output += "/";
                    output += (*it)->name;
                    folder = *it;
                } else {
                    folder = this->currDir;
                    output = save;
                }
				delete tempFolder;
            }
        } else {
			if (name == "..") {
				if (folder->name != "/") {
					if (folder->parent->name == "/")
						output = "/";
					else
						output = output.substr(0, output.find_last_of('/'));
					folder = folder->parent;
				}
			}
			else {
				Folder* tempFolder = new Folder(name);
				auto it = std::find_if(folder->folders.begin(), folder->folders.end(),
					[tempFolder](const Folder* f) -> bool { return *f == *tempFolder; });
				if (it != folder->folders.end()) { // Folder found
					if (output.back() != '/')
						output += "/";
					output += (*it)->name;
					folder = *it;
				}
				else {
					folder = this->currDir;
					output = save;
				}
				delete tempFolder;
			}
			name = "";
        }
    }
    this->currDir = folder;
    return output;
}

std::string FileSystem::listDir(std::string name, std::string current) {
	std::string output;
	Folder* origFolder = this->currDir;
	if (name != "") 
		std::string temp = goToFolder(name, current);
	for (const auto f : this->currDir->folders)
		output += f->name + '\n';
	for (const auto f : this->currDir->files)
		output += std::to_string(f->blockIds.size() * 512) + '\t' + f->name + '\n';
	this->currDir = origFolder;
	return output;
}

std::string FileSystem::getFileContent(std::string fileName, std::string current) {
	std::string output;
	File* tempFile = new File(fileName);
	auto it = std::find_if(this->currDir->files.begin(), this->currDir->files.end(),
		[tempFile](const File* f) -> bool { return *f == *tempFile; });
	if (it != this->currDir->files.end()) { // Folder found
		for (const auto i : (*it)->blockIds)
			output += mMemblockDevice.readBlock(i).toString().substr(0, mMemblockDevice.readBlock(i).toString().find_first_of('\n'));
	}
	else
		output = '\n';
	delete tempFile;
	return output;
}

void FileSystem::createImage(std::string name) {
	this->currDir = &this->root;
	std::ofstream file;
	file.open(name);
	recCreate(this->currDir, file);
	file.close();
}

void FileSystem::recCreate(Folder* dir, std::ofstream& file) {
	this->currDir = dir;
	size_t folderSize = this->currDir->folders.size();
	size_t fileSize = this->currDir->files.size();
	file << folderSize << std::endl << fileSize << std::endl;
	for (size_t i = 0; i < fileSize; i++)
		file << this->currDir->files[i]->name << '=' << getFileContent(this->currDir->files[i]->name, "") << std::endl;
	for (size_t i = 0; i < folderSize; i++) {
		file << this->currDir->folders[i]->name << std::endl;
		recCreate(&*this->currDir->folders[i], file);
	}
	this->currDir = this->currDir->parent;
}

void FileSystem::loadImage(std::string path) {
	this->currDir = &this->root;
	std::ifstream file(path);
	if (file.is_open())
		recLoad(this->currDir, file);
	file.close();
	this->currDir = &this->root;
}

void FileSystem::recLoad(Folder* dir, std::ifstream& file) {
	this->currDir = dir;
	std::string line;
	int fileCount, folderCount;
	std::getline(file, line);
	folderCount = std::stoi(line);
	std::getline(file, line);
	fileCount = std::stoi(line);
	for (size_t i = 0; i < fileCount; i++) {
		std::getline(file, line);
		std::string name, content;
		size_t pos = line.find('=');
		name = line.substr(0, pos);
		content = line.substr(pos + 1);
		int temp = createFile(name, content, "");
	}
	for (size_t i = 0; i < folderCount; i++)
	{
		std::getline(file, line);
		createFolder(line);
		recLoad(&*this->currDir->folders[i], file);
	}
	this->currDir = this->currDir->parent;
}
