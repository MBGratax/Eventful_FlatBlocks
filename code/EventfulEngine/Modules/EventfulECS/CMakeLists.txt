set(ECS_SOURCE_FILES
        Private/EFECS.cpp
)
set(ECS_HEADER_FILES
        Public/EFECS.h
        Public/EFStandardComponents.h
)

set(ECS_INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public/Systems>
)

# Create the engine static library.
set(ECS_LIB EFECS)
add_library(${ECS_LIB} SHARED ${ECS_HEADER_FILES} ${ECS_SOURCE_FILES})

target_compile_definitions(${ECS_LIB}
        PRIVATE
        $ENV{PREPROCESSORS}
)

find_package(EnTT CONFIG REQUIRED)

target_link_libraries(${ECS_LIB}
        PRIVATE
        EnTT::EnTT
        EFCore
)

target_include_directories(${ECS_LIB}
        PUBLIC
        ${ECS_INCLUDE_DIRS}
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
