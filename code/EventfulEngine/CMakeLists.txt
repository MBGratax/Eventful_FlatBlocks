set(EVENTFUL_SOURCE_FILES
        Config/Version.cpp
        Source/Runtime/Rendering/Private/EFRenderer.cpp
        Source/Runtime/UI/Private/EFUI.cpp
)
set(EVENTFUL_HEADER_FILES
        Config/Version.h
        Source/Runtime/Rendering/Public/EFRenderer.h
        Source/Runtime/UI/Public/EFUI.h
)

set(EVENTFUL_INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Config>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime/Rendering/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime/UI/Public>
)

# Create the engine static library.
set(LIBRARY Eventful)
add_library(${LIBRARY} SHARED ${EVENTFUL_HEADER_FILES} ${EVENTFUL_SOURCE_FILES})

target_compile_definitions(${LIBRARY}
        PRIVATE
        $ENV{PREPROCESSORS}
)

target_include_directories(${LIBRARY}
        PUBLIC
        ${EVENTFUL_INCLUDE_DIRS}
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set up pch
target_precompile_headers(${LIBRARY} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/pch.h)

add_subdirectory(Modules/EventfulECS)
add_subdirectory(Modules/EventfulPhysics)
add_subdirectory(Source/Runtime/Core)

# Find all packages
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(${LIBRARY}
        PRIVATE
        spdlog::spdlog
        glfw
        EnTT::EnTT
        glad::glad
        imgui::imgui


        PUBLIC
        COMPILER_FLAGS
        EFECS
        EFPhysics
        EFCore
)

if (MSVC)
    set_target_properties(${LIBRARY} PROPERTIES
            COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMPILE_PDB_NAME ${LIBRARY}
    )

    install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${LIBRARY}.pdb"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            OPTIONAL
    )
endif ()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Private" FILES ${EVENTFUL_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Public" FILES ${EVENTFUL_HEADER_FILES})

if (BUILD_SHARED_LIBS)
    target_compile_definitions(${LIBRARY}
            PUBLIC
            EF_SHARED
    )
endif ()

install(
        TARGETS
        ${LIBRARY}
        COMPILER_FLAGS
        EFECS
        EFPhysics
        EFCore
        EXPORT ${LIBRARY}Config
        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        ARCHIVE DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

install(
        EXPORT ${LIBRARY}Config
        NAMESPACE ${LIBRARY}Engine::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY}"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/EventfulEngineConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
)

install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/EventfulEngineConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY}"
)
