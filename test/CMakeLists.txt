# Copyright(C) 2020 Henry Bullingham
# This file is subject to the license terms in the LICENSE file
# found in the top - level directory of this distribution.


if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/catch_amalgamated.hpp)
    file(DOWNLOAD
         https://raw.githubusercontent.com/catchorg/Catch2/devel/extras/catch_amalgamated.hpp
         ${CMAKE_CURRENT_BINARY_DIR}/catch_amalgamated.hpp
         STATUS status
         LOG log)

    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading catch: ${status_string}"
                "${log}")
    endif()
    
    file(DOWNLOAD
         https://raw.githubusercontent.com/catchorg/Catch2/devel/extras/catch_amalgamated.cpp
         ${CMAKE_CURRENT_BINARY_DIR}/catch_amalgamated.cpp
         STATUS status
         LOG log)

    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading catch: ${status_string}"
                "${log}")
    endif()
endif()

set(source_files 
    test_main.cpp
	cppcbb_test.cpp
    )
                 
add_executable(cppcbb_test ${source_files})
target_link_libraries(cppcbb_test PUBLIC cppcbb)
target_include_directories(cppcbb_test PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET cppcbb_test PROPERTY CXX_STANDARD 11)
add_test(NAME test COMMAND cppcbb_test)