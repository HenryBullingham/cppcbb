# Copyright(C) 2020 Henry Bullingham
# This file is subject to the license terms in the LICENSE file
# found in the top - level directory of this distribution.


add_executable(cppcbb_example cppcbb_examples.cpp)            
target_link_libraries(cppcbb_example PUBLIC cppcbb)
target_include_directories(cppcbb_example PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET cppcbb_example PROPERTY CXX_STANDARD 17)
