#include "Include/Core/Version.h"
#include "config.h"

namespace EventfulEngine {

    std::string GetVersion() {
        // PROJECT_VERSION is defined in the generated config.h
        return PROJECT_VERSION;
    }

}
