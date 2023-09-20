cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(helloworld_cli 
    src/main.cpp
)

target_include_directories(helloworld_cli PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
)

find_package(GTest REQUIRED)

add_executable(test_version 
    test/main.cpp)

target_link_libraries(test_version GTest::gtest)

target_include_directories(test_version PRIVATE
    "${CMAKE_BINARY_DIR}"
    test
)
    
set_target_properties(helloworld_cli test_version PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_in_test_version test_version)
