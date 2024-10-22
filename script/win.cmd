@echo off

@REM 设置生成代码的语言
set code_mode=c++
@REM set mode=java
@REM set mode=rust
@REM set mode=php

@REM 设置文件路径
set path_excel=C:/Users/User/data
set path_template=C:/Users/User/data
set path_json_out=C:/Users/User/data
set path_code_out=C:/Users/User/data

@REM 运行 C++ 程序，并传递常量作为命令行参数
my_program.exe %code_mode% %path_excel% %path_template% %path_json_out% %path_code_out%

@REM 暂停保持窗口
pause