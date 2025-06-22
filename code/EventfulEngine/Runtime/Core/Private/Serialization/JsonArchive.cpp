#pragma once

#include "JsonArchive.h"

#include <fstream>


namespace EventfulEngine{
    bool JsonArchive::Load(const EFPath& file){
        std::ifstream in(file);
        if (!in.is_open()){
            return false;
        }
        in >> _json;
        return true;
    }

    bool JsonArchive::Save(const EFPath& file) const{
        std::ofstream out(file);
        if (!out.is_open()){
            return false;
        }
        out << _json.dump(4);
        return true;
    }


    std::unordered_map<std::type_index, JsonWriteFunction>& G_Writers(){
        static std::unordered_map<std::type_index, JsonWriteFunction> g_Writers;
        if (g_Writers.empty()){
            InitJsonLoaders();
        }
        return g_Writers;
    }

    std::unordered_map<std::type_index, JsonReadFunction>& G_Readers(){
        static std::unordered_map<std::type_index, JsonReadFunction> g_Readers;
        if (g_Readers.empty()){
            InitJsonLoaders();
        }
        return g_Readers;
    }

    //TODO: Refactor this whole jsontype registration to per-file JsonArchive& operator<<(JsonArchive& Ar, T& value)
    void InitJsonLoaders(){
        RegisterJsonType<bool>();

        RegisterJsonType<int>();

        RegisterJsonType<float>();

        RegisterJsonType<double>();

        RegisterJsonType<EFString>();
    }
} // EventfulEngine
