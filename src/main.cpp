#include <iostream>
#include <string>
#include <fstream>  // 文件流
#include <filesystem>   // c++17引入，遍历文件夹及子文件夹
// 第三方库
#include <xlnt/xlnt.hpp> // 操作excel文件
#include <nlohmann/json.hpp>    // 操作json文件

// 遍历excel表，将数据序列化为json
// 参数：excel表路径，生成json文件路径
bool serialize_excel_to_json(const std::string& excel_file_path, const std::string& json_file_path);

int main(int argc, char* argv[]) 
{
    // 默认常量值
    std::string code_mode = "c++";
    std::string path_excel = "./test/excel/";
    std::string path_template = "./test/trmplate/";
    std::string path_json_out = "./test/json/";
    std::string path_code_out = "./test/code/";

    // 从命令行参数读取常量
    //if (argc < 6)
    //{
    //    std::cout << "请设置参数";
    //}
    //else if (argc >= 5)
    //{
    //    // argv[0]为可执行文件名
    //    code_mode = argv[1];  // 第一个参数为模式
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
            serialize_excel_to_json(excel_file, path_json_out); // 转换为json文件
        }
    }

    return 0;
}

bool serialize_excel_to_json(const std::string& excel_file_path, const std::string& json_file_path) 
{
    try 
    {
        // 打开Excel文件
        xlnt::workbook wb;
        wb.load(excel_file_path);

        // 获取第一个活动表
        xlnt::worksheet ws = wb.active_sheet();
        std::string sheet_name = ws.title();

        // 将生成的json文件名使用活动表名
        std::string json_filename = json_file_path + sheet_name + ".json";

        // 读取第一行（字段名），第二行（字段类型），第三行（备注）
        std::vector<std::string> field_names;
        std::vector<std::string> field_types;
        std::vector<std::string> field_comments;

        // 获取第一行的字段名
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_names.push_back(ws.cell(1, col).to_string());
        }

        // 获取第二行的字段类型
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_types.push_back(ws.cell(2, col).to_string());
        }

        // 获取第三行的字段备注
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_comments.push_back(ws.cell(3, col).to_string());
        }

        // 创建JSON对象
        nlohmann::json json_data;

        // 从第四行开始读取数据
        for (std::size_t row = 4; row <= ws.highest_row(); ++row) 
        {
            nlohmann::json row_data;
            for (std::size_t col = 1; col <= field_names.size(); ++col) 
            {
                std::string field_name = field_names[col - 1];
                std::string cell_value = ws.cell(row, col).to_string();
                row_data[field_name] = cell_value;  // 将每个字段名和数据放入json中
            }
            json_data.push_back(row_data);
        }

        // 将JSON数据写入文件
        std::ofstream json_file(json_filename);
        if (json_file.is_open()) 
        {
            json_file << json_data.dump(4);  // 美化输出json
            json_file.close();
            std::cout << "成功序列化文件: " << excel_file_path << " 到 " << json_filename << std::endl;
            return true;
        }
        else 
        {
            std::cerr << "无法打开文件保存JSON: " << json_filename << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "序列化过程中出错: " << e.what() << std::endl;
        return false;
    }
}