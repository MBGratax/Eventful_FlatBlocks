#pragma once

#include "../../Public/EventfulObject/EFObject.h"

namespace EventfulEngine{
    bool __ReflectedClass_EFObject();
    EFREGISTER(EFObject)
        EFPROPERTY(_bIsValid, E_PropertyFlags::None, {"Category", {"Validation"}})
        EFMETHOD(IsValid, E_MethodFlags::None, {"Category", {"Validation"}})
    EFREGISTER_END(EFObject)
}
