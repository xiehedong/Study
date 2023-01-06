#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.3)

#设置msvc输入代码编码格式为utf-8
if(MSVC)
	set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(OpenGLDemo)

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 指定头文件搜索路径
INCLUDE_DIRECTORIES(${ROOT_DIR}/include)

# 指定引用的外部库的搜索路径
LINK_DIRECTORIES(${ROOT_DIR}/lib)

# 指定可执行文件存放目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#4.source directory，源文件目录
AUX_SOURCE_DIRECTORY(src DIR_SRCS)


#5.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(PROJECT_MATH ${DIR_SRCS})

#6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_MATH})


#7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest)