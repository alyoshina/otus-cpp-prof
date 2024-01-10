cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(editor VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(editor 
    src/main.cpp
)

target_include_directories(editor PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
    include/commands
)
    
set_target_properties(editor PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

