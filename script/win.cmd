@echo off

@REM �������ɴ��������
set code_mode=c++
@REM set mode=java
@REM set mode=rust
@REM set mode=php

@REM �����ļ�·��
set path_excel=C:/Users/User/data
set path_template=C:/Users/User/data
set path_json_out=C:/Users/User/data
set path_code_out=C:/Users/User/data

@REM ���� C++ ���򣬲����ݳ�����Ϊ�����в���
my_program.exe %code_mode% %path_excel% %path_template% %path_json_out% %path_code_out%

@REM ��ͣ���ִ���
pause