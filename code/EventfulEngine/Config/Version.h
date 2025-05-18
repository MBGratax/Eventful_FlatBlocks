#pragma once
#include <string>

namespace EventfulEngine {
    /**
     * @brief Returns the current version of the project.
     *
     * The version string is defined in the configuration header generated at build time.
     *
     * @return std::string containing the semantic version.
     */
    std::string GetVersion();
}
