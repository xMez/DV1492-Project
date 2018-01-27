#include <iostream>
#include <sstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();
std::string help();

/* More functions ... */
std::string trim(std::string input);
std::string listDirectory(std::string userCommand, std::string current, FileSystem& fs);
void createFile(std::string userCommand, std::string current, FileSystem& fs);
std::string printFileContent(std::string userCommand, std::string current, FileSystem& fs);
void createImage(std::string userCommand, FileSystem& fs);
void restoreImage(std::string userCommand, std::string& currentDir, FileSystem* fs);
void createFolder(std::string userCommand, FileSystem& fs);
void removeFile(std::string userCommand, FileSystem& fs);
void copyFile(std::string userCommand, std::string current, FileSystem& fs);
std::string goToFolder(std::string name, std::string output, FileSystem& fs);

int main(void) {

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

    bool bRun = true;

    FileSystem* fs = new FileSystem(); // Make a filesystem

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: //quit
				bRun = quit();                
                break;
			case 1: { // format
				delete fs;
				fs = new FileSystem();
				currentDir = "/";
				break;
			}
            case 2: // ls
                std::cout << "Listing directory" << std::endl << listDirectory(userCommand, currentDir, *fs);
                break;
            case 3: // create
				createFile(userCommand, currentDir, *fs);
                break;
            case 4: // cat
				std::cout << printFileContent(userCommand, currentDir, *fs) << std::endl;
                break;
            case 5: // createImage
				createImage(userCommand, *fs);
                break;
            case 6: // restoreImage
				restoreImage(userCommand, currentDir, fs);
                break;
            case 7: // rm
                removeFile(userCommand, *fs);
                break;
            case 8: // cp
				copyFile(userCommand, currentDir, *fs);
                break;
            case 9: // append
                break;
            case 10: // mv
                break;
            case 11: // mkdir
                createFolder(userCommand, *fs);
                break;
            case 12: // cd
                currentDir = goToFolder(userCommand, currentDir, *fs);
                break;
            case 13: // pwd
				std::cout << currentDir << std::endl;
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);
	delete fs;
    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}

/* Insert code for your shell functions and call them from the switch-case */
std::string trim(std::string input) {
    std::string whitespace = " \t";
    if (input.find_first_not_of(whitespace) == std::string::npos) // Check for empty string
        return "";
    return input.substr(input.find_first_not_of(whitespace), input.find_last_not_of(whitespace) - input.find_first_not_of(whitespace) + 1);
}

std::string listDirectory(std::string userCommand, std::string current, FileSystem& fs) {
	userCommand.erase(0, 3);
	userCommand = trim(userCommand);
	return fs.listDir(userCommand, current);
}

void createFile(std::string userCommand, std::string current, FileSystem& fs) {
	userCommand.erase(0, 7);
	userCommand = trim(userCommand);
	if (userCommand == "")
		std::cout << "Name cannot be empty" << std::endl;
	else if (userCommand.find_first_of(" \t") != std::string::npos)
		std::cout << "Name cannot contain whitespace" << std::endl;
	else {
		std::cout << "Enter file content:" << std::endl;
		std::string content;
		std::getline(std::cin, content);
		int rtn = fs.createFile(userCommand, content, current);
		if (rtn == -1)
			std::cout << "cannot create file '" << userCommand << "': File exists" << std::endl;
		else if (rtn == -2)
			std::cout << "cannot create file '" << userCommand << "': Out of memory" << std::endl;
		else
			std::cout << userCommand << " was created" << std::endl;
	}
}

std::string printFileContent(std::string userCommand, std::string current, FileSystem& fs) {
	std::string output;
	userCommand.erase(0, 4);
	userCommand = trim(userCommand);
	output = fs.getFileContent(userCommand, current);
	if (output == std::string(1, '\n'))
		output = userCommand + ": No such file or directory";
	return output;
}

void createImage(std::string userCommand, FileSystem& fs) {
	userCommand.erase(0, 12);
	userCommand = trim(userCommand);
	fs.createImage(userCommand);
}

void restoreImage(std::string userCommand, std::string& currentDir, FileSystem* fs) {
	userCommand.erase(0, 13);
	userCommand = trim(userCommand);
	currentDir = "/";
	delete fs;
	fs = new FileSystem();
	fs->goToFolder("..", currentDir);
	fs->loadImage(userCommand);
}

void createFolder(std::string userCommand, FileSystem& fs) {
    userCommand.erase(0, 6);
    userCommand = trim(userCommand);
    if (userCommand == "")
        std::cout << "Name cannot be empty" << std::endl;
    else if (userCommand.find_first_of("/ \t") != std::string::npos)
        std::cout << "Name cannot contain '/' or whitespace" << std::endl;
    else if (fs.createFolder(userCommand))
        std::cout << userCommand << " was created" << std::endl;
    else
        std::cout << "cannot create directory '" << userCommand << "': File exists" << std::endl;
}

void removeFile(std::string userCommand, FileSystem& fs) {
    userCommand.erase(0, 3);
    userCommand = trim(userCommand);
    if (fs.removeFile(userCommand))
        std::cout << userCommand << " was deleted" << std::endl;
    else
        std::cout << "cannot delete '" << userCommand << "': File does not exists" << std::endl;
}

void copyFile(std::string userCommand, std::string current, FileSystem& fs) {
	userCommand.erase(0, 3);
	userCommand = trim(userCommand);
	size_t pos = userCommand.find(" ");
	std::string file1, file2, content;
	file1 = userCommand.substr(0, pos);
	file2 = userCommand.substr(pos + 1);
	content = fs.getFileContent(file1, current);
	if (content != std::string(1, '\n')) {
		fs.createFile(file2, content, current);
	}
	else
		std::cout << content << std::endl;
}

std::string goToFolder(std::string userCommand, std::string output, FileSystem& fs) {
    userCommand.erase(0, 3);
    userCommand = trim(userCommand);
    return fs.goToFolder(userCommand, output);
}