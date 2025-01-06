# Fox Calculator

## 项目概述

此项目实现了任意精度的整数和浮点数计算，包括加法、减法、乘法、除法、取模等基本操作，计划使用Qt编写图形化界面。本项目目前封存。

此项目是一个学习性质的项目，所有实现均为自定义设计，未完成部分存在性能和功能上的不足。

编译时记得根据自己的配置修改`CMakeLists.txt`，不提供`GMP`和`Qt`库，请自行安装。`Qt`部分没做，可以直接在根目录处删除相关内容。
## 功能特性

- **高精度整数计算：** 提供任意位数的整数计算，支持加法、减法、乘法、除法、取模等基本操作。
- **高精度浮点数计算：** 提供对任意精度浮点数的支持，支持加减乘除运算。
- **工厂模式：** 使用工厂方法创建 `IntEx` 和 `FloatEx` 实例，确保高精度计算的便捷性和可扩展性。
- **测试：** 包含基础的测试用例，提供与 GMP 库的效率对比。

## 项目结构

```
fox-calculator
├── CMakeLists.txt         # CMake 构建配置
├── include
│   ├── common
│   │   ├── FloatEx.h      # 高精度浮点数类
│   │   ├── IntEx.h        # 高精度整数类
│   │   ├── NumberEx.h     # 数字基类
│   │   └── utils.h        # 工具函数
│   ├── qt_ui              # 计划中的 Qt UI 文件夹（暂时为空）
│   └── ui                 # UI 相关文件夹（暂时为空）
│
├── lib                    # 外部库（暂时为空）
├── run.sh                 # 运行脚本
├── src
│   ├── common
│   │   ├── FloatEx.cpp    # 高精度浮点数类实现
│   │   ├── IntEx.cpp      # 高精度整数类实现
│   │   ├── NumberEx.cpp   # 数字基类实现
│   │   └── utils.cpp      # 工具函数实现
│   ├── core               # 核心计算部分（暂时为空）
│   └── ui                 # UI 相关代码（暂时为空）
├── test
│   ├── CMakeLists.txt     # 测试项目 CMake 配置
│   ├── include
│   │   ├── FloatEx.h      
│   │   ├── IntEx.h        
│   │   ├── NumberEx.h     
│   │   └── utils.h        
│   ├── src
│   │   ├── CMakeLists.txt # 测试项目的 CMake 配置
│   │   ├── FloatEx.cpp    
│   │   ├── IntEx.cpp      
│   │   ├── NumberEx.cpp   
│   │   └── utils.cpp      
│   └── test.cpp           # 测试程序，包含基本运算的测试与 GMP 效率对比
└── tree.md                # 项目目录结构说明
```

## 主要类和功能

### `IntEx` 类

- 用于处理任意精度的整数，支持加法、减法、乘法、除法、取模等基本运算。
- 使用 `std::vector<uint64_t>` 存储大数的每一块数据。
- 支持运算符重载和逻辑比较。
- 支持通过工厂模式创建实例，支持从字符串转换和输出为字符串。
- 使用传统算法实现。

#### 主要方法

- `create`：通过字符串创建 `IntEx` 实例。
- `operator+`, `operator-`, `operator*`, `operator/`, `operator%`：支持基本数学运算。
- `from_string` 和 `to_string`：实现字符串与任意精度整数之间的转换。

### `FloatEx` 类

- 用于处理任意精度浮点数，包括尾数和指数部分。
- 支持浮点数的加法、减法、乘法、除法等基本运算。
- 使用 `IntEx` 作为尾数（`mantissa`），支持精确表示浮动数值。
- 提供了 `align`、`add`、`sub`、`mult` 和 `div` 等方法，用于浮点数的计算。

#### 主要方法

- `create`：通过字符串创建 `FloatEx` 实例。
- `operator+`, `operator-`, `operator*`, `operator/`：支持浮点数的基本运算。
- `from_string` 和 `to_string`：实现字符串与任意精度浮点数之间的转换。

## 运行脚本

项目包含了一个脚本 `run.sh`，用于简化构建和运行过程。命令格式如下：

```bash
./run.sh [build/test] [-c/-r/-cr]
```

- `[build/test]`：选择要执行的操作类型：
  - `build`：构建整个项目。
  - `test`：只构建测试项目（针对 `test` 文件夹）。
- `[-c/-r/-cr]`：选择操作：
  - `-c`：清除构建目录。
  - `-r`：运行项目。
  - `-cr`：清除构建目录并运行项目。

### 示例：

- 构建整个项目并运行：

  ```bash
  ./run.sh build -cr
  ```

- 仅构建测试项目并运行：

  ```bash
  ./run.sh test -r
  ```

- 清除构建目录并重新构建测试项目：

  ```bash
  ./run.sh test -c
  ```

## 未完成的部分

- **GUI 部分：** 原计划使用 Qt 实现 GUI 界面，但由于技术方向的调整，UI 部分目前为空。
- **核心计算部分：** 部分优化算法和功能尚未完全实现，计划后续逐步补充。
- **外部库：** 目前未引入其他外部库，后续可能会增加与其他科学计算库的集成。

## 运行与编译

1. 克隆项目：

```bash
git clone https://github.com/Weichky/fox-calculator.git
cd fox-calculator
```

2. 编译项目：
手动：
```bash
mkdir build
cd build
cmake ..
make
```
自动：
```bash
./run.sh [build/test] [-c/-r/-cr]
```

3. 运行程序：
手动：
请到对应bulid文件夹下寻找

自动：
使用`run.sh`时请添加`-r`参数
