#pragma once

#include "FileSystem.h"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <CoreGlobals.h>
#include <EFAssert.h>

#include <GLFW/glfw3native.h>
#include "WindowsHeaderWrapper.h"

namespace EventfulEngine{
    static EFPath g_persistentStoragePath;

    FileStatus FileSystem::TryOpenFile(const EFPath& filepath){
        using enum FileStatus;
        const HANDLE fileHandle = CreateFile(reinterpret_cast<LPCSTR>(filepath.c_str()), GENERIC_READ, 0, nullptr,
                                             OPEN_EXISTING, 0, nullptr);
        if (fileHandle == INVALID_HANDLE_VALUE){
            const DWORD errorCode = GetLastError();
            if (errorCode == ERROR_FILE_NOT_FOUND || errorCode == ERROR_PATH_NOT_FOUND){
                return Invalid;
            }
            if (errorCode == ERROR_SHARING_VIOLATION){
                return Locked;
            }

            return OtherError;
        }

        CloseHandle(fileHandle);
        return Success;
    }

    EFPath FileSystem::GetPersistentStoragePath(){
        if (!g_persistentStoragePath.empty())
            return g_persistentStoragePath;

        PWSTR roamingFilePath;
        const HRESULT result =
            SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &roamingFilePath);
        EF_VERIFY(result == S_OK, CoreLog, "");
        g_persistentStoragePath = roamingFilePath;
        g_persistentStoragePath /= "EventBox";

        if (!std::filesystem::exists(g_persistentStoragePath)){
            std::filesystem::create_directory(g_persistentStoragePath);
        }

        return g_persistentStoragePath;
    }

    bool FileSystem::HasEnvironmentVariable(const EFString& key){
        HKEY hKey;
        LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey);

        if (lOpenStatus == ERROR_SUCCESS){
            lOpenStatus = RegQueryValueExA(hKey, key.c_str(), nullptr, nullptr, nullptr, nullptr);
            RegCloseKey(hKey);
        }

        return lOpenStatus == ERROR_SUCCESS;
    }

    bool FileSystem::SetEnvironmentVariable(const EFString& key, const EFString& value){
        HKEY hKey;
        const LPCSTR keyPath = "Environment";
        DWORD createdNewKey;
        const LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, nullptr, REG_OPTION_NON_VOLATILE,
                                                    KEY_ALL_ACCESS, nullptr, &hKey, &createdNewKey);
        if (lOpenStatus == ERROR_SUCCESS){
            const LSTATUS lSetStatus = RegSetValueExA(hKey, key.c_str(), 0, REG_SZ, LPBYTE(value.c_str()),
                                                      static_cast<DWORD>(value.length() + 1));
            RegCloseKey(hKey);

            if (lSetStatus == ERROR_SUCCESS){
                SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, reinterpret_cast<LPARAM>("Environment"),
                                    SMTO_BLOCK, 100,
                                    nullptr);
                return true;
            }
        }

        return false;
    }

    std::string FileSystem::GetEnvironmentVariable(const EFString& key){
        char* value = nullptr;
        size_t size = 0;
        if (const errno_t err = ::_dupenv_s(&value, &size, key.c_str()); err == 0 && value != nullptr){
            EFString result{value, size};
            ::free(value);
            return result;
        }
        if (value){
            ::free(value);
        }
        return {};
    }
}
