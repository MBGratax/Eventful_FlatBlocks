
set(SOURCE_FILES
        Src/main.cpp
        Src/Pch.h
        Src/Pch.cpp)

set(EXECUTABLE FlatBlocks)
# Create the game executable.
add_executable(${EXECUTABLE} ${SOURCE_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE $ENV{PREPROCESSORS})

set(GAME_INCLUDE_DIRS
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${EXECUTABLE} PUBLIC ${GAME_INCLUDE_DIRS})
# Set up pch
target_precompile_headers(${EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

find_package(glfw3 CONFIG REQUIRED)

# Link the game executable with the engine library.
target_link_libraries(${EXECUTABLE} PRIVATE
        Eventful
        COMPILER_FLAGS
        glfw)
