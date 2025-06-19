#pragma once

#include <unordered_map>
#include "CoreTypes.h"
#include "FileSystem.h"
#include "EFCoreModuleAPI.h"

namespace EventfulEngine {

using IniSection = std::unordered_map<EFString, EFString>;
using IniFile = std::unordered_map<EFString, IniSection>;

class EFCORE_API IIniSerializable{
public:
    virtual ~IIniSerializable() = default;
    virtual void LoadFromIni(const IniFile& ini) = 0;
    virtual void SaveToIni(IniFile& ini) const = 0;
};

class EFCORE_API IniConfigFile {
public:
static bool Load(const EFPath& file, IniFile& out);
    static bool Save(const EFPath& file, const IniFile& ini);
};

} // EventfulEngine
