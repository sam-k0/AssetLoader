#pragma once

#include <string>
#include <direct.h>
#include <map>
#include <iostream>
#include <fstream>
#include <filesystem>

// File functions


namespace Filesys {

std::string GetCurrentDir() // Returns EXE directory
{
    char cCurrentPath[FILENAME_MAX]; // get working directory into buffer
    if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
        exit(-1);
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

    char* s = cCurrentPath; // save path from buffer into currentpath chararr
    std::string str(s);
    //free(s);
    return str;
}

// Returns EXE directory + Assets + double backslash
std::string GetAssetDir()
{
    return GetCurrentDir() + "\\Assets\\";
}

inline bool FileExists(const std::string& name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::string> findFilesWithExtension(const std::string& directory_path, const std::string& extension) {
    std::vector<std::string> found_files;
    
    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
        if (entry.path().extension() == extension) {
            found_files.push_back(entry.path().string());
        }
    }

    return found_files;
}

void SaveDataToFile(const std::map<std::string, std::string>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : data) {
            file << pair.first << "=" << pair.second << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

std::map<std::string, std::string> LoadDataFromFile(const std::string& filename) {
    std::map<std::string, std::string> data;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                data[key] = value;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return data;
}

int GetDataImageCount(std::map<std::string, std::string> dmap)
{
    std::string key_imagecount = "IMAGECOUNT";
    if(dmap.find(key_imagecount) != dmap.end())
    {
        return std::stoi(dmap[key_imagecount]);
    }
    else
    {
        return 0;
    }
}

std::string GetDataSpritesheet(std::map<std::string, std::string> dmap)
{
    std::string key_image = "SPRITESHEET";
    if(dmap.find(key_image) != dmap.end())
    {
        return dmap[key_image];
    }
    else
    {
        return "";
    }
}


}