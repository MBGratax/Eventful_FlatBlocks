#pragma once

#include "../../Public/EventfulObject/EFReflectionManager.h"

#include <EfMemory.h>
#include <ranges>

namespace EventfulEngine{
    EFClassPtr EFReflectionManager::RegisterClass(const std::string_view name, const std::size_t hash,
                                                  const std::size_t parentHash){
        auto& data = _classes[hash];
        if (!data)
            data = MakeShared<EFClass>();
        data->Name = name;
        data->Hash = hash;
        data->ParentHash = parentHash;
        return data;
    }

    void EFReflectionManager::RegisterProperty(const std::size_t classHash, const EFProperty& property){
        auto& cls = _classes[classHash];
        if (!cls)
            cls = MakeShared<EFClass>();
        cls->Properties.push_back(property);
    }

    void EFReflectionManager::RegisterMethod(const std::size_t classHash, EFMethod method){
        auto& cls = _classes[classHash];
        if (!cls)
            cls = MakeShared<EFClass>();
        cls->Methods.push_back(std::move(method));
    }

    EFClassPtr EFReflectionManager::GetClass(const std::size_t hash) const{
        if (const auto it = _classes.find(hash); it != _classes.end())
            return it->second;
        return nullptr;
    }

    EFClassPtr EFReflectionManager::GetClass(const std::string_view name) const{
        for (auto& efClassPtr : _classes | std::views::values){
            if (efClassPtr->Name == name)
                return std::cref(efClassPtr);
        }
        return nullptr;
    }
} // EventfulEngine
