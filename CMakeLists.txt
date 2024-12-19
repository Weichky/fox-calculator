# 设置最低的 CMake 版本要求为 3.1.0
cmake_minimum_required(VERSION 3.1.0)

# 定义项目名称为 Fox-Calculator，版本为 0.1.0，使用 C++ 语言
project(Fox-Calculator VERSION 0.1.0 LANGUAGES CXX)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 17)

# 强制使用 C++11 标准，防止使用其他版本的 C++ 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定生成的可执行文件的输出目录为 ${CMAKE_BINARY_DIR}/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 指定生成的库文件的输出目录为 ${PROJECT_BINARY_DIR}/lib
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 启用自动处理 Qt 的 MOC、RCC 和 UIC 文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找并加载 Qt5 的 Widgets 组件
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# find_package(GMP REQUIRED)

set(GMP_INCLUDE_DIR /usr/local/include)
set(GMP_LIBRARIES /usr/local/lib)

include_directories(${GMP_INCLUDE_DIR})
link_directories(${GMP_LIBRARIES})

# 将 Qt5Widgets 的头文件路径添加到编译器的头文件搜索路径中
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
)

# 处理 include/ui/mainwindow.h 文件中的 Qt 特殊语法，生成相应的 MOC 文件
qt5_wrap_cpp(header_SRC include/ui/mainwindow.h)

# 添加项目中 core 和 ui 目录下的头文件路径到编译器的头文件搜索路径中
include_directories(
    ${PROJECT_SOURCE_DIR}/include/core
    ${PROJECT_SOURCE_DIR}/include/ui
    ${PROJECT_SOURCE_DIR}/include/algorithm
    ${PROJECT_SOURCE_DIR}/include/common
    ${PROJECT_SOURCE_DIR}/include/display
    ${PROJECT_SOURCE_DIR}/include/interaction
    ${PROJECT_SOURCE_DIR}/include/thread
)

# 确保所有编译的代码都是位置无关的，以满足 Qt 使用 -reduce-relocations 选项的要求。
# 自动应用 -fPIC 编译选项，减少重定位并提高性能和安全性。
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 添加 src 目录及其 CMake 配置# find_package(GMP REQUIRED)
add_subdirectory(src)

# 定义生成可执行文件的目标，将 main.cpp 和生成的 MOC 文件一起编译
add_executable(${PROJECT_NAME} src/main.cpp
    ${header_SRC}
)

target_link_libraries(${PROJECT_NAME} core ui algorithm common display interaction thread Qt5::Widgets ${GMP_LIBRARIES}/libgmp.a ${GMP_LIBRARIES}/libgmpxx.a)