#pragma once

#include "../../Public/EventfulObject/EFObject.h"

#include <JsonArchive.h>

namespace EventfulEngine{
    EFREGISTER(EFObject)
        EFPROPERTY(_bIsValid, E_PropertyFlags::None, {"Category", {"Validation"}})
        EFMETHOD(IsValid, E_MethodFlags::None, {"Category", {"Validation"}})
    EFREGISTER_END(EFObject)

    void EFObject::Serialize(JsonArchive& ar) const{
        const auto& writers = G_Writers();
        for (const auto& property : _efClass->Properties){
            if (auto iterator = writers.find(property.Type); iterator != writers.end()){
                iterator->second(this, ar, property);
                continue;
            }
        }
    }

    void EFObject::Deserialize(const JsonArchive& ar){
        //Stub
    }
}
