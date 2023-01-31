### �����о���������cmake��ص�����
#### 1. ��ν��Լ���д�Ĵ�������һ����
   ��һ�������Լ�Դ��Ŀ¼���ϼ�Ŀ¼��CMakeLists.txt�� ����������
      add_subdirectory(code_directory)
   �ڶ�������Դ��Ŀ¼���ṩ���µ�CMakeLists.txt�ļ�
 ```cpp
      file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
      file(GLOB_RECURSE HEADRERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
      file(GLOB_RECURSE HPPS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
      set(target_name "�������")
      ADD_QTC_LIBRARY(${target_name} "STATIC OR SHARED"
         DEFINES CAXACTOR
         DEPENDS CaxConfig
        SOURCES
           ${SRCS}
           ${HEADRERS}
           ${HPPS}
          )
  ```
  #### 2. �ڱ�д����ʱ�������ͷ�ļ�
    a.ͬ��Ŀ¼�µ�ͷ�ļ� ʹ�� #include "test.h"
    b.����ģ���µ�ͷ�ļ�ʹ�� #include <lib_name/test.h>������ʹ�����Ŀ¼��˫�������ã�ǰ�����ڵ�ǰģ������������ģ��
       DEPENDS "lib_name"
    c. ϵͳĿ¼ʹ�ü���������
    d. ��Qt������ò�����ʹ��#include<QtWidget/xxxwidget.h>����#inlcude <QtCore/xxx.h>
       ,Ӧ��ֱ��ʹ��#include<xxxwidget.h>��#inlcude <xxx.h>��Ϊ�ڶ����cmake���Ѿ���֪��Qt�Ŀ�·����û�б�Ҫ�ټ������
    e. ���������ͷ�ļ�������ã�1.�����������ͷ�ļ��Ϳ��ļ�����3rdpartyĿ¼�£�2.����Ҫ�������������ģ���CMakeLists.txt
       ����Ե������������ 
 ```cpp
       target_include_directories({target_name} PUBLIC {PROJECT_SOURCE_DIR/src/3rdparty/include_path})
       target_link_directories({target_name} PUBLIC {PROJECT_SOURCE_DIR/src/3rdparty/lib_path})
       target_link_libraries({target_name} PUBLIC {lib_name})
  ```
####  3. ��ν������ļ������������Ҫ��ȡ���ļ����ڳ�������ʱ�ҵõ�
     ��һ������ģ�������ļ�Ŀ¼�д��һ����Ҫ��ȡ���ļ���ʹ��cmake���������ɻ����ʱ������
     �Զ������ı���Ŀ¼
```cpp
     file(GLOB scripts_files ${CMAKE_CURRENT_SOURCE_DIR}/event_bus/*)
     file (INSTALL  ${scripts_files}  DESTINATION ${CMAKE_BINARY_DIR}/${IDE_APP_PATH}/event_bus/ )
     # ���ģʽ��ʹ�� 
   install(
     DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/event_bus/
     DESTINATION ${CMAKE_INSTALL_PREFIX}/${IDE_APP_PATH}/event_bus/
    )
  ```
    �ڶ���:�ڳ����ж�ȡ�ļ���1 ͨ����ִ�г����Ŀ¼��ȡ
    
   ```cpp

    Qt ����
    QString lanFile = QDir::cleanPath(qApp->applicationDirPath())
     
     C++17 std::filesystem::current_path
     #include <iostream>
     #include <filesystem>
     namespace fs = std::filesystem;
    int main()
    {
      std::cout << "Current working directory: " << fs::current_path() << '\n'; 
     }

     windowsƽ̨ _getcwd
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

   linuxƽ̨ getcwd
    #include <unistd.h>

    int main() 
   {
    char *cwd = get_current_dir_name();
    cout << "Current working directory: " << cwd << endl;
    free(cwd);
    return 0;
   }
    
   ```
   2 ͨ��ϵͳ�Ļ�������

   #### 4 ��Ԫ���������������ʱ�ҵ������ļ�
    ��ÿһ����Ԫ����ģ���У����Ѿ��ṩ����һ�������ļ��У�cmake���ɻ����ʱ��������ļ��п���������Ŀ¼�£�������ֻ��Ҫ��
    ��Ĳ����ļ���������Ӧ�Ĳ����ļ����¾Ϳ���
   #### 5 CMake���ɻ����Ŀ¼������
    vs2019���ɵ�cmake����Ĭ������C�̵�userĿCMakeBuilds�ļ����£�vs2022Ĭ�����ɻ����Ŀ¼���ڹ����ļ����µ�outĿ¼��
    ��������CMakeSettings.json��  "buildRoot": "${projectDir}\\out\\build\\${name}"
   #### 6 CMake������ɾ�̬��Ͷ�̬�⣬�Լ���̬��Ͷ�̬�������
```cpp
add_library(${PROJECT_NAME} SHARED library.cpp library.h)
 
```
SHARED��ʾ�Ƕ�̬�⣬STATIC��ʾ�Ǿ�̬�⡣
1. ��̬�⼴��̬���ӿ⣨Windows �µ� .lib��Linux �� Mac �µ� .a����֮���Խ�����̬������Ϊ��̬���ڱ����ʱ��ᱻֱ�ӿ���һ�ݣ����Ƶ�Ŀ��������δ�����Ŀ�������Ͳ����ٸı��ˡ�
��̬��ĺô������ԣ��������֮�󣬿��ļ�ʵ���Ͼ�û�������ˡ�Ŀ�����û���ⲿ������ֱ�ӾͿ������С���Ȼ��ȱ��Ҳ�����ԣ����ǻ�ʹ��Ŀ�������������
��̬�⼴��̬���ӿ⣨Windows �µ� .dll��Linux �µ� .so��Mac �µ� .dylib�����뾲̬���෴����̬���ڱ���ʱ�����ᱻ������Ŀ������У�Ŀ�������ֻ��洢ָ��̬������á��ȵ���������ʱ����̬��Żᱻ�������ؽ�����
2.windows�±���Ķ�̬����Ҫʹ�ã�����Ҫ�е�������Linuxϵͳ��̬��ĺ�������Ĭ���ǵ����ġ�
3.����ʹ�õ�ʱ�򣬵����Ǳ���ɶ�̬�⻹�Ǿ�̬���أ����Ҫ�������ʹ�ó����������Ŀ������ķǳ����ֺܵײ���Ƽ�ʹ�þ�̬�⣬���������һ���ʹ���ˡ������Ŀ�ᱻ�ܶ�������Ŀ���þ��Ƽ�ʹ�ö�̬��
  ������������һ���Ƚ����ص������Ǿ�̬���е��������ڳ����лᷢ���ж�� ʵ����
