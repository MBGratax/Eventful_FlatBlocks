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


    std::unordered_map<std::type_index, JsonWriteFunction>& GWriters(){
        static std::unordered_map<std::type_index, JsonWriteFunction> writers;
        if (writers.empty()){
            InitJsonLoaders();
        }
        return writers;
    }

    std::unordered_map<std::type_index, JsonReadFunction>& GReaders(){
        static std::unordered_map<std::type_index, JsonReadFunction> readers;
        if (readers.empty()){
            InitJsonLoaders();
        }
        return readers;
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
