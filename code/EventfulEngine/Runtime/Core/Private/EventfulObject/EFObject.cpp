#pragma once

#include "../../Public/EventfulObject/EFObject.h"

#include <JsonArchive.h>

namespace EventfulEngine{
    EFREGISTER(EFObject)
        EFPROPERTY(_bIsValid, E_PropertyFlags::None, {"Category", {"Validation"}})
        EFMETHOD(IsValid, E_MethodFlags::None, {"Category", {"Validation"}})
    EFREGISTER_END(EFObject)

    // Intentional recursion, should be safe with the checks below. TODO: Think about turning recursion into coroutine
    void EFObject::Serialize(JsonArchive& ar) const{
        const auto& writers = G_Writers();
        for (const auto& property : _efClass->Properties){
            if (auto iterator = writers.find(property.Type); iterator != writers.end()){
                iterator->second(this, ar, property);
            }

            // Is the property a subObject?
            if (const auto subClass = EFReflectionManager::Get().GetClass(property.Type); subClass){
                // Is there an actual instance?
                if (const EFObject* const obj = any_cast<const EFObject*>(property.AutoProperty->Get(this)); obj &&
                    subClass->Hash != _efClass->Hash){
                    JsonArchive subArchive;
                    obj->Serialize(subArchive);
                    ar.Set(property.Name, subArchive.Data());
                }
            }
        }
    }

    void EFObject::Deserialize(const JsonArchive& ar){
        if (const auto efClass = EFReflectionManager::Get().GetClass(GetClassName()); !efClass){
            static_assert("Somehow tried Deserializing a EFObject from JsonArchive before load time init?");
            return;
        }

        const auto& readers = G_Readers();
        for (const auto& property : _efClass->Properties){
            if (auto iterator = readers.find(property.Type); iterator != readers.end()){
                iterator->second(this, ar, property);
            }

            // Is the property a subObject?
            if (const auto subClass = EFReflectionManager::Get().GetClass(property.Type); subClass){
                // Is there an actual instance?
                if (EFObject* const obj = any_cast<EFObject*>(property.AutoProperty->Get(this)); obj &&
                    subClass->Hash != _efClass->Hash){
                    if (auto subIterator = ar.Data().find(property.Name); subIterator != ar.Data().end()){
                        JsonArchive subArchive;
                        subArchive.Data() = *subIterator;
                        obj->Deserialize(subArchive);
                    }
                }
            }
        }
    }
}
