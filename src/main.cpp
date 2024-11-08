#include <iostream>
#include <string>
#include <vector>
#include <sstream>  // 字符串流
#include <fstream>  // 文件流
#include <filesystem>   // c++17引入，遍历文件夹及子文件夹
#include <regex>    // 正则表达式库
// 第三方库
#include <xlsxio_read.h> // 操作excel文件
#include <nlohmann/json.hpp>    // 操作json文件

// 遍历excel表，将数据序列化为json
// 参数：excel表路径，生成json文件路径
bool excel_to_json(const std::string& excel_file_path, const std::string& json_path);

// 转c++
bool excel_to_cpp(const std::string& excel_file_path, const std::string& code_path);

int main(int argc, char* argv[]) 
{
    // 默认常量值
    std::string code_mode = "cpp";  // 代码生成语言，默认是cpp
    std::string path_excel = "./test/excel/";   // excel路径
    std::string path_template = "./trmplate/";  // 模板文件路径
    std::string path_json_out = "./test/json/"; // 生成json保存路径
    std::string path_code_out = "./test/code/"; // 生成代码保存路径

    // 从命令行参数读取常量
    //if (argc < 6)
    //{
    //    std::cout << "请设置参数";
    //}
    //else if (argc >= 5)
    //{
    //    // argv[0]为可执行文件名
    //    code_mode = argv[1];  // 第一个参数为代码生成语言
    //    path_excel = argv[2];  // 第二个参数为excel路径
    //    path_template = argv[3]; // 第三个参数是模板文件路径
    //    path_json_out = argv[4]; // 第四个参数是生成json保存路径，
    //    path_code_out = argv[5]; // 第五个参数是生成代码保存路径
    //}

    // 遍历指定目录及子目录，查找所有的.xlsx文件
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path_excel)) // 遍历 path_excel 指定的文件夹及其子文件夹
    {
        if (entry.path().extension() == ".xlsx")    // 查找扩展名为 xlsx的文件
        {
            std::string excel_file = entry.path().string(); // 得到文件的详细路径
            excel_to_json(excel_file, path_json_out); // 转换为json文件

            if (code_mode == "cpp")
            {
                excel_to_cpp(excel_file, path_code_out);
            }
            else
            {
                std::cerr << "不存在的语言模式" << std::endl;
            }
            
        }
    }

    // 根据传入的代码生成模式，选择对应的执行函数

    return 0;
}

bool excel_to_json(const std::string& excel_file_path, const std::string& json_path)
{
    try
    {
        // 打开 .xlsx 文件
        xlsxioreader xlsxioread;
        if ((xlsxioread = xlsxioread_open(excel_file_path.c_str())) == NULL) {
            std::cerr << "打开 Excel 文件失败: " << excel_file_path << std::endl;
            return false;
        }

        // 获取第一个表单的名称
        const char* sheet_name = NULL; // 表单名
        xlsxioreadersheet sheet;    // 表单

        nlohmann::json json_data; // 创建 JSON 数据对象
        std::cout << "正在序列化" << excel_file_path << "\n";

        // 获取表单列表
        xlsxioreadersheetlist sheetlist;
        if ((sheetlist = xlsxioread_sheetlist_open(xlsxioread)) != NULL) 
        {
            // 获取第一个表单名
            sheet_name = xlsxioread_sheetlist_next(sheetlist);
            if (sheet_name == NULL) 
            {
                std::cerr << "未找到表单。" << std::endl;
                return false;
            }
            // 关闭表单列表
            xlsxioread_sheetlist_close(sheetlist);
        }
        else 
        {
            std::cerr << "无法读取表单列表" << std::endl;
            return false;
        }

        // 打开第一个表单
        // XLSXIOREAD_SKIP_EMPTY_ROWS：是否跳过空行：true
        if ((sheet = xlsxioread_sheet_open(xlsxioread, sheet_name, XLSXIOREAD_SKIP_EMPTY_ROWS)) != NULL) {
            // 读取字段名（第一行）
            std::vector<std::string> headers;
            char* value;

            // 读取第一行（字段名）
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    headers.push_back(value);
                    xlsxioread_free(value);
                }
            }

            // 跳过第二行（数据类型）
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    xlsxioread_free(value);  // 忽略第二行内容
                }
            }

            // 跳过第三行（注释）
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    xlsxioread_free(value);  // 忽略第三行内容
                }
            }

            // 读取实际数据（从第四行开始）
            while (xlsxioread_sheet_next_row(sheet)) {
                nlohmann::json row_data; // 创建每一行的 JSON 对象
                for (size_t i = 0; i < headers.size(); ++i) {
                    if ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                        row_data[headers[i]] = value; // 将字段名与数据对应
                        xlsxioread_free(value);
                    }
                }
                json_data.push_back(row_data); // 将行数据添加到 JSON 数组中
            }
            xlsxioread_sheet_close(sheet);  // 关闭表单
        }

        // 关闭 Excel 文件
        xlsxioread_close(xlsxioread);

        if (sheet_name == NULL) // 防止表单名为空
        {
            sheet_name = "null";
        }

        // 根据工作表名称生成 JSON 文件名
        std::string json_file_full_path = json_path + "DT_" + std::string(sheet_name) + ".json"; // 拼接生成的json文件名：路径+第一个表单名+.json
        std::ofstream json_file(json_file_full_path);
        if (json_file.is_open()) {
            json_file << json_data.dump(4); // 使用 4 个空格进行美化输出
            json_file.close();
            std::cout << "序列化完成>>" << json_file_full_path << std::endl;
            return true;
        }
        else {
            std::cerr << "创建 JSON 文件失败: " << json_file_full_path << std::endl;
            return false;
        }
    }
    catch (const std::exception& errpr)
    {
        std::cerr << "序列化过程中出错: " << errpr.what() << std::endl;
        return false;
    }
}

bool excel_to_cpp(const std::string& excel_file_path, const std::string& code_path)
{
    try {
        // 1. 打开 Excel 文件并读取表结构
        xlsxioreader xlsxioread;
        if ((xlsxioread = xlsxioread_open(excel_file_path.c_str())) == NULL) {
            std::cerr << "打开 Excel 文件失败: " << excel_file_path << std::endl;
            return false;
        }

        const char* sheet_name = NULL;
        xlsxioreadersheet sheet;
        std::vector<std::string> headers, types, comments;

        // 获取第一个表单名
        xlsxioreadersheetlist sheetlist = xlsxioread_sheetlist_open(xlsxioread);
        if ((sheet_name = xlsxioread_sheetlist_next(sheetlist)) == NULL) {
            std::cerr << "未找到表单。" << std::endl;
            return false;
        }
        xlsxioread_sheetlist_close(sheetlist);

        // 打开第一个表单，跳过空行
        sheet = xlsxioread_sheet_open(xlsxioread, sheet_name, XLSXIOREAD_SKIP_EMPTY_ROWS);

        // 读取字段名（第一行）
        char* value;
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                headers.push_back(value);
                xlsxioread_free(value);
            }
        }

        // 读取数据类型（第二行）
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                types.push_back(value);
                xlsxioread_free(value);
            }
        }

        // 读取注释（第三行）
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                comments.push_back(value);
                xlsxioread_free(value);
            }
        }

        // 关闭表单和 Excel 文件
        xlsxioread_sheet_close(sheet);
        xlsxioread_close(xlsxioread);

        // 2. 读取模板文件内容
        std::ifstream header_template("cpp_h.hbs");
        std::ifstream cpp_template("cpp_cpp.hbs");
        std::stringstream header_buffer, cpp_buffer;

        header_buffer << header_template.rdbuf();
        cpp_buffer << cpp_template.rdbuf();

        std::string header_template_str = header_buffer.str();
        std::string cpp_template_str = cpp_buffer.str();

        // 3. 替换模板标记
        std::string class_name = sheet_name;
        header_template_str = std::regex_replace(header_template_str, std::regex("\\{\\{CLASS_NAME\\}\\}"), class_name);
        cpp_template_str = std::regex_replace(cpp_template_str, std::regex("\\{\\{CLASS_NAME\\}\\}"), class_name);

        // 替换 {{FIELDS}}
        std::string fields_str;
        for (size_t i = 0; i < headers.size(); ++i) {
            fields_str += "    " + types[i] + " " + headers[i] + "; // " + comments[i] + "\n";
        }
        header_template_str = std::regex_replace(header_template_str, std::regex("\\{\\{FIELDS\\}\\}"), fields_str);

        // 4. 保存生成的代码到头文件和实现文件
        std::string header_file_path = code_path + class_name + ".h";
        std::ofstream header_file(header_file_path);
        if (header_file.is_open()) {
            header_file << header_template_str;
            header_file.close();
            std::cout << "生成头文件: " << header_file_path << std::endl;
        }
        else {
            std::cerr << "无法创建头文件: " << header_file_path << std::endl;
            return false;
        }

        std::string cpp_file_path = code_path + class_name + ".cpp";
        std::ofstream cpp_file(cpp_file_path);
        if (cpp_file.is_open()) {
            cpp_file << cpp_template_str;
            cpp_file.close();
            std::cout << "生成实现文件: " << cpp_file_path << std::endl;
        }
        else {
            std::cerr << "无法创建实现文件: " << cpp_file_path << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& ex) {
        std::cerr << "生成 C++ 代码时出错: " << ex.what() << std::endl;
        return false;
    }
}
