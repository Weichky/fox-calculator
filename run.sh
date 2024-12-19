#!/bin/bash

export QT_QPA_PLATFORM=xcb

# 初始化标志和变量
run_program=0
clean_build=0
action=""
program_name="Fox-Calculator"

# 解析参数
while [[ $# -gt 0 ]]; do
  case "$1" in
    build|test)
      action="$1"
      shift
      ;;
    -r)
      run_program=1
      shift
      ;;
    -c)
      clean_build=1
      shift
      ;;
    -rc|-cr)
      run_program=1
      clean_build=1
      shift
      ;;
    *)
      echo "Invalid argument: $1"
      echo "Usage: ./run.sh [build|test] [-r] [-c] [-rc|-cr]"
      exit 1
      ;;
  esac
done

# 检查是否提供了必要的行为参数
if [ -z "$action" ]; then
  echo "Usage: ./run.sh [build|test] [-r] [-c] [-rc|-cr]"
  exit 1
fi

# 根据选择的行为执行相应的操作
case "$action" in
  build)
    echo "Running build process..."

    # 如果有 -c 参数，则清理构建目录
    if [ "$clean_build" -eq 1 ]; then
      echo "Cleaning build directory..."
      rm -rf build
      mkdir build
    fi

    cd build || { echo "Failed to enter 'build' directory"; exit 1; }
    cmake .. || { echo "CMake configuration failed"; exit 1; }
    make || { echo "Build failed"; exit 1; }

    # 如果有 -r 参数，则运行程序
    if [ "$run_program" -eq 1 ]; then
      ./bin/$program_name || { echo "Program execution failed"; exit 1; }
    fi

    cd ..
    ;;
  test)
    echo "Running test process..."

    # 如果有 -c 参数，则清理测试构建目录
    if [ "$clean_build" -eq 1 ]; then
      echo "Cleaning test build directory..."
      rm -rf test/build
      mkdir -p test/build
    fi

    cd test/build || { echo "Failed to enter 'build' directory"; exit 1; }
    cmake .. || { echo "CMake configuration failed"; exit 1; }
    make || { echo "Build failed"; exit 1; }

    # 如果有 -r 参数，则运行测试程序
    if [ "$run_program" -eq 1 ]; then
      ./bin/TEST || { echo "Program execution failed"; exit 1; }
    fi

    cd ../..
    ;;
  *)
    echo "Invalid argument. Use 'build' or 'test'."
    exit 1
    ;;
esac
