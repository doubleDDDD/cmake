# 设置cmake版本必须大于3.0
cmake_minimum_required (VERSION 3.0)

# Set the project name
# project也就定义了一个项目的名称，并且会生成两个变量
# PROJECT_BINARY_DIR （Make所在的目录）
# PROJECT_SOURCE_DIR （工程所在的目录，即MakeLists所在的目录）
# 变量是否相同，关键在于是内部构建还是外部构建
# 内部构建
#   cmake ./ && make 不可取
# 外部构建
#   mkdir build && cd build && cmake ../ && make  应该使用外部构建
#       没有cmake clean
#       常在在build中进行外部构建，如果我想 make clean，我可以rm -rf build
#       通常，与根目录 "CMakeLists.txt" 位于同一目录中的 "build"文件夹
#       要构建您的项目，只需将cmake的CMakeLists.txt位置作为参数即可
#       cd /build && cmake ..
# cmake
#   Specify a source directory to (re-)generate a build system for it in the current working directory
#   Specify an existing build directory to re-generate its build system
#   指定一个目录去生成构建系统
project (Tutorial VERSION 1.0)

# 版本号 1.0
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

# does this system provide the log and exp functions?
#   log 和 exp 的测试需要放在 configure_file 命令之前
#   configure_file 命令会立即使用 CMake 中的当前设置生成文件
#   当系统提供了这两个函数时，我们可以使用以下代码在 mysqrt 函数中提供一个基于 log 和 exp 的替代实现
include (CheckFunctionExists)
check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)

# 配置一个头文件将一些 CMake 设置传入到源代码中
# 以 TutorialConfig.h.in 为模版，替换相关变量以生成 TutorialConfig.h
# PROJECT_BINARY_DIR 变量为编译发生的目录，也就是make执行的目录 PROJECT_SOURCE_DIR为工程所在的目录 
# configure_file的作用将一份文件拷贝到另一个位置并修改它的内容，使得在代码中使用CMake中定义的变量 
# 由 TutorialConfig.h.in 生成 TutorialConfig.h
configure_file (
    "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"  # 工程所在的目录，自行创建
    "${PROJECT_BINARY_DIR}/TutorialConfig.h"  # make的目录，即build的目录
)

# message(STATUS "it is ${PROJECT_SOURCE_DIR}")  -- it is /home/doubled/double_D/cmake
# message(STATUS "it is ${PROJECT_BINARY_DIR}")  -- it is /home/doubled/double_D/cmake/build

# 将构建目录添加到 include 的搜索路径中以便找到 TutorialConfig.h 文件
# include_directories 添加头文件的目录
#   Add the given directories to those the compiler uses to search for include files
# 另一个方法是
#   target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}" "${EXTRA_INCLUDES}")
include_directories("${PROJECT_BINARY_DIR}")

# 是否使用我们自己的函数？
option (
    USE_MYMATH 
    "Use tutorial provided math implementation" ON)

# Add the dlibs library?
if (USE_MYMATH)
    include_directories ("${PROJECT_SOURCE_DIR}/dlibs")
    # Add a subdirectory to the build; add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
    # the source_dir specifies the directory in which the source CMakeLists.txt and code files are located
    # dlib 独立编译，再链接进入到可执行文件
    add_subdirectory(dlibs)  # 添加一个模块，并且将编译好的库文件放在build/dlibs中
    set (EXTRA_LIBS ${EXTRA_LIBS} dlibs)
endif (USE_MYMATH)

# Add an executable to the project using the specified source files
# add_executable(exename srcname)
aux_source_directory(. DIR_SRCS)
# message(STATUS "it is ${DIR_SRCS}")
add_executable(Tutorial ${DIR_SRCS})
target_link_libraries (Tutorial ${EXTRA_LIBS})  # 链接模块

# add the install targets
# 软件安装的过程说白了就是把头文件与库文件移动到合适的地方么
install (TARGETS Tutorial DESTINATION bin)  # /usr/local/bin
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)

# 测试部分
include(CTest)

# does the application run
add_test (TutorialRuns Tutorial 25)

# does it sqrt of 25
add_test (TutorialComp25 Tutorial 25)
set_tests_properties (TutorialComp25 PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")

# does it handle negative numbers
add_test (TutorialNegative Tutorial -25)
set_tests_properties (TutorialNegative PROPERTIES PASS_REGULAR_EXPRESSION "-25 is 0")

# does it handle small numbers
add_test (TutorialSmall Tutorial 0.0001)
set_tests_properties (TutorialSmall PROPERTIES PASS_REGULAR_EXPRESSION "0.0001 is 0.01")

# does the usage message work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")

# tips
# MakeLists.txt这个文件名的大小写要写对