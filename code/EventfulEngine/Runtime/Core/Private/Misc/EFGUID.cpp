#pragma once

#include "EFGUID.h"

namespace EventfulEngine{
    EFGUID EFGUID::NewGuid(){
        EFGUID guid(0, 0, 0, 0);
        // Do platformspecific gui stuff


        return guid;
    }
} // EventfulEngine
