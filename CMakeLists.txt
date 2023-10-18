cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

add_executable(print_ip 
    src/main.cpp
)

target_include_directories(print_ip PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
)

# find_package(GTest REQUIRED)

# add_executable(ip_test 
#     test/main.cpp
#     test/ip_utils_test.cpp
#     src/ip_utils.cpp
# )

# target_link_libraries(ip_test GTest::gtest)

# target_include_directories(ip_test PRIVATE
#     "${CMAKE_BINARY_DIR}"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include"
# )
    
set_target_properties(print_ip PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

# enable_testing()

# add_test(tests ip_test)
