一个使用c++编写的将 excel表格 转换为 json格式文件 和 编程语言(c++,,,)类定义的工具
转语言类定义时，借鉴了 Handlebars 库

# 当前开发进度：未完成，无法使用

# 项目使用方法：
## 安装
### 源码 clone 后编译

### todo：下载可执行文件
请在 relest 文件夹中下载对应系统的可执行文件，目前仅支持 linux(.sh)和 windows(.exe)


## 用前须知
test文件夹为测试文件夹（可删除）：
包含内容：excel表的格式范例，生成的json文件范例，生成的代码文件范例，模板文件范例
各文件的详细格式，请看下面的 “项目文件格式”

启动脚本内的所有路径都指向这个文件夹

每次运行工具，都会将之前的生成文件完全覆盖


## 启动
1. 启动前需要修改脚本
修改脚本的几个变量值，指向实际的路径

2. （可选）自定义模板文件
自定义模板文件，详细修改方式请看

3. 使用脚本进行启动
需要将 启动脚本 与 工具的可执行文件 放在同一个文件夹下
3.1 windows使用.bat脚本
3.2 linux使用.sh脚本


# 项目文件格式
1. excel表格式：(test文件夹下有范例文件)
第一行：填写单列的字段名
第二行：字段对应的类型
第三行：字段的备注
第四行及以后：详细数据

2. json文件：
格式默认
修改格式：需要在源码中进行更改

3. 代码文件：
需要在启动脚本中，修改 code_mode变量以修改生成代码的语言
不同语言有不同的模板文件
修改格式：修改语言对应的模板文件（占位符的含义需要在源码中修改）

4. 模板文件：

5. 启动脚本：
变量：
code_mode：设置生成代码的语言  (默认值：c++)
path_excel：源 excel表的包含目录，只需要指定到包含所有 excel文件的上级目录，即可自动遍历文件夹下所有的 xlsx格式文件  (默认值：../test/excel/)
path_template：模板文件的路径  (默认值：../test/trmplate/)
path_json_out：生成的json文件的路径  (默认值：../test/json/)
path_code_out：生成的类定义的路径  (默认值：../test/code/)


# 项目源码描述
本项目使用cmake进行管理，使用vcpkg进行第三方库管理
第三方库的版本请参考 vcpkg.json文件

## 环境版本
c++版本：c++17
使用vcpkg进行包管理

## 使用的第三方库
来自 vcpkg.json
"dependencies": [
{
    "name": "nlohmann-json",    操作json文件
    "version>=": "3.11.3#1"
},
{
    "name": "xlsxion",          操作excel文件
    "version>=": "0.2.34"
}
]

不知道为什么，使用 xlnt库读取excel文件时，总是提示找不到文件（尝试过更新库，重装库。路径和文件也没有问题）

## 目录树：
文件夹：
script/：启动脚本
src/：源码
template/：转换成代码时的模板
test/：测试用的文件：excel文件，json文件，生成代码
vcpkg_installed/：第三方库文件
    x64-windows/
        bin/：dll动态链接文件，需要复制到生成目录
        lib/：静态链接文件，需要在 CMakeLists文件中链接，编译时加入到可执行文件（使用搜索或link_directories链接会找不到lib文件，是我的开发环境或vcpkg的问题。所以直接在target_link_libraries链接完整路径）
        include/：第三方库头文件，需要在 CMakeLists文件中链接，

文件：
vcpkg.json：vcpkg的第三方库管理文件
vcpkg-configuration.json：vcpkg的库管理文件

