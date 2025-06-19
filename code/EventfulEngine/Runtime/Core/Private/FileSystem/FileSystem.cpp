#pragma once

#include "FileSystem.h"

#include "CoreTypes.h"

namespace EventfulEngine{

    //TODO: Make sure either that CMake install rules guarantee this, or make CMake give us the path via Config Generator Defines!
    EFPath FileSystem::GetAssetsPath(){
        return std::filesystem::current_path() / "Assets";
    }

    //TODO: We should also provide another safe path that is in the project folder aka currentDir/Saved if the user so chooses
    // not sure how to name it
    EFPath FileSystem::GetSafePath(){
        EFPath safeDir;
        EFChar* path;
        size_t pathSize;
        // TODO: Platform-specific code here, should probably be moved or defined by PlatformAbstraction
#ifdef _WIN32
        if (::_dupenv_s(&path, &pathSize, "APPDATA")){
            safeDir = path;
        }
        else{
            safeDir = std::filesystem::temp_directory_path();
        }
#else
        if(::_dupenv_s(&path, &pathSize, "HOME")){
            saveDir = home;
        }else{
            saveDir = std::filesystem::temp_directory_path();
        }
#endif
        // TODO: Resolve name of this folder to mirror CompanyName/ProjectName once those settings exist
        safeDir /= "EvenfulEngine/EventfulProject";
        if (!DirectoryExists(safeDir)){
            std::filesystem::create_directories(safeDir);
        }
        return safeDir;
    }

    bool FileSystem::FileExists(const EFPath& p){
        return std::filesystem::exists(p) && !std::filesystem::is_directory(p);
    }

    bool FileSystem::DirectoryExists(const EFPath& p){
        return std::filesystem::exists(p) && std::filesystem::is_directory(p);
    }

    bool FileSystem::CreateDirectory(const EFPath& p){
        return std::filesystem::create_directories(p);
    }

    bool FileSystem::Remove(const EFPath& p){
        return std::filesystem::remove_all(p) > 0;
    }

    std::vector<std::filesystem::path> FileSystem::EnumerateFiles(const EFPath& dir){
        std::vector<EFPath> files;
        if (!DirectoryExists(dir))
            return files;

        for (const auto& entry : std::filesystem::directory_iterator(dir)){
            files.push_back(entry.path());
        }
        return files;
    }
} // EventfulEngine
