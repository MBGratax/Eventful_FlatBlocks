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

set(DEBUG_CMAKELIST ON)

if (DEBUG_CMAKELIST)
    set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if (PROJECT_IS_TOP_LEVEL)
    option(EVENTFUL_BUILD_UNIT_TESTS "Build the Eventful_FlatBlocks unit tests" ON)
    option(EVENTFUL_BUILD_DOCS "Build the Eventful_FlatBlocks documentation" ON)
    option(EVENTFUL_USER_SETTINGS "Override EVENTFUL settings with EFUserSettings.h" OFF)

    option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
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

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif (UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif ()

# Make sure the generated header is available to all targets.
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# -------------------------------------------------------------------------
# vcpkg integration: if no toolchain is already specified, use our submodule.
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
    string(APPEND PROJECT_VERSION "_" "${GIT_DESCRIBE}")
endif ()
message(STATUS "Project version: ${PROJECT_VERSION}")

# Generate configuration header for versioning.
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

    # 2.1) Read JSON properties; TODO: Handle empty strings better, clearer parsing
    file(READ ${MODULE_DEFINITION} MOD_JSON)

    string(JSON MODULE_NAME GET "${MOD_JSON}" "Module Name")
    string(JSON MODULE_VERSION GET "${MOD_JSON}" "Module Version")
    string(JSON TARGET_TYPE GET "${MOD_JSON}" "Target Type")
    string(JSON INCLUDE_PLATFORMS GET "${MOD_JSON}" "Include Platform")
    string(JSON EXCLUDE_PLATFORMS GET "${MOD_JSON}" "Exclude Platform")
    string(JSON MODULE_DEFINES GET "${MOD_JSON}" Defines)
    string(JSON MODULE_DEPS GET "${MOD_JSON}" Dependencies)

    message(STATUS "${MODULE_NAME}: Pass 1 Start")

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

    message(STATUS "${MODULE_NAME}: Finding Sources...")

    # 2.3) Gather source and header files
    file(GLOB_RECURSE ${MODULE_NAME}_SOURCE CONFIGURE_DEPENDS
            ${MOD_DIR}/*.cpp
    )
    file(GLOB_RECURSE ${MODULE_NAME}_HEADER CONFIGURE_DEPENDS
            ${MOD_DIR}/*.h
    )

    list(LENGTH ${MODULE_NAME}_SOURCE SOURCE_NUMBER)
    list(LENGTH ${MODULE_NAME}_HEADER HEADER_NUMBER)
    message(STATUS "${MODULE_NAME}: Found ${SOURCE_NUMBER} source- and ${HEADER_NUMBER} header files")

    message(DEBUG "${MODULE_NAME} Sources: ${${MODULE_NAME}_SOURCE}")
    message(DEBUG "${MODULE_NAME} Headers: ${${MODULE_NAME}_HEADER}")
    # 2.4) Gather Public include folders: any “*/Public” in this tree

    message(STATUS "${MODULE_NAME}: Finding include directories...")
    file(GLOB_RECURSE ${MODULE_NAME}_SUBDIRS LIST_DIRECTORIES TRUE CONFIGURE_DEPENDS
            ${MOD_DIR}/*/Public/
            ${MOD_DIR}/Public
    )
    set(${MODULE_NAME}_PUBLIC_DIRS "")
    foreach (DIR IN LISTS ${MODULE_NAME}_SUBDIRS)
        if (DIR MATCHES "/Public$")
            list(APPEND ${MODULE_NAME}_PUBLIC_DIRS "${DIR}")
        endif ()
    endforeach ()

    # 2.5) Filter out source files that belong to submodules
    message(STATUS "${MODULE_NAME}: Filtering Source files and include directories...")

    set(REMOVE_SRC "")
    set(REMOVE_INC "")
    set(REMOVE_PUB "")
    foreach (SRC INC PUB IN ZIP_LISTS ${MODULE_NAME}_SOURCE ${MODULE_NAME}_HEADER ${MODULE_NAME}_PUBLIC_DIRS)
        set(SKIP_SRC FALSE)
        set(SKIP_INC FALSE)
        set(SKIP_PUB FALSE)
        foreach (OTHER_DIR IN LISTS EFMODDEF_DIRS)
            if (NOT OTHER_DIR STREQUAL MOD_DIR)
                string(FIND "${SRC}" "${OTHER_DIR}/" IS_SUBSOURCE)
                string(FIND "${INC}" "${OTHER_DIR}/" IS_SUBINCLUDE)
                string(FIND "${PUB}" "${OTHER_DIR}/Public" IS_SUBPUBDIR)
                string(FIND "${PUB}" "${OTHER_DIR}/*/Public" IS_SUBPUBDIR)
                if (NOT IS_SUBSOURCE EQUAL -1)
                    message(DEBUG "Skipping ${SRC} because it contains ${OTHER_DIR}/")
                    set(SKIP_SRC TRUE)
                endif ()
                if (NOT IS_SUBINCLUDE EQUAL -1)
                    message(DEBUG "Skipping ${INC} because it contains ${OTHER_DIR}/")
                    set(SKIP_INC TRUE)
                endif ()
                if (NOT IS_SUBPUBDIR EQUAL -1)
                    message(DEBUG "Skipping ${PUB} because it contains ${OTHER_DIR}/")
                    set(SKIP_PUB TRUE)
                endif ()
            endif ()
            if (SKIP_SRC OR SKIP_INC OR SKIP_PUB)
                cmake_path(RELATIVE_PART OTHER_DIR MOD_DIR SKIPPED_SUBPATH)
                list(APPEND SUBMODULES ${SKIPPED_SUBPATH})
                break()
            endif ()
        endforeach ()
        if (SKIP_SRC)
            list(APPEND REMOVE_SRC "${SRC}")
        endif ()
        if (SKIP_INC)
            list(APPEND REMOVE_INC "${INC}")
        endif ()
        if (SKIP_PUB)
            list(APPEND REMOVE_PUB "${PUB}")
        endif ()
    endforeach ()

    list(LENGTH SUBMODULES NUM_SUBMODULES)
    message(STATUS "${MODULE_NAME}: Filtered ${NUM_SUBMODULES} Subdirectories")

    foreach (SRC INC PUB IN ZIP_LISTS REMOVE_SRC REMOVE_INC REMOVE_PUB)
        list(REMOVE_ITEM ${MODULE_NAME}_SOURCE "${SRC}")
        list(REMOVE_ITEM ${MODULE_NAME}_HEADER "${INC}")
        list(REMOVE_ITEM ${MODULE_NAME}_PUBLIC_DIRS "${PUB}")
    endforeach ()

    message(STATUS "${MODULE_NAME}: Filtering Successful")

    set(${MODULE_NAME}_PUBLIC_INCLUDES "")
    foreach (PUB IN LISTS ${MODULE_NAME}_PUBLIC_DIRS)
        list(APPEND ${MODULE_NAME}_PUBLIC_INCLUDES
                $<BUILD_INTERFACE:${PUB}>)
    endforeach ()

    # 2.6) Create CMake target
    set(CURRENT_TARGET ${MODULE_NAME})
    message(STATUS "${MODULE_NAME}: Creating Target \"${CURRENT_TARGET}\" of type ${TARGET_TYPE}")
    if (TARGET_TYPE STREQUAL "STATIC_LIBRARY")
        add_library(${CURRENT_TARGET} STATIC ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
    elseif (TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        add_library(${CURRENT_TARGET} SHARED ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
        generate_export_header(${CURRENT_TARGET})
    elseif (TARGET_TYPE STREQUAL "EXECUTABLE")
        add_executable(${CURRENT_TARGET} ${${MODULE_NAME}_HEADER} ${${MODULE_NAME}_SOURCE})
    else ()
        message(FATAL_ERROR "Unknown TargetType '${TARGET_TYPE}' in ${MOD_DEF}")
    endif ()

    set_target_properties(${CURRENT_TARGET} PROPERTIES
            POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
    )

    # 2.7) Add compile definitions - project wide and from module definition
    target_compile_definitions(${CURRENT_TARGET}
            PRIVATE
            ${MODULE_DEFINES}
            $ENV{PREPROCESSORS})

    # 2.8) Cache current target into list
    list(APPEND ALL_MODULE_TARGETS ${CURRENT_TARGET})

    # 2.9) Parse Dependencies
    message(STATUS "${MODULE_NAME}: Parsing Dependencies...")
    set(DEPENDENCIES "")
    set(${CURRENT_TARGET}_PUBLIC_DEPENDENCIES "")
    set(${CURRENT_TARGET}_PRIVATE_DEPENDENCIES "")

    string(JSON DEP_COUNT LENGTH "${MODULE_DEPS}")
    if (DEP_COUNT)
        math(EXPR last "${DEP_COUNT}-1")
        foreach (i RANGE 0 ${last})
            # retrieve each object’s fields
            string(JSON DEP_TYPE GET "${MODULE_DEPS}" ${i} "Dependency Type")
            string(JSON LIB_NAME GET "${MODULE_DEPS}" ${i} "Library Name")

            if ("${DEP_TYPE}" STREQUAL "PUBLIC")
                if (${CURRENT_TARGET}_PUBLIC_DEPENDENCIES STREQUAL "")
                    string(PREPEND ${CURRENT_TARGET}_PUBLIC_DEPENDENCIES "${LIB_NAME}")
                    continue()
                endif ()
                string(JOIN "\n" ${CURRENT_TARGET}_PUBLIC_DEPENDENCIES "${${CURRENT_TARGET}_PUBLIC_DEPENDENCIES}" "${LIB_NAME}")
            elseif ("${DEP_TYPE}" STREQUAL "PRIVATE")
                if (${CURRENT_TARGET}_PRIVATE_DEPENDENCIES STREQUAL "")
                    string(PREPEND ${CURRENT_TARGET}_PRIVATE_DEPENDENCIES "${LIB_NAME}")
                    continue()
                endif ()
                string(JOIN "\n" ${CURRENT_TARGET}_PRIVATE_DEPENDENCIES "${${CURRENT_TARGET}_PRIVATE_DEPENDENCIES}" "${LIB_NAME}")
            else ()
                message(FATAL_ERROR "${MODULE_NAME}: Invalid dependency type ${DEP_TYPE}!")
            endif ()
        endforeach ()

    else ()
        message(STATUS "${MODULE_NAME}: No Dependencies")
    endif ()

    message(STATUS "${MODULE_NAME}: Dependencies gathered!")
    message(STATUS "${MODULE_NAME}: Pass 1 End, waiting for other targets")
    # 2.10) Store dependencies and include directories for linking step
    set(${CURRENT_TARGET}_INCLUDE_DIRS "${${MODULE_NAME}_PUBLIC_INCLUDES}")
    set(${CURRENT_TARGET}_INSTALL_DIR "${MOD_DIR}")
endforeach ()

# Find packages; TODO: Find a way to automate this and make it less error prone
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)

set(BULLET_TARGETS
        BulletDynamics
        BulletCollision
        Bullet3Common
)

# 3) Second Pass:
foreach (TARGET IN LISTS ALL_MODULE_TARGETS)
    message(STATUS "${TARGET}: Pass 2 start")

    message(STATUS "${TARGET}: Linking libraries")
    # 3.1) Link compiler definitions and dependencies

    message(STATUS "${TARGET}: target_link_libraries(${TARGET}\nPUBLIC\nCOMPILER_FLAGS\n${${TARGET}_PUBLIC_DEPENDENCIES}\nPRIVATE\n${${TARGET}_PRIVATE_DEPENDENCIES})")
    target_link_libraries(${TARGET}
            PUBLIC
            COMPILER_FLAGS
            ${TARGET}_PUBLIC_DEPENDENCIES
            PRIVATE
            ${TARGET}_PRIVATE_DEPENDENCIES
    )

    # 3.2) Set Include directories
    message(STATUS "${TARGET}: Set Include and install Directories")
    message(STATUS "${TARGET}: target_include_directories(${TARGET}\nPUBLIC\n${${TARGET}_INCLUDE_DIRS}\n$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>\nPRIVATE\n$<BUILD_INTERFACE:${${TARGET}_INSTALL_DIR}>\n)")
    target_include_directories(${TARGET}
            PUBLIC
            ${${TARGET}_INCLUDE_DIRS}
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            PRIVATE
            $<BUILD_INTERFACE:${${TARGET}_INSTALL_DIR}>
    )
    message(STATUS "${TARGET}: Pass 2 end")
endforeach ()

#add_subdirectory(EventfulEngine)
#add_subdirectory(Game)

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
    # The Doxyfile.in template is in the docs folder (sibling to code/).
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/../documentation/Doxyfile.in")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_SOURCE_DIR}/../documentation/Doxyfile")
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
