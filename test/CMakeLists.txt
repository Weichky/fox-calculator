cmake_minimum_required(VERSION 3.1.0)

project(TEST VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_executable(${PROJECT_NAME}
    ${TEST_SOURCES}
    test.cpp
    )

find_library(GMP_LIBRARY NAMES gmp gmpxx PATHS /usr/local/lib)
find_path(GMP_INCLUDE_DIR NAMES gmp.h PATHS /usr/local/include)

# 如果找到了 GMP 库和头文件，链接它们
if(GMP_LIBRARY AND GMP_INCLUDE_DIR)
    target_link_libraries(${PROJECT_NAME} ${GMP_LIBRARY} gmpxx)
    include_directories(${GMP_INCLUDE_DIR})
else()
    message(FATAL_ERROR "GMP not found!")
endif()