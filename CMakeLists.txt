# 指定 CMake 的最低版本要求
cmake_minimum_required (VERSION 3.8)

# 设置项目名称(name)和版本(1.0)
project("excel_to_json_code" VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
# 强制要求指定的 C++ 标准必须被支持。如果编译器不支持这个标准，CMake 将会生成错误并停止构建
set(CMAKE_CXX_STANDARD_REQUIRED True)


# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 引入自定义文件
# 在set(SOURCES  )中添加源码的路径
# 在include_directories( ) 中添加头文件所在的文件夹路径
  
   
# 项目中所有自定义文件的 源文件（只需要cpp文件）的路径（.h或资源文件不需要添加）
# 包含的文件才会被编译
set(SOURCES
    src/main.cpp
)

# 包含头文件的目录（包括自定义文件和第三方库文件）
# 例如 include/下有一个文件 aaa.h
# 在源码中引入头文件时使用：#include"aaa.h"，
# 不需要使用"./inlude/aaa.h"
# include_directories(${PROJECT_SOURCE_DIR}/include)

 
# 指定编译生成的可执行文件信息：文件名${PROJECT_NAME} ，${SOURCES} 项目所有源文件路径
add_executable(${PROJECT_NAME} ${SOURCES})


# 引入第三方库
# 使用 include_directories() 指定第三方库的头文件所在目录
# 使用 link_directories() 指定库文件所在文件夹的路径（lib文件或dll文件）
# 使用 target_link_libraries() 中指定库的名称（头文件名）

# 添加第三方库的链接文件所在文件夹路径
# link_directories(/path/to/third_party/lib)
 
# 如果需要链接第三方库，可以在这里指定（项目名，库文件权限，库名列表）
# target_link_libraries(${PROJECT_NAME} PRIVATE 库名1 库名2)

 
# 安装可执行文件到 bin 目录
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET excel_to_json_code PROPERTY CXX_STANDARD 20)
endif()
