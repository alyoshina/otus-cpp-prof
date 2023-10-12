cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(allocator 
    src/main.cpp
    src/factorial.cpp
)

target_include_directories(allocator PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
)

find_package(GTest REQUIRED)

add_executable(allocator_test 
    test/main.cpp
    test/alloc_test.cpp
    src/factorial.cpp
)

target_link_libraries(allocator_test GTest::gtest)

target_include_directories(allocator_test PRIVATE
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
    
set_target_properties(allocator allocator_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(tests allocator_test)
