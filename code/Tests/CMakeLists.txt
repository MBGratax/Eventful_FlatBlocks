cmake_minimum_required(VERSION 3.30.5)
set(TEST_FILES
        Test_Version.cpp)
# Create the test executable.
add_executable(tests ${TEST_FILES})

# Find and link with Catch2 and our engine.
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(tests PRIVATE Catch2::Catch2
        PRIVATE Catch2::Catch2WithMain
        PUBLIC Eventful COMPILER_FLAGS)

# Integrate Catch2 with CTest.
include(CTest)
include(Catch)
catch_discover_tests(tests)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_FILES})
