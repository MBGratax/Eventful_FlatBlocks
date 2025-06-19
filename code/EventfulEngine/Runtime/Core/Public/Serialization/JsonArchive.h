#pragma once

#include "CoreTypes.h"
#include <nlohmann/json.hpp>
#include "FileSystem.h"
#include "EFCoreModuleAPI.h"
#include <typeindex>
#include "EFObject.h"

namespace EventfulEngine{

    class EFCORE_API JsonArchive{
    public:
        bool Load(const EFPath& file);

        bool Save(const EFPath& file) const;

        template <typename T>
        void Set(const EFString& key, const T& value){ _json[key] = value; }

        template <typename T>
        bool Get(const EFString& key, T& out) const{
            if (!_json.contains(key)){
                return false;
            }
            out = _json.at(key).get<T>();
            return true;
        }

        nlohmann::json& Data(){ return _json; }
        const nlohmann::json& Data() const{ return _json; }

    private:
        nlohmann::json _json;
    };

    using JsonWriteFunction = std::function<void(const EFObject*, JsonArchive&, const EFProperty&)>;
    using JsonReadFunction = std::function<void(EFObject*, const JsonArchive&, const EFProperty&)>;

    std::unordered_map<std::type_index, JsonWriteFunction>& G_Writers();

    std::unordered_map<std::type_index, JsonReadFunction>& G_Readers();

    template <typename T>
    void RegisterJsonType();

    void InitJsonLoaders();
} // EventfulEngine
