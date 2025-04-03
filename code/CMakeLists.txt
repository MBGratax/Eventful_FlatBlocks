cmake_minimum_required(VERSION 3.30.5)
include(FetchContent)
include(CMakeDependentOption)

# Define the project and semantic versioning.
project(Eventful_FlatBlocks VERSION 0.1.0 LANGUAGES CXX)

# stuff to help debug cmake
message(STATUS "cmake source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "library postfix: ${CMAKE_DEBUG_POSTFIX}")
message(STATUS "CMake C compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMake C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if (PROJECT_IS_TOP_LEVEL)
    option(EVENTFUL_BUILD_UNIT_TESTS "Build the Eventful_FlatBlocks unit tests" ON)
    option(EVENTFUL_BUILD_DOCS "Build the Eventful_FlatBlocks documentation" OFF)
    option(EVENTFUL_USER_SETTINGS "Override EVENTFUL settings with EFUserSettings.h" OFF)

    option(EVENTFUL_SHARED_LIBS "Build EVENTFUL as a shared library" OFF)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

add_library(COMPILER_FLAGS INTERFACE)
target_compile_features(COMPILER_FLAGS INTERFACE cxx_std_20)


# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(GCC_LIKE_CXX "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(MSVC_CXX "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(COMPILER_FLAGS INTERFACE
        "$<${GCC_LIKE_CXX}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${MSVC_CXX}:$<BUILD_INTERFACE:-W3>>"
)

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif (UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif ()

# Generate configuration header for versioning.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
# Make sure the generated header is available to all targets.
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# -------------------------------------------------------------------------
# vcpkg integration: if no toolchain is already specified, use our submodule.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif ()

# -------------------------------------------------------------------------
# Add subdirectories for our modules.
add_subdirectory(Engine)
add_subdirectory(Game)

# (Optional) Enable testing if a tests/ directory is added later.
if (EVENTFUL_BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif ()

# -------------------------------------------------------------------------
# Documentation generation using Doxygen.
find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    # The Doxyfile.in template is in the docs folder (sibling to code/).
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/../documentation/Doxyfile.in")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif ()

# add the install targets
install(TARGETS ${EXECUTABLE} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h"
        DESTINATION include
)
