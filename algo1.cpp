#include <iostream>
#include <vector>
#include <string>

class File {
public:
    std::string name;
    File(std::string name) : name(name) {}
};

class Directory {
public:
    std::string name;
    std::vector<Directory*> subDirectories;
    std::vector<File*> files;
    
    Directory(std::string name) : name(name) {}
    
    void addFile(File* file) {
        files.push_back(file);
    }
    
    void addSubDirectory(Directory* directory) {
        subDirectories.push_back(directory);
    }
    
    bool removeFile(const std::string& filename) {
        for (auto it = files.begin(); it != files.end(); ++it) {
            if ((*it)->name == filename) {
                delete *it;
                files.erase(it);
                return true;
            }
        }
        return false;
    }
    
    Directory* findSubDirectory(const std::string& subDirName) {
        for (auto& subdir : subDirectories) {
            if (subdir->name == subDirName) {
                return subdir;
            }
        }
        return nullptr;
    }
    
    bool fileExists(const std::string& filename) {
        for (auto& file : files) {
            if (file->name == filename) {
                return true;
            }
        }
        return false;
    }
};

class FileManager {
public:
    Directory* root;
    
    FileManager() {
        root = new Directory("root");  // Initialize root directory
    }

    // Create file in specified directory
    void createFile(Directory* dir, const std::string& filename) {
        if (dir->fileExists(filename)) {
            std::cout << "File " << filename << " already exists." << std::endl;
            return;
        }
        dir->addFile(new File(filename));
        std::cout << "File " << filename << " created in " << dir->name << "." << std::endl;
    }

    // Delete file from specified directory
    void deleteFile(Directory* dir, const std::string& filename) {
        if (!dir->removeFile(filename)) {
            std::cout << "File " << filename << " not found in " << dir->name << "." << std::endl;
        } else {
            std::cout << "File " << filename << " deleted from " << dir->name << "." << std::endl;
        }
    }

    // Move file from one directory to another
    void moveFile(Directory* fromDir, Directory* toDir, const std::string& filename) {
        if (fromDir->fileExists(filename)) {
            File* file = nullptr;
            // Remove file from source directory
            for (auto it = fromDir->files.begin(); it != fromDir->files.end(); ++it) {
                if ((*it)->name == filename) {
                    file = *it;
                    fromDir->files.erase(it);
                    break;
                }
            }
            toDir->addFile(file);
            std::cout << "File " << filename << " moved from " << fromDir->name << " to " << toDir->name << "." << std::endl;
        } else {
            std::cout << "File " << filename << " not found in " << fromDir->name << "." << std::endl;
        }
    }

    // Copy file from one directory to another
    void copyFile(Directory* fromDir, Directory* toDir, const std::string& filename) {
        if (fromDir->fileExists(filename)) {
            toDir->addFile(new File(filename));  // Create a new file with the same name
            std::cout << "File " << filename << " copied from " << fromDir->name << " to " << toDir->name << "." << std::endl;
        } else {
            std::cout << "File " << filename << " not found in " << fromDir->name << "." << std::endl;
        }
    }

    // Display the file system structure
    void displayDirectoryStructure(Directory* dir, int indent = 0) {
        std::string indentStr(indent, ' ');
        std::cout << indentStr << dir->name << "/" << std::endl;

        // Display files in the current directory
        for (auto& file : dir->files) {
            std::cout << indentStr << "  " << file->name << std::endl;
        }

        // Recursively display subdirectories
        for (auto& subdir : dir->subDirectories) {
            displayDirectoryStructure(subdir, indent + 2);
        }
    }
};

// Test the FileManager system
int main() {
    FileManager fm;
    
    Directory* root = fm.root;
    Directory* docs = new Directory("Documents");
    Directory* pics = new Directory("Pictures");
    
    root->addSubDirectory(docs);
    root->addSubDirectory(pics);
    
    // Create files in the directories
    fm.createFile(docs, "file1.txt");
    fm.createFile(docs, "file2.txt");
    fm.createFile(pics, "pic1.jpg");
    
    // Display directory structure
    std::cout << "File System Structure:" << std::endl;
    fm.displayDirectoryStructure(root);
    
    // Perform file operations
    fm.moveFile(docs, pics, "file1.txt");
    fm.copyFile(pics, docs, "file2.txt");
    
    // Display after operations
    std::cout << "\nAfter moving and copying files:" << std::endl;
    fm.displayDirectoryStructure(root);
    
    // Delete a file
    fm.deleteFile(pics, "file1.txt");
    std::cout << "\nAfter deleting file1.txt:" << std::endl;
    fm.displayDirectoryStructure(root);
    
    return 0;
}
