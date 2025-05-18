set(PHYS_SOURCE_FILES
        Private/EFPhysics.cpp
)
set(PHYS_HEADER_FILES
        Public/EFPhysics.h
)

set(PHYS_INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
)

# Create the engine static library.
set(PHYS_LIB EFPhysics)
add_library(${PHYS_LIB} SHARED ${PHYS_HEADER_FILES} ${PHYS_SOURCE_FILES})

target_compile_definitions(${PHYS_LIB}
        PRIVATE
        $ENV{PREPROCESSORS}
)

find_package(Bullet CONFIG REQUIRED)

set(BULLET_TARGETS
        BulletDynamics
        BulletCollision
        Bullet3Common
)

target_link_libraries(${PHYS_LIB}
        PRIVATE
        ${BULLET_TARGETS}
        EFCore
)

target_include_directories(${PHYS_LIB}
        PUBLIC
        ${PHYS_INCLUDE_DIRS}
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)