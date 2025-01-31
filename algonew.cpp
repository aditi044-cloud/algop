#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <set>

namespace fs = std::filesystem;

// Tree Node Structure for Stage 2
struct TreeNode {
    std::string name;
    bool isDirectory;
    std::map<std::string, TreeNode*> children;

    TreeNode(std::string n, bool isDir) : name(std::move(n)), isDirectory(isDir) {}
};

class FileManager {
private:
    TreeNode* root;
    std::map<std::string, TreeNode*> pathMap;

public:
    FileManager() {
        root = new TreeNode("/", true);
        pathMap["/"] = root;
    }
// creating a file
    void createFile(const std::string& path) {
        auto [parentPath, fileName] = splitPath(path);
        if (pathMap.find(parentPath) != pathMap.end()) {
            TreeNode* parent = pathMap[parentPath];
            parent->children[fileName] = new TreeNode(fileName, false);
            pathMap[path] = parent->children[fileName];
            std::cout << "File created: " << path << "\n";
        } else {
            std::cerr << "Error: Parent directory does not exist.\n";
        }
    }
// creating a directory
    void createDirectory(const std::string& path) {
        auto [parentPath, dirName] = splitPath(path);
        if (pathMap.find(parentPath) != pathMap.end()) {
            TreeNode* parent = pathMap[parentPath];
            parent->children[dirName] = new TreeNode(dirName, true);
            pathMap[path] = parent->children[dirName];
            std::cout << "Directory created: " << path << "\n";
        } else {
            std::cerr << "Error: Parent directory does not exist.\n";
        }
    }
// deleting a file or directory

    void deleteFileOrDirectory(const std::string& path) {
        if (pathMap.find(path) != pathMap.end()) {
            delete pathMap[path];
            pathMap.erase(path);
            std::cout << "Deleted: " << path << "\n";
        } else {
            std::cerr << "Error: Path not found.\n";
        }
    }
// moving a file or directory
    void moveFileOrDirectory(const std::string& src, const std::string& dest) {
        if (pathMap.find(src) != pathMap.end()) {
            pathMap[dest] = pathMap[src];
            pathMap.erase(src);
            std::cout << "Moved: " << src << " -> " << dest << "\n";
        } else {
            std::cerr << "Error: Source path not found.\n";
        }
    }
// displaying the file system hierarchy
    void displayHierarchy(TreeNode* node, int level = 0) {
        if (!node) return;
        std::cout << std::string(level * 2, ' ') << "|- " << node->name << "\n";
        for (auto& child : node->children) {
            displayHierarchy(child.second, level + 1);
        }
    }
// basically a recursive function to print the complete hierarchy
    void displayHierarchy() {
        displayHierarchy(root);
    }
// for user input, user can interactively manage files and directories
    void interactiveMenu() {
        int choice;
        std::string path, src, dest;
        while (true) {
            std::cout << "\nFile Management System";
            std::cout << "\n1. Create File";
            std::cout << "\n2. Create Directory";
            std::cout << "\n3. Delete File/Directory";
            std::cout << "\n4. Move File/Directory";
            std::cout << "\n5. Display Hierarchy";
            std::cout << "\n6. Exit";
            std::cout << "\nEnter choice: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    std::cout << "Enter file path: ";
                    std::getline(std::cin, path);
                    createFile(path);
                    break;
                case 2:
                    std::cout << "Enter directory path: ";
                    std::getline(std::cin, path);
                    createDirectory(path);
                    break;
                case 3:
                    std::cout << "Enter path to delete: ";
                    std::getline(std::cin, path);
                    deleteFileOrDirectory(path);
                    break;
                case 4:
                    std::cout << "Enter source path: ";
                    std::getline(std::cin, src);
                    std::cout << "Enter destination path: ";
                    std::getline(std::cin, dest);
                    moveFileOrDirectory(src, dest);
                    break;
                case 5:
                    displayHierarchy();
                    break;
                case 6:
                    return;
                default:
                    std::cout << "Invalid choice, try again.\n";
            }
        }
    }

private:
    std::pair<std::string, std::string> splitPath(const std::string& path) {
        size_t pos = path.find_last_of("/");
        if (pos == std::string::npos) return {"/", path};
        return {path.substr(0, pos), path.substr(pos + 1)};
    }
};
//main
int main() {
    FileManager fm;
    fm.interactiveMenu();
    return 0;
}
