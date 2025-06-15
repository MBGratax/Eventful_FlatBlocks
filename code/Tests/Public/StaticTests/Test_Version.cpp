#pragma once

#include <catch.hpp>

#include "Version.h"

TEST_CASE("Engine version is correct", "[EventfulEngine]"){
    // The expected version should match the one in config.h (set via CMake).
    REQUIRE(EventfulEngine::GetVersion() == "@PROJECT_VERSION@");
}

