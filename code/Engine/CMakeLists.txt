set(EVENTFUL_SOURCE_FILES
        src/Core/Logger.cpp
        src/Core/Version.cpp
)
set(EVENTFUL_HEADER_FILES
        Include/Core/Logger.h
        Include/Core/Version.h
)

# Create the engine static library.
set(LIBRARY Eventful)
add_library(${LIBRARY} ${EVENTFUL_SOURCE_FILES} ${EVENTFUL_HEADER_FILES})

target_compile_definitions(${LIBRARY} PRIVATE $ENV{PREPROCESSORS})

target_include_directories(${LIBRARY}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set up pch
target_precompile_headers(${LIBRARY} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# Find all packages
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)


target_link_libraries(${LIBRARY} PRIVATE spdlog::spdlog
        PRIVATE glfw
        PRIVATE EnTT::EnTT
        PUBLIC COMPILER_FLAGS
)

if(MSVC)
    set_target_properties(${LIBRARY} PROPERTIES
            COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMPILE_PDB_NAME ${LIBRARY}
    )

    install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${LIBRARY}.pdb"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            OPTIONAL
    )
endif()

if (BUILD_SHARED_LIBS)
    target_compile_definitions(${LIBRARY}
            PUBLIC
            EF_SHARED
    )
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${EVENTFUL_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Include" PREFIX "Include" FILES ${EVENTFUL_HEADER_FILES})

install(
        TARGETS ${LIBRARY} COMPILER_FLAGS
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
