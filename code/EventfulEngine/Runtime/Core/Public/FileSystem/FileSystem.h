#pragma once

#include <CoreTypes.h>
#include <filesystem>
#include "EFCoreModuleAPI.h"

namespace EventfulEngine{
	using EFPath = std::filesystem::path;
	/**
	 * Simple registry storing asset identifiers and their paths. This is only a
	 * placeholder that will be expanded once the asset system is implemented.
	 */
	enum class FileStatus{
		Success = 0, Invalid, Locked, OtherError
	};

	/**
	 * Static helper class exposing basic filesystem utilities and commonly used
	 * directories. The implementation relies on std::filesystem and performs only
	 * very small convenience tasks for now.
	 */
	class EFCORE_API FileSystem{
	public:
		static EFPath GetWorkingDirectory();

		static void SetWorkingDirectory(const EFPath& path);

		static bool CreateDirectory(const EFPath& directory);

		static bool CreateDirectory(const EFString& directory);

		static bool Exists(const EFPath& filepath);

		static bool Exists(const EFString& filepath);

		static bool DeleteFile(const EFPath& filepath);

		static bool MoveFile(const EFPath& filepath, const EFPath& dest);

		static bool CopyFile(const EFPath& filepath, const EFPath& dest);

		static bool IsDirectory(const EFPath& filepath);

		static FileStatus TryOpenFile(const EFPath& filepath);

		static FileStatus TryOpenFileAndWait(const EFPath& filepath, uint64 waitMs = 100);

		static bool IsNewer(const EFPath& fileA, const EFPath& fileB);

		static bool Move(const EFPath& oldFilepath, const EFPath& newFilepath);

		static bool Copy(const EFPath& oldFilepath, const EFPath& newFilepath);

		static bool Rename(const EFPath& oldFilepath, const EFPath& newFilepath);

		static bool RenameFilename(const EFPath& oldFilepath, const EFString& newName);

		static bool ShowFileInExplorer(const EFPath& path);

		static bool OpenDirectoryInExplorer(const EFPath& path);

		static bool OpenExternally(const EFPath& path);

		static EFPath GetUniqueFileName(const EFPath& filepath);

		static uint64 GetLastWriteTime(const EFPath& filepath);

		struct FileDialogFilterItem{
			const char* Name;
			const char* Spec;
		};

		static EFPath OpenFileDialog(std::initializer_list<FileDialogFilterItem> inFilters = {});

		static EFPath OpenFolderDialog(const char* initialFolder = "");

		static EFPath SaveFileDialog(std::initializer_list<FileDialogFilterItem> inFilters = {});

		static EFPath GetPersistentStoragePath();

		static bool HasEnvironmentVariable(const EFString& key);

		static bool SetEnvironmentVariable(const EFString& key, const EFString& value);

		static EFString GetEnvironmentVariable(const EFString& key);
	};

} // EventfulEngine
