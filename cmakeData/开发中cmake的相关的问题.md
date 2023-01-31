### 开发中经常遇到与cmake相关的问题
#### 1. 如何将自己编写的代码编译成一个库
   第一步：在自己源码目录的上级目录中CMakeLists.txt中 添加如下语句
      add_subdirectory(code_directory)
   第二步：在源码目录下提供如下的CMakeLists.txt文件
 ```cpp
      file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
      file(GLOB_RECURSE HEADRERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
      file(GLOB_RECURSE HPPS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
      set(target_name "库的名称")
      ADD_QTC_LIBRARY(${target_name} "STATIC OR SHARED"
         DEFINES CAXACTOR
         DEPENDS CaxConfig
        SOURCES
           ${SRCS}
           ${HEADRERS}
           ${HPPS}
          )
  ```
  #### 2. 在编写代码时如何引用头文件
    a.同级目录下的头文件 使用 #include "test.h"
    b.其他模块下的头文件使用 #include <lib_name/test.h>，不得使用相对目录，双引号引用，前提是在当前模块中依赖其他模块
       DEPENDS "lib_name"
    c. 系统目录使用尖括号引用
    d. 对Qt库的引用不建议使用#include<QtWidget/xxxwidget.h>或者#inlcude <QtCore/xxx.h>
       ,应该直接使用#include<xxxwidget.h>或#inlcude <xxx.h>因为在顶层的cmake中已经告知了Qt的库路径，没有必要再加入包名
    e. 第三方库的头文件如何引用，1.将第三方库的头文件和库文件放在3rdparty目录下，2.在需要依赖第三方库的模块的CMakeLists.txt
       加入对第三方库的依赖 
 ```cpp
       target_include_directories({target_name} PUBLIC {PROJECT_SOURCE_DIR/src/3rdparty/include_path})
       target_link_directories({target_name} PUBLIC {PROJECT_SOURCE_DIR/src/3rdparty/lib_path})
       target_link_libraries({target_name} PUBLIC {lib_name})
  ```
####  3. 如何将配置文件或则代码中需要读取的文件，在程序运行时找得到
     第一步：在模块代码的文件目录中存放一份需要读取的文件，使用cmake机制在生成缓存的时候，让其
     自动拷贝的编译目录
```cpp
     file(GLOB scripts_files ${CMAKE_CURRENT_SOURCE_DIR}/event_bus/*)
     file (INSTALL  ${scripts_files}  DESTINATION ${CMAKE_BINARY_DIR}/${IDE_APP_PATH}/event_bus/ )
     # 打包模式下使用 
   install(
     DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/event_bus/
     DESTINATION ${CMAKE_INSTALL_PREFIX}/${IDE_APP_PATH}/event_bus/
    )
  ```
    第二步:在程序中读取文件，1 通过可执行程序的目录读取
    
   ```cpp

    Qt 机制
    QString lanFile = QDir::cleanPath(qApp->applicationDirPath())
     
     C++17 std::filesystem::current_path
     #include <iostream>
     #include <filesystem>
     namespace fs = std::filesystem;
    int main()
    {
      std::cout << "Current working directory: " << fs::current_path() << '\n'; 
     }

     windows平台 _getcwd
     #include <direct.h>
    std::string current_working_directory()
    {
    char buff[250];
    _getcwd(buff, 250); 
    std::string current_working_directory(buff);
    return current_working_directory;
    }

    int main()
   {
    std::cout << "Current working directory: " << current_working_directory() << endl;
   }

   linux平台 getcwd
    #include <unistd.h>

    int main() 
   {
    char *cwd = get_current_dir_name();
    cout << "Current working directory: " << cwd << endl;
    free(cwd);
    return 0;
   }
    
   ```
   2 通过系统的环境变量

   #### 4 单元测试中如何在运行时找到测试文件
    在每一个单元测试模块中，都已经提供好了一个测试文件夹，cmake生成缓存的时候会把这个文件夹拷贝到编译目录下，所以你只需要将
    你的测试文件拷贝到对应的测试文件夹下就可以
   #### 5 CMake生成缓存的目录在哪里
    vs2019生成的cmake缓存默认是在C盘的user目CMakeBuilds文件夹下，vs2022默认生成缓存的目录是在工程文件夹下的out目录，
    配置是在CMakeSettings.json的  "buildRoot": "${projectDir}\\out\\build\\${name}"
   #### 6 CMake如何生成静态库和动态库，以及静态库和动态库的区别
```cpp
add_library(${PROJECT_NAME} SHARED library.cpp library.h)
 
```
SHARED表示是动态库，STATIC表示是静态库。
1. 静态库即静态链接库（Windows 下的 .lib，Linux 和 Mac 下的 .a）。之所以叫做静态，是因为静态库在编译的时候会被直接拷贝一份，复制到目标程序里，这段代码在目标程序里就不会再改变了。
静态库的好处很明显，编译完成之后，库文件实际上就没有作用了。目标程序没有外部依赖，直接就可以运行。当然其缺点也很明显，就是会使用目标程序的体积增大
动态库即动态链接库（Windows 下的 .dll，Linux 下的 .so，Mac 下的 .dylib）。与静态库相反，动态库在编译时并不会被拷贝到目标程序中，目标程序中只会存储指向动态库的引用。等到程序运行时，动态库才会被真正加载进来。
2.windows下编译的动态库若要使用，必须要有导出符，Linux系统动态库的函数符号默认是导出的。
3.最终使用的时候，到底是编译成动态库还是静态库呢，这个要看具体的使用场景，如果你的库依赖的非常多又很底层就推荐使用静态库，这样打包在一起就使用了。如果你的库会被很多个其他的库调用就推荐使用动态库
  ，经常会遇到一个比较隐秘的问题是静态库中单例对象在程序中会发现有多个 实例。
