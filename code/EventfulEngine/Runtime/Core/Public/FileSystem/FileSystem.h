#pragma once

#include <filesystem>
#include <unordered_map>
#include "EFCoreModuleAPI.h"

namespace EventfulEngine{
    typedef std::filesystem::path EFPath;
    /**
     * Simple registry storing asset identifiers and their paths. This is only a
     * placeholder that will be expanded once the asset system is implemented.
     */
    struct EFCORE_API AssetRegistry{
        //TODO: Store by GUID instead of string for cheaper more reliable lookup
        //TODO: Add helpers to find GUID by name/path
        std::unordered_map<std::string, EFPath> assets;
    };

    /**
     * Static helper class exposing basic filesystem utilities and commonly used
     * directories. The implementation relies on std::filesystem and performs only
     * very small convenience tasks for now.
     */
    class EFCORE_API FileSystem{
    public:
        /** Path where engine assets reside. */
        static EFPath GetAssetsPath();

        /** Platform agnostic writable directory (created if missing). */
        static EFPath GetSafePath();

        static bool FileExists(const EFPath& p);

        static bool DirectoryExists(const EFPath& p);

        static bool CreateDirectory(const EFPath& p);

        static bool Remove(const EFPath& p);

        static std::vector<EFPath>
        EnumerateFiles(const EFPath& dir);
    };

} // EventfulEngine
