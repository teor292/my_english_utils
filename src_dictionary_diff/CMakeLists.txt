cmake_minimum_required(VERSION 3.5)

project(src_dictionary_diff LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("thirdparty")

file(GLOB MAIN_SRC
    "*.h"
    "*.cpp")




add_executable(src_dictionary_diff ${MAIN_SRC})
target_link_libraries(src_dictionary_diff boost_filesystem)
