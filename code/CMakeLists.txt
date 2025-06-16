cmake_minimum_required(VERSION 3.30.5)
include(FetchContent)
include(CMakeDependentOption)
include(GenerateExportHeader)

# Define the project and semantic versioning.
project(Eventful_FlatBlocks VERSION 0.1.0 LANGUAGES CXX)

# stuff to help debug cmake
message(STATUS "cmake source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMake C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake host system processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

option(DEBUG_CMAKELIST "Receive CMake Debug messages" OFF)

if (DEBUG_CMAKELIST)
    set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (PROJECT_IS_TOP_LEVEL)
    option(EVENTFUL_BUILD_UNIT_TESTS "Build the Eventful_FlatBlocks unit tests" OFF)
    option(EVENTFUL_BUILD_DOCS "Build the Eventful_FlatBlocks documentation" ON)

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
        "$<${MSVC_CXX}:$<BUILD_INTERFACE:-W3;-utf-8>>"
)

# Enable Edit&Continue for MSVC projects
if (MSVC)
    target_compile_options(COMPILER_FLAGS INTERFACE "/ZI")
    target_link_options(COMPILER_FLAGS INTERFACE "/INCREMENTAL")
endif ()

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif (UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif ()

# Make sure the generated header is available to all targets.
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# -------------------------------------------------------------------------
# Fallback for vcpkg in case user forgot to use presets.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif ()

# --- Embed Git tag/commit into semantic version ---
find_package(Git QUIET)
if (Git_FOUND)
    execute_process(
            COMMAND "${GIT_EXECUTABLE}" describe --tags --always --dirty
            WORKING_DIRECTORY "${CMAKE_BIN_DIR}"
            OUTPUT_VARIABLE GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(PROJECT_VERSION "${PROJECT_VERSION}_${GIT_DESCRIBE}")
endif ()
message(STATUS "Project version: ${PROJECT_VERSION}")

# Generate configuration header for versioning.
# TODO: Move to custom build command that runs on build
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
# -------------------------------------------------------------------------

# Add subdirectories for our modules.
# 1) Find all module defs
file(GLOB_RECURSE EFMODDEF_PATHS
        CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/EventfulEngine/*.efmoddef"
        "${CMAKE_SOURCE_DIR}/Game/*.efmoddef")

set(EFMODDEF_DIRS "")

foreach (EFMODDEF_FILE IN LISTS EFMODDEF_PATHS)
    get_filename_component(DIR ${EFMODDEF_FILE} DIRECTORY)
    list(APPEND EFMODDEF_DIRS ${DIR})
endforeach ()
message(DEBUG "ModDefs: ${EFMODDEF_DIRS}")

set(ALL_MODULE_TARGETS "")

# 2) First Pass:
foreach (MODULE_DEFINITION IN LISTS EFMODDEF_PATHS)
    # Module directory
    message(STATUS "New Target: ${MODULE_DEFINITION}")
    cmake_path(GET MODULE_DEFINITION PARENT_PATH MOD_DIR)

    # TODO: Add custom command to only reconfigure a single module
    # 2.1) Read JSON properties; TODO: Handle empty strings better, clearer parsing
    file(READ ${MODULE_DEFINITION} MOD_JSON)

    string(JSON MODULE_NAME GET "${MOD_JSON}" "Module Name")
    string(JSON MODULE_VERSION GET "${MOD_JSON}" "Module Version")
    string(JSON TARGET_TYPE GET "${MOD_JSON}" "Target Type")
    string(JSON INCLUDE_PLATFORMS GET "${MOD_JSON}" "Include Platform")
    string(JSON EXCLUDE_PLATFORMS GET "${MOD_JSON}" "Exclude Platform")
    string(JSON MODULE_DEFINES GET "${MOD_JSON}" Defines)
    string(JSON MODULE_DEPS GET "${MOD_JSON}" Dependencies)

    message(CHECK_START "${MODULE_NAME}: Pass 1 Start")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # 2.2) Platform filter; TODO: Define own platform Targets to check against
    message(CHECK_START "Checking if: ${MODULE_NAME} should compile...")
    if (NOT ${INCLUDE_PLATFORMS} STREQUAL "[]")
        list(FIND INCLUDE_PLATFORMS ${CMAKE_SYSTEM_NAME} _inc)
        if (_inc EQUAL -1)
            message(CHECK_FAIL "${MODULE_NAME} will not be compiled because it's include platforms do not contain ${CMAKE_SYSTEM_NAME}.")
            continue()
        endif ()
    endif ()
    if (NOT ${EXCLUDE_PLATFORMS} STREQUAL "[]")
        list(FIND EXCLUDE_PLATFORMS ${CMAKE_SYSTEM_NAME} _exc)
        if (NOT _exc EQUAL -1)
            message(CHECK_FAIL "${MODULE_NAME} will not be compiled because it's exclude platforms contain ${CMAKE_SYSTEM_NAME}.")
            continue()
        endif ()
    endif ()

    message(CHECK_PASS "Pass!")

    message(CHECK_START "${MODULE_NAME}: Finding Sources...")

    # 2.3) Gather source and header files
    file(GLOB_RECURSE ${MODULE_NAME}_SOURCE CONFIGURE_DEPENDS
            ${MOD_DIR}/*.cpp
    )
    file(GLOB_RECURSE ${MODULE_NAME}_HEADER CONFIGURE_DEPENDS
            ${MOD_DIR}/*.h
    )

    list(LENGTH ${MODULE_NAME}_SOURCE SOURCE_NUMBER)
    list(LENGTH ${MODULE_NAME}_HEADER HEADER_NUMBER)
    message(CHECK_PASS "${MODULE_NAME}: Found ${SOURCE_NUMBER} source- and ${HEADER_NUMBER} header files")

    message(DEBUG "${MODULE_NAME} Sources:")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_SOURCE)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()

    message(DEBUG "${MODULE_NAME} Headers: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_HEADER)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()

    # 2.4) Grab all includes we gathered and filter out any that are specifically in a /Private* folder
    message(CHECK_START "${MODULE_NAME}: Finding include directories...")
    set(${MODULE_NAME}_SUBDIRS ${${MODULE_NAME}_HEADER})
    list(REMOVE_DUPLICATES ${MODULE_NAME}_SUBDIRS)
    set(${MODULE_NAME}_PUBLIC_DIRS "")
    foreach (DIR IN LISTS ${MODULE_NAME}_SUBDIRS)
        get_filename_component(CURRENT_DIR ${DIR} DIRECTORY)
        if (NOT CURRENT_DIR MATCHES "/(^|[\\\/])Private([\\\/]|$)/i")
            list(APPEND ${MODULE_NAME}_PUBLIC_DIRS "${CURRENT_DIR}")
        endif ()
    endforeach ()

    message(DEBUG "${MODULE_NAME} Include Directories: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_PUBLIC_DIRS)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()

    list(LENGTH ${MODULE_NAME}_PUBLIC_DIRS INCLUDEDIR_NUMBER)
    message(CHECK_PASS "${MODULE_NAME}: Found ${INCLUDEDIR_NUMBER} include directories")
    # 2.5) Filter out source files that belong to submodules
    message(CHECK_START "${MODULE_NAME}: Filtering Source files and include directories...")

    set(REMOVE_SRC "")
    set(REMOVE_INC "")
    set(REMOVE_PUB "")
    foreach (SRC INC PUB IN ZIP_LISTS ${MODULE_NAME}_SOURCE ${MODULE_NAME}_HEADER ${MODULE_NAME}_PUBLIC_DIRS)
        set(SKIP_SRC FALSE)
        set(SKIP_INC FALSE)
        set(SKIP_PUB FALSE)
        foreach (OTHER_DIR IN LISTS EFMODDEF_DIRS)
            if (NOT OTHER_DIR STREQUAL MOD_DIR)

                cmake_path(IS_PREFIX OTHER_DIR "${MOD_DIR}" SKIP)
                if (SKIP)
                    message(DEBUG "Skipping filter step on ${OTHER_DIR} because it is a parent to ${MOD_DIR}")
                    break()
                endif ()

                if (NOT "${SRC}" STREQUAL "")
                    cmake_path(IS_PREFIX OTHER_DIR "${SRC}" IS_SUBSOURCE)
                endif ()

                if (NOT "${INC}" STREQUAL "")
                    cmake_path(IS_PREFIX OTHER_DIR "${INC}" IS_SUBINCLUDE)
                endif ()

                if (NOT "${PUB}" STREQUAL "")
                    cmake_path(IS_PREFIX OTHER_DIR "${PUB}" IS_SUBPUBDIR)
                endif ()

                if (IS_SUBSOURCE)
                    message(DEBUG "Filtering source ${SRC} because it contains ${OTHER_DIR}/")
                    set(SKIP_SRC TRUE)
                endif ()
                if (IS_SUBINCLUDE)
                    message(DEBUG "Filtering header ${INC} because it contains ${OTHER_DIR}/")
                    set(SKIP_INC TRUE)
                endif ()
                if (IS_SUBPUBDIR)
                    message(DEBUG "Filtering include dir ${PUB} because it contains ${OTHER_DIR}/")
                    set(SKIP_PUB TRUE)
                endif ()
            endif ()
            if (SKIP_SRC OR SKIP_INC OR SKIP_PUB)
                cmake_path(GET OTHER_DIR RELATIVE_PART SKIPPED_SUBPATH)
                list(APPEND ${MODULE_NAME}_SUBMODULES ${SKIPPED_SUBPATH})
            endif ()
            if (SKIP_SRC)
                list(APPEND REMOVE_SRC "${SRC}")
                set(SKIP_SRC FALSE)
            endif ()
            if (SKIP_INC)
                list(APPEND REMOVE_INC "${INC}")
                set(SKIP_INC FALSE)
            endif ()
            if (SKIP_PUB)
                list(APPEND REMOVE_PUB "${PUB}")
                set(SKIP_PUB FALSE)
            endif ()
        endforeach ()
    endforeach ()

    list(LENGTH ${MODULE_NAME}_SUBMODULES NUM_SUBMODULES)
    message(VERBOSE "${MODULE_NAME}: Filtered ${NUM_SUBMODULES} Subdirectories")

    foreach (SRC INC PUB IN ZIP_LISTS REMOVE_SRC REMOVE_INC REMOVE_PUB)
        if (NOT SRC STREQUAL "")
            list(REMOVE_ITEM ${MODULE_NAME}_SOURCE "${SRC}")
        endif ()
        if (NOT INC STREQUAL "")
            list(REMOVE_ITEM ${MODULE_NAME}_HEADER "${INC}")
        endif ()
        if (NOT PUB STREQUAL "")
            list(REMOVE_ITEM ${MODULE_NAME}_PUBLIC_DIRS "${PUB}")
        endif ()
    endforeach ()

    message(CHECK_PASS "${MODULE_NAME}: Filtering Successful")

    message(DEBUG "${MODULE_NAME}: Filtered Sources: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_SOURCE)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()
    message(DEBUG "${MODULE_NAME}: Filtered Headers: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_HEADER)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()
    message(DEBUG "${MODULE_NAME}: Filtered Include Directories: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_PUBLIC_DIRS)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()

    message(CHECK_START "${MODULE_NAME}: Configuring public includes...")

    set(${MODULE_NAME}_PUBLIC_INCLUDES "")
    foreach (PUB IN LISTS ${MODULE_NAME}_PUBLIC_DIRS)
        list(APPEND ${MODULE_NAME}_PUBLIC_INCLUDES
                $<BUILD_INTERFACE:${PUB}>)
    endforeach ()

    message(DEBUG "${MODULE_NAME}: Public Includes: ")
    if (DEBUG_CMAKELIST)
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        foreach (ITEM IN LISTS ${MODULE_NAME}_PUBLIC_INCLUDES)
            message(DEBUG "${ITEM}")
        endforeach ()
        list(POP_BACK CMAKE_MESSAGE_INDENT)
    endif ()

    message(CHECK_PASS "${MODULE_NAME}: Public Includes configured")

    # 2.6) Create CMake target
    set(${MODULE_NAME}_TARGET ${MODULE_NAME})
    message(CHECK_START "${MODULE_NAME}: Creating Target \"${${MODULE_NAME}_TARGET}\" of type ${TARGET_TYPE}")
    if (TARGET_TYPE STREQUAL "STATIC_LIBRARY")
        add_library(${${MODULE_NAME}_TARGET} STATIC ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
    elseif (TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        add_library(${${MODULE_NAME}_TARGET} SHARED ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
        string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)
        generate_export_header(${${MODULE_NAME}_TARGET}
                BASE_NAME ${MODULE_NAME}ModuleAPI
                EXPORT_MACRO_NAME ${MODULE_NAME_UPPER}_API
                EXPORT_FILE_NAME Exports/${MODULE_NAME}ModuleAPI.h
                INCLUDE_GUARD_NAME ${MODULE_NAME_UPPER}_MODULE_API
        )
        install(TARGETS ${${MODULE_NAME}_TARGET} LIBRARY DESTINATION Modules PUBLIC_HEADER DESTINATION include)
        install(FILES "${CMAKE_BINARY_DIR}/Exports/${MODULE_NAME}ModuleAPI.h" DESTINATION include)
    elseif (TARGET_TYPE STREQUAL "EXECUTABLE")
        add_executable(${${MODULE_NAME}_TARGET} ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
        install(TARGETS ${${MODULE_NAME}_TARGET} RUNTIME DESTINATION bin)
    else ()
        message(CHECK_FAIL "Unknown TargetType '${TARGET_TYPE}' in ${MOD_DEF}")
    endif ()
    list(APPEND ${MODULE_NAME}_PUBLIC_INCLUDES
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Exports>
    )
    set_target_properties(${${MODULE_NAME}_TARGET} PROPERTIES
            POSITION_INDEPENDENT_CODE BUILD_SHARED_LIBS
    )
    message(CHECK_PASS "${MODULE_NAME}: Target successfully created!")

    # 2.7) Add compile definitions - project wide and from module definition
    message(CHECK_START "${MODULE_NAME}: Parsing Defines...")
    set(${${MODULE_NAME}_TARGET}_MODULE_DEFINES "")
    string(JSON DEF_COUNT LENGTH "${MODULE_DEFINES}")
    if (DEF_COUNT)
        math(EXPR last "${DEF_COUNT}-1")
        foreach (i RANGE 0 ${last})
            # retrieve each object’s fields
            string(JSON DEFINE GET "${MODULE_DEFINES}" ${i})

            list(APPEND ${${MODULE_NAME}_TARGET}_MODULE_DEFINES "${LIB_NAME}")
        endforeach ()
        message(CHECK_PASS "Success")
    else ()
        message(CHECK_PASS "Success - No Additional Defines")
    endif ()

    target_compile_definitions(${${MODULE_NAME}_TARGET}
            PRIVATE
            ${${${MODULE_NAME}_TARGET}_MODULE_DEFINES}
            ${PREPROCESSORS})

    get_target_property(DEFINES ${${MODULE_NAME}_TARGET} COMPILE_DEFINITIONS)
    message(DEBUG "${${MODULE_NAME}_TARGET}: Defines: ${DEFINES}")

    # 2.8) Cache current target into list
    list(APPEND ALL_MODULE_TARGETS ${${MODULE_NAME}_TARGET})

    # 2.9) Parse Dependencies
    message(CHECK_START "${MODULE_NAME}: Parsing Dependencies...")
    set(${${MODULE_NAME}_TARGET}_PUBLIC_DEPENDENCIES "")
    set(${${MODULE_NAME}_TARGET}_PRIVATE_DEPENDENCIES "")

    string(JSON DEP_COUNT LENGTH "${MODULE_DEPS}")
    if (DEP_COUNT)
        math(EXPR last "${DEP_COUNT}-1")
        foreach (i RANGE 0 ${last})
            # retrieve each object’s fields
            string(JSON DEP_TYPE GET "${MODULE_DEPS}" ${i} "Dependency Type")
            string(JSON LIB_NAME GET "${MODULE_DEPS}" ${i} "Library Name")

            string(FIND ${LIB_NAME} "INCLUDE" IS_INCLUDE_DEPENDENCY)

            if ("${DEP_TYPE}" STREQUAL "PUBLIC")
                if (NOT IS_INCLUDE_DEPENDENCY EQUAL -1)
                    target_include_directories(${${MODULE_NAME}_TARGET} PUBLIC ${${LIB_NAME}})
                    continue()
                endif ()
                if (${${MODULE_NAME}_TARGET}_PUBLIC_DEPENDENCIES STREQUAL "")
                    list(PREPEND ${${MODULE_NAME}_TARGET}_PUBLIC_DEPENDENCIES "${LIB_NAME}")
                    continue()
                endif ()
                list(APPEND ${${MODULE_NAME}_TARGET}_PUBLIC_DEPENDENCIES "${LIB_NAME}")
            elseif ("${DEP_TYPE}" STREQUAL "PRIVATE")
                if (NOT IS_INCLUDE_DEPENDENCY EQUAL -1)
                    target_include_directories(${${MODULE_NAME}_TARGET} PRIVATE ${${LIB_NAME}})
                    continue()
                endif ()
                if (${${MODULE_NAME}_TARGET}_PRIVATE_DEPENDENCIES STREQUAL "")
                    list(PREPEND ${${MODULE_NAME}_TARGET}_PRIVATE_DEPENDENCIES "${LIB_NAME}")
                    continue()
                endif ()
                list(APPEND ${${MODULE_NAME}_TARGET}_PRIVATE_DEPENDENCIES "${LIB_NAME}")
            else ()
                message(CHECK_FAIL "${MODULE_NAME}: Invalid dependency type ${DEP_TYPE}!")
            endif ()
        endforeach ()

    else ()
        message(STATUS "${MODULE_NAME}: No Dependencies")
    endif ()

    message(CHECK_PASS "${MODULE_NAME}: Dependencies gathered!")
    if (NOT TARGET ${MODULE_NAME})
        message(CHECK_FAIL "🔥 ${MODULE_NAME} target not found! Something went wrong!")
    endif ()
    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "${MODULE_NAME}: Pass 1 End, waiting for other targets")
    # 2.10) Store dependencies and include directories for linking step
    set(${${MODULE_NAME}_TARGET}_INCLUDE_DIRS "${${MODULE_NAME}_PUBLIC_INCLUDES}")
    set(${${MODULE_NAME}_TARGET}_INSTALL_DIR "${MOD_DIR}")
endforeach ()

# Find packages; TODO: Find a way to automate this and make it less error prone
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

# 3) Second Pass:
foreach (TARGET IN LISTS ALL_MODULE_TARGETS)
    message(CHECK_START "${TARGET}: Pass 2 start")

    message(CHECK_START "${TARGET}: Linking libraries")
    # 3.1) Link compiler definitions and dependencies

    if (NOT TARGET ${MODULE_NAME})
        message(CHECK_FAIL "🔥 ${MODULE_NAME} target not found!")
    endif ()

    foreach (PUB_DEP PRIV_DEP IN ZIP_LISTS ${TARGET}_PUBLIC_DEPENDENCIES ${TARGET}_PRIVATE_DEPENDENCIES)
        if (NOT TARGET ${PUB_DEP})
            message(WARNING "🔥 ${PUB_DEP} target not found!")
        endif ()
        if (NOT TARGET ${PRIV_DEP})
            message(WARNING "🔥 ${PRIV_DEP} target not found!")
        endif ()
    endforeach ()
    message(VERBOSE "${TARGET}: target_link_libraries(${TARGET}\nPUBLIC\nCOMPILER_FLAGS\n${${TARGET}_PUBLIC_DEPENDENCIES}\nPRIVATE\n${${TARGET}_PRIVATE_DEPENDENCIES})")
    target_link_libraries(${TARGET}
            PUBLIC
            COMPILER_FLAGS
            ${${TARGET}_PUBLIC_DEPENDENCIES}
            PRIVATE
            ${${TARGET}_PRIVATE_DEPENDENCIES}
    )

    if ("${TARGET}" STREQUAL "EFEntryPoint")
        target_link_options("${TARGET}" PUBLIC "/SUBSYSTEM:WINDOWS")
    endif ()

    get_target_property(LINK_LIBS ${TARGET} LINK_LIBRARIES)
    message(DEBUG "${TARGET}: LINK_LIBRARIES: ${LINK_LIBS}")
    message(CHECK_PASS "${TARGET}: Success")
    # 3.2) Set Include directories
    message(CHECK_START "${TARGET}: Set Include and install Directories")
    message(VERBOSE "${TARGET}: target_include_directories(${TARGET}\nPUBLIC\n${${TARGET}_INCLUDE_DIRS}\n$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>\nPRIVATE\n$<BUILD_INTERFACE:${${TARGET}_INSTALL_DIR}>\n)")
    target_include_directories(${TARGET}
            PUBLIC
            ${${TARGET}_INCLUDE_DIRS}
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            PRIVATE
            $<BUILD_INTERFACE:${${TARGET}_INSTALL_DIR}>
    )
    get_target_property(INCLUDES ${TARGET} INCLUDE_DIRECTORIES)
    message(DEBUG "${TARGET}: INCLUDE_DIRS: ${INCLUDES}")
    message(CHECK_PASS "${TARGET}: Success")
    message(CHECK_PASS "${TARGET}: Pass 2 end")
endforeach ()

# (Optional) Enable testing if a tests/ directory is added later.
if (EVENTFUL_BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif ()


# Configure packaging (CPack).
include(CPack)

# -------------------------------------------------------------------------
# Documentation generation using Doxygen.
find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Config/Doxyfile.in")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_SOURCE_DIR}/../documentation/Doxyfile")
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generate API documentation with Doxygen"
            VERBATIM)
endif ()

install(FILES "${PROJECT_BINARY_DIR}/Config.h"
        DESTINATION include
)
