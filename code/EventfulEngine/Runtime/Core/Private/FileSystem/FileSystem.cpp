#pragma once

#include "Platform.h"
#include "FileSystem.h"

#if EF_PLATFORM_WINDOWS
#include "WindowsHeaderWrapper.h"
#endif

#if EF_PLATFORM_LINUX
#include <libgen.h>
#endif

#include <CoreGlobals.h>
#include <EFLogger.h>
#include "EFAssert.h"
#include <thread>
#include <nfd.hpp>
#include "format"

namespace EventfulEngine{


    EFPath FileSystem::GetWorkingDirectory(){
        return std::filesystem::current_path();
    }

    void FileSystem::SetWorkingDirectory(const EFPath& path){
        std::filesystem::current_path(path);
    }

    bool FileSystem::CreateDirectory(const EFPath& directory){
        return std::filesystem::create_directories(directory);
    }

    bool FileSystem::CreateDirectory(const EFString& directory){
        return CreateDirectory(EFPath(directory));
    }

    bool FileSystem::Exists(const EFPath& filepath){
        return std::filesystem::exists(filepath);
    }

    bool FileSystem::Exists(const EFString& filepath){
        return std::filesystem::exists(EFPath(filepath));
    }

    bool FileSystem::DeleteFile(const EFPath& filepath){
        if (!Exists(filepath)){
            return false;
        }

        if (std::filesystem::is_directory(filepath))
            return std::filesystem::remove_all(filepath) > 0;
        return std::filesystem::remove(filepath);
    }

    bool FileSystem::MoveFile(const EFPath& filepath, const EFPath& dest){
        return Move(filepath, dest / filepath.filename());
    }

    bool FileSystem::CopyFile(const EFPath& filepath, const EFPath& dest){
        return Copy(filepath, dest / filepath.filename());
    }

    bool FileSystem::IsDirectory(const EFPath& filepath){
        return std::filesystem::is_directory(filepath);
    }

    FileStatus FileSystem::TryOpenFileAndWait(const EFPath& filepath, const uint64 waitMs){
        const FileStatus fileStatus = TryOpenFile(filepath);
        if (fileStatus == FileStatus::Locked){
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(operator""ms(waitMs));
            return TryOpenFile(filepath);
        }
        return fileStatus;
    }

    bool FileSystem::IsNewer(const EFPath& fileA, const EFPath& fileB){
        return std::filesystem::last_write_time(fileA) > std::filesystem::last_write_time(fileB);
    }

    bool FileSystem::Move(const EFPath& oldFilepath, const EFPath& newFilepath){
        if (Exists(newFilepath)){
            return false;
        }

        std::filesystem::rename(oldFilepath, newFilepath);
        return true;
    }

    bool FileSystem::Copy(const EFPath& oldFilepath, const EFPath& newFilepath){
        if (Exists(newFilepath)){
            return false;
        }

        std::filesystem::copy(oldFilepath, newFilepath);
        return true;
    }

    bool FileSystem::Rename(const EFPath& oldFilepath, const EFPath& newFilepath){
        return Move(oldFilepath, newFilepath);
    }

    bool FileSystem::RenameFilename(const EFPath& oldFilepath, const EFString& newName){
        const std::filesystem::path newPath = oldFilepath.parent_path() / std::filesystem::path(
            newName + oldFilepath.extension().string());
        return Rename(oldFilepath, newPath);
    }

    bool FileSystem::ShowFileInExplorer(const EFPath& path){
        auto absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath))
            return false;

#ifdef EF_PLATFORM_WINDOWS
        std::string cmd = std::format("explorer.exe /select,\"{}\"", absolutePath.string());
#elif defined(EF_PLATFORM_LINUX)
        std::string cmd = std::format("xdg-open \"{0}\"", dirname(absolutePath.string().data()));
#endif
        ::system(cmd.c_str());
        return true;
    }

    bool FileSystem::OpenDirectoryInExplorer(const EFPath& path){
#ifdef EF_PLATFORM_WINDOWS
        const auto absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath)){
            return false;
        }

        ShellExecute(nullptr, reinterpret_cast<LPCSTR>(L"explore"), reinterpret_cast<LPCSTR>(absolutePath.c_str()),
                     nullptr, nullptr, SW_SHOWNORMAL);
        return true;
#elif defined(EF_PLATFORM_LINUX)
        return ShowFileInExplorer(path);
#endif
    }

    bool FileSystem::OpenExternally(const EFPath& path){
        auto absolutePath = std::filesystem::canonical(path);
        if (!Exists(absolutePath)){
            return false;
        }

#ifdef EF_PLATFORM_WINDOWS
        ShellExecute(nullptr, reinterpret_cast<LPCSTR>(L"open"), reinterpret_cast<LPCSTR>(absolutePath.c_str()),
                     nullptr, nullptr, SW_SHOWNORMAL);
        return true;
#elif defined(EF_PLATFORM_LINUX)
        std::string cmd = std::format("xdg-open \"{0}\"", absolutePath.string().data());
        system(cmd.c_str());
        return true;
#endif
    }

    EFPath FileSystem::GetUniqueFileName(const EFPath& filepath){
        if (!Exists(filepath)){
            return filepath;
        }

        const auto parent = filepath.parent_path();
        auto stem = filepath.stem().string(); // filename w/o extension
        auto extension = filepath.extension().string(); // “.txt”, “.cpp”, etc.
        int counter = 1;
        EFPath candidate;

        // keep bumping “(01)”, “(02)”, … until we find a free name
        do{
            EFString pathName;
            // {:02} means “pad to width 2 with leading zeros”
            if (counter <= 10){
                pathName = EFText::Format("{} ({:2}){}", stem, counter, extension);
            }
            else{
                pathName = EFText::Format("{} ({:02}){}", stem, counter, extension);
            }
            candidate = parent / pathName;
            ++counter;
        }
        while (Exists(candidate));

        return candidate;
    }

    uint64 FileSystem::GetLastWriteTime(const EFPath& filepath){
        EF_ASSERT(FileSystem::Exists(filepath), CoreLog, EFText::Format("File {} does not exist!", filepath.string()))

        if (TryOpenFileAndWait(filepath) == FileStatus::Success){
            const std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(filepath);
            return std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();
        }

        EF_ERROR_CAT(CoreLog, "FileSystem::GetLastWriteTime - could not open file: {}", filepath.string());
        return 0;
    }

    EFPath FileSystem::OpenFileDialog(const std::initializer_list<FileDialogFilterItem> inFilters){
        NFD::UniquePath filePath;

        switch (NFD::OpenDialog(filePath, reinterpret_cast<const nfdfilteritem_t*>(inFilters.begin()),
                                static_cast<nfdfiltersize_t>(inFilters.size()))){
        case NFD_OKAY: return filePath.get();
        case NFD_CANCEL: return "";
        case NFD_ERROR:
        default: {
            EF_VERIFY(false, CoreLog, "NFD-Extended threw an error: {}", NFD::GetError());
            return "";
        }
        }
    }

    EFPath FileSystem::OpenFolderDialog(const char* initialFolder){
        NFD::UniquePath filePath;

        switch (NFD::PickFolder(filePath, initialFolder)){
        case NFD_OKAY: return filePath.get();
        case NFD_CANCEL: return "";
        case NFD_ERROR:
        default: {
            EF_VERIFY(false, CoreLog, "NFD-Extended threw an error: {}", NFD::GetError());
            return "";
        }
        }
    }

    EFPath FileSystem::SaveFileDialog(const std::initializer_list<FileDialogFilterItem> inFilters){
        NFD::UniquePath filePath;

        switch (NFD::SaveDialog(filePath, reinterpret_cast<const nfdfilteritem_t*>(inFilters.begin()),
                                static_cast<nfdfiltersize_t>(inFilters.size()))){
        case NFD_OKAY: return filePath.get();
        case NFD_CANCEL: return "";
        case NFD_ERROR:
        default: {
            EF_VERIFY(false, CoreLog, "NFD-Extended threw an error: {}", NFD::GetError());
            return "";
        }
        }
    }
} // EventfulEngine
