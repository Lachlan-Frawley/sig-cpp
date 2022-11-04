cmake_minimum_required(VERSION 3.16)
project(sigcpp VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TEST_PROGRAM "Build testing program" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXPORT_HEADERS sig.h)

add_library(sigcpp SHARED sig.cpp)
add_library(sigcpp-static STATIC sig.cpp)

if(BUILD_TEST_PROGRAM)
    find_package(CLI11 REQUIRED)
    add_executable(sig-test test_program.cpp)
    target_link_libraries(sig-test PUBLIC sigcpp CLI11::CLI11)
endif(BUILD_TEST_PROGRAM)

set_target_properties(sigcpp PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION 1)

set(include_dest "include/sigcpp")
set(project_lib_dest "lib/cmake/sigcpp")
set(lib_dest "${project_lib_dest}")

install(TARGETS sigcpp EXPORT sigcpp DESTINATION lib)
install(TARGETS sigcpp-static EXPORT sigcpp-static DESTINATION lib)
install(FILES ${EXPORT_HEADERS} DESTINATION "${include_dest}")
install(
        EXPORT sigcpp
        DESTINATION "${lib_dest}"
        NAMESPACE sigcpp::
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/sigcpp-config.cmake"
  INSTALL_DESTINATION "${project_lib_dest}"
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sigcpp-config.cmake DESTINATION "${project_lib_dest}")
