#pragma once

#include "FileSystem.h"

#include "CoreTypes.h"

namespace EventfulEngine{

    //TODO: Make sure either that CMake install rules guarantee this, or make CMake give us the path via Config Generator Defines!
    std::filesystem::path FileSystem::GetAssetsPath(){
        return std::filesystem::current_path() / "Assets";
    }

    std::filesystem::path FileSystem::GetSafeSavePath(){
        std::filesystem::path saveDir;
        EFChar* path;
        size_t pathSize;
        // TODO: Platformspicifc code here, should probably be moved or defined by PlatformAbstraction
#ifdef _WIN32
        if (::_dupenv_s(&path, &pathSize, "APPDATA")){
            saveDir = path;
        }
        else{
            saveDir = std::filesystem::temp_directory_path();
        }
#else
        if(::_dupenv_s(&path, &pathSize, "HOME")){
            saveDir = home;
        }else{
            saveDir = std::filesystem::temp_directory_path();
        }
#endif
        // TODO: Resolve name of this folder to mirror CompanyName/ProjectName once those settings exist
        saveDir /= "EventfulSaves";
        if (!DirectoryExists(saveDir)){
            std::filesystem::create_directories(saveDir);
        }
        return saveDir;
    }

    bool FileSystem::FileExists(const std::filesystem::path& p){
        return std::filesystem::exists(p) && !std::filesystem::is_directory(p);
    }

    bool FileSystem::DirectoryExists(const std::filesystem::path& p){
        return std::filesystem::exists(p) && std::filesystem::is_directory(p);
    }

    bool FileSystem::CreateDirectory(const std::filesystem::path& p){
        return std::filesystem::create_directories(p);
    }

    bool FileSystem::Remove(const std::filesystem::path& p){
        return std::filesystem::remove_all(p) > 0;
    }

    std::vector<std::filesystem::path> FileSystem::EnumerateFiles(const std::filesystem::path& dir){
        std::vector<std::filesystem::path> files;
        if (!DirectoryExists(dir))
            return files;

        for (const auto& entry : std::filesystem::directory_iterator(dir)){
            files.push_back(entry.path());
        }
        return files;
    }
} // EventfulEngine
