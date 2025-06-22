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
        [[nodiscard]] const nlohmann::json& Data() const{ return _json; }

    private:
        nlohmann::json _json;
    };

    using JsonWriteFunction = std::function<void(const EFObject*, JsonArchive&, const EFProperty&)>;
    using JsonReadFunction = std::function<void(EFObject*, const JsonArchive&, const EFProperty&)>;

    std::unordered_map<std::type_index, JsonWriteFunction>& G_Writers();

    std::unordered_map<std::type_index, JsonReadFunction>& G_Readers();

    // TODO: Instead of just EFObject, we need to handle this on a per struct/Type basis
    // TODO: This automated version is cool and all, but quickly crumbles once we get to structs or classes
    template <typename T>
    void EFCORE_API RegisterJsonType(){
        auto& g_Writers = G_Writers();
        auto& g_Readers = G_Readers();
        if (g_Writers.contains(typeid(T)) && g_Readers.contains(typeid(T))){
            return;
        }
        if constexpr (std::is_base_of_v<EFObject, T>){
            g_Writers[typeid(T)] =
                [](const EFObject* instance,
                   JsonArchive& ar,
                   const EFProperty& prop){
                    std::any raw = prop.AutoProperty->Get(instance);
                    const EFObject* const val = any_cast<const EFObject*>(std::move(raw));
                    ar.Set(prop.Name, nlohmann::json::object({{"Name", val->GetName()}, {"GUID", val->GetGUID()}}));
                };

            g_Readers[typeid(T)] =
                [](EFObject* instance,
                   const JsonArchive& ar,
                   const EFProperty& prop){
                    if (nlohmann::json val; ar.Get(prop.Name, val)){
                        JsonArchive subArchive;
                        subArchive.Data() = val;
                        /// TODO: Since EFObjects can only be references when serialized, here we should queue a
                        /// deferred call that tells our deserialization system to restore references once everything
                        /// is loaded
                    }
                };
        }
        else{
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
    }

    void InitJsonLoaders();
} // EventfulEngine
