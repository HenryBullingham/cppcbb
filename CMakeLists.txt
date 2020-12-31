# Copyright(C) 2020 Henry Bullingham
# This file is subject to the license terms in the LICENSE file
# found in the top - level directory of this distribution.


cmake_minimum_required(VERSION 3.1)

project(cppcbb)

set(header_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cppcbb/cbb_vector.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cppcbb/cbb_map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cppcbb/cbb_common.hpp)

add_library(cppcbb INTERFACE)
target_sources(cppcbb INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(cppcbb INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>")
target_include_directories(cppcbb SYSTEM INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/>")

add_custom_target(cppcbb_ SOURCES ${header_files})

# Setup package config
include( CMakePackageConfigHelpers )
set(CONFIG_PACKAGE_INSTALL_DIR lib/cmake/cppcbb)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cppcbb-config-version.cmake
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion)

# Install target
install(DIRECTORY include/cppcbb DESTINATION include)

enable_testing()
install(TARGETS cppcbb
    EXPORT cppcbb-targets
    DESTINATION lib)

install( EXPORT cppcbb-targets
    DESTINATION
    ${CONFIG_PACKAGE_INSTALL_DIR})

install( FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cppcbb-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cppcbb-config-version.cmake
    DESTINATION
    ${CONFIG_PACKAGE_INSTALL_DIR} )
    
option(CPPCBB_BUILD_TEST "build tests" ON)
if(${CPPCBB_BUILD_TEST})
    add_subdirectory(test/)
endif()

option(CPPDCBB_BUILD_EXAMPLES "build examples" ON)
if(${CPPDCBB_BUILD_EXAMPLES})
    add_subdirectory(examples/)
endif()
