cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

configure_file(version.h.in version.h)

find_package(Boost CONFIG COMPONENTS program_options filesystem regex REQUIRED)

add_executable(bayan 
    src/main.cpp
    src/scan.cpp
    src/calc.cpp
    src/hash_algorithms.cpp
)

target_include_directories(bayan PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
)

target_link_libraries(bayan Boost::program_options Boost::filesystem Boost::regex)
    
set_target_properties(bayan PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bayan PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bayan PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
