#pragma once
#include "EfMemory.h"

#include "EFClass.h"
#include "IManager.h"
#include <unordered_map>

namespace EventfulEngine{
    // Global Registry for Eventful Reflection, holds all Reflection data that was registered and provides methods to
    // get info on classes or invoke methods/get members
    using EFClassPtr = std::shared_ptr<EFClass>;

    class EFReflectionManager : public IManager<EFReflectionManager>{
    public:
        EFClassPtr RegisterClass(EFClassPtr& cls);

        void RegisterProperty(std::size_t classHash, const EFProperty& property);

        void RegisterMethod(std::size_t classHash, EFMethod method);

        EFClassPtr GetClass(std::size_t hash) const;

        EFClassPtr GetClass(std::string_view name) const;

        EFClassPtr GetClass(std::type_index type) const;

    private:
        std::unordered_map<std::size_t, EFClassPtr> _classes;
    };
}
