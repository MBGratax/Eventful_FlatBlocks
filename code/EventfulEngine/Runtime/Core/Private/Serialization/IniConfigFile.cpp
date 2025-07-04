#pragma once

#include "IniConfigFile.h"
#include <fstream>

namespace EventfulEngine{
    bool IniConfigFile::Load(const EFPath& file, IniFile& out){
        std::ifstream in(file);
        if (!in.is_open()){
            return false;
        }

        EFString line;
        EFString current;
        while (std::getline(in, line)){
            // Ignore commented line
            if (line.empty() || line[0] == '#' || line[0] == ';'){
                continue;
            }
            // Ignore Header Line
            if (line.front() == '[' && line.back() == ']'){
                current = line.substr(1, line.size() - 2);
                continue;
            }

            const auto position = line.find('=');
            if (position == EFString::npos){
                continue;
            }

            EFString key = line.substr(0, position);
            EFString value = line.substr(position + 1);
            // Map to [current] -> [key][value]
            // #A Random Comment <- Ignored
            // [Header] <- Used as map for sub pairs
            //    key = value <- Map Header -> {key->value}
            // <- Empty line is skipped
            auto& section = out.try_emplace(current, IniSection{}).first->second;
            section.insert_or_assign(std::move(key), std::move(value));
        }
        return true;
    }

    bool IniConfigFile::Save(const EFPath& file, const IniFile& ini){
        std::ofstream out(file);
        if (!out.is_open()){
            return false;
        }

        for (const auto& [section, pairs] : ini){
            if (!section.empty()){
                out << '[' << section << ']' << '\n';
            }
            for (const auto& [iniEntry,iniEntryValue] : pairs){
                out << iniEntry << '=' << iniEntryValue << '\n';
            }
            out << '\n';
        }
        return true;
    }


} // EventfulEngine
