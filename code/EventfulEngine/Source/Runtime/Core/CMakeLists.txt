set(CORE_SOURCE_FILES
        Debug/Private/EFLogger.cpp
        EFObject/Private/EFObject.cpp
)
set(CORE_HEADER_FILES
        Debug/Public/EFLogger.h
        EFObject/Public/EFObject.h
        EFEvents.h
)

set(CORE_INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Debug/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EFObject/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Create the engine static library.
set(CORE_LIB EFCore)
add_library(${CORE_LIB} SHARED ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})

target_compile_definitions(${CORE_LIB}
        PRIVATE
        $ENV{PREPROCESSORS}
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(${CORE_LIB}
        PRIVATE spdlog::spdlog)

target_include_directories(${CORE_LIB}
        PUBLIC
        ${CORE_INCLUDE_DIRS}
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
