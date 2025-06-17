#pragma once

#include "../../Public/EventfulObject/EFObject.h"

namespace EventfulEngine{

    bool __ReflectedClass_EFObject();

    namespace{
        [[maybe_unused]] const bool _ef_register_EFObject = __ReflectedClass_EFObject();
    }

    bool __ReflectedClass_EFObject(){
        using registeringClass = EFObject;
        const auto clsHash = typeid(registeringClass).hash_code();
        if (auto classData = EFReflectionManager::Get().GetClass(clsHash); !classData){
            classData = EFReflectionManager::Get().RegisterClass(registeringClass::_name, clsHash,
                                                                 typeid(registeringClass::_superClass).hash_code());
        }
        EFReflectionManager::Get()
            .RegisterProperty(clsHash,
                              {
                                  "_bIsValid",
                                  getMemberOffset(&registeringClass::_bIsValid),
                                  typeid(decltype(registeringClass::_bIsValid)).name(),
                                  E_PropertyFlags::None,
                                  EFMetaDataList{{"Category", {"Validity"}}}
                              }
            );
        using MethodPointer = decltype(&registeringClass::IsValid);
        using CallableT = EFCallableMethod<MethodPointer>;
        auto methodData = make_method_data(&registeringClass::IsValid);
        auto efCallable = std::make_unique<CallableT>(methodData);

        EFReflectionManager::Get()
            .RegisterMethod(clsHash,
                            EFMethod{
                                "IsValid",
                                std::move(efCallable),
                                E_MethodFlags::None,
                                EFMetaDataList{{"Category", {"Validity"}}}
                            });
        return true;
    }
}
