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

    template <typename T>
    static void RegisterJsonType(){
        auto& g_Writers = G_Writers();
        auto& g_Readers = G_Readers();
        g_Writers[typeid(T)] =
            [](const EFObject* instance,
               JsonArchive& ar,
               const EFProperty& prop){
                std::any raw = prop.AutoProperty->Get(instance);
                T val = std::any_cast<T>(std::move(raw));
                ar.Set(prop.Name, val);
            };

        g_Readers[typeid(T)] =
            [](EFObject* instance,
               const JsonArchive& ar,
               const EFProperty& prop){
                if (T val; ar.Get(prop.Name, val)){
                    prop.AutoProperty->Set(instance, std::any(std::move(val)));
                }
            };
    }

    void InitJsonLoaders(){
        RegisterJsonType<bool>();

        RegisterJsonType<int>();

        RegisterJsonType<float>();

        RegisterJsonType<double>();

        RegisterJsonType<EFString>();
    }
} // EventfulEngine
