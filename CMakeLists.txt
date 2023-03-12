# CMakeList.txt: GeneExpressionProgramming 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("GeneExpressionProgramming")

include_directories(./include)
#将当前目录(.)下的所有文件添加到编译器
aux_source_directory(./src SourcePath)
# set(SourcePath
#               main.cpp
#               test.h
#               test.cpp
#               parameter.h
#               dataSource.h
#               dataSource.cpp 
#               BinaryTree.cpp
#               BinaryTree.h
#               Gene.cpp
#               Gene.h
#               individual.cpp
#               individual.h
#               population.cpp
#               population.h
#               CalculateOperation.h)
# 将源代码添加到此项目的可执行文件。
add_executable (GeneExpressionProgramming ${SourcePath})  
add_library(GEP STATIC ${SourcePath}) #编译静态库
set(LIBRARY_OUTPUT_PATH ../lib)
set(EXECUTABLE_OUTPUT_PATH ./)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GeneExpressionProgramming PROPERTY CXX_STANDARD 11)
endif()

# TODO: 如有需要，请添加测试并安装目标。
