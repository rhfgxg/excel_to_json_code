#include <iostream>
#include <string>
#include <fstream>  // �ļ���
#include <filesystem>   // c++17���룬�����ļ��м����ļ���
// ��������
#include <xlnt/xlnt.hpp> // ����excel�ļ�
#include <nlohmann/json.hpp>    // ����json�ļ�

// ����excel�����������л�Ϊjson
// ������excel��·��������json�ļ�·��
bool serialize_excel_to_json(const std::string& excel_file_path, const std::string& json_file_path);

int main(int argc, char* argv[]) 
{
    // Ĭ�ϳ���ֵ
    std::string code_mode = "c++";
    std::string path_excel = "./test/excel/";
    std::string path_template = "./test/trmplate/";
    std::string path_json_out = "./test/json/";
    std::string path_code_out = "./test/code/";

    // �������в�����ȡ����
    //if (argc < 6)
    //{
    //    std::cout << "�����ò���";
    //}
    //else if (argc >= 5)
    //{
    //    // argv[0]Ϊ��ִ���ļ���
    //    code_mode = argv[1];  // ��һ������Ϊģʽ
    //    path_excel = argv[2];  // �ڶ�������Ϊexcel·��
    //    path_template = argv[3]; // ������������ģ���ļ�·��
    //    path_json_out = argv[4]; // ���ĸ�����������json����·����
    //    path_code_out = argv[5]; // ��������������ɴ��뱣��·��
    //}

    // ����ָ��Ŀ¼����Ŀ¼���������е�.xlsx�ļ�

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path_excel)) // ���� path_excel ָ�����ļ��м������ļ���
    {
        if (entry.path().extension() == ".xlsx")    // ������չ��Ϊ xlsx���ļ�
        {
            std::string excel_file = entry.path().string(); // �õ��ļ�����ϸ·��
            serialize_excel_to_json(excel_file, path_json_out); // ת��Ϊjson�ļ�
        }
    }

    return 0;
}

bool serialize_excel_to_json(const std::string& excel_file_path, const std::string& json_file_path) 
{
    try 
    {
        // ��Excel�ļ�
        xlnt::workbook wb;
        wb.load(excel_file_path);

        // ��ȡ��һ�����
        xlnt::worksheet ws = wb.active_sheet();
        std::string sheet_name = ws.title();

        // �����ɵ�json�ļ���ʹ�û����
        std::string json_filename = json_file_path + sheet_name + ".json";

        // ��ȡ��һ�У��ֶ��������ڶ��У��ֶ����ͣ��������У���ע��
        std::vector<std::string> field_names;
        std::vector<std::string> field_types;
        std::vector<std::string> field_comments;

        // ��ȡ��һ�е��ֶ���
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_names.push_back(ws.cell(1, col).to_string());
        }

        // ��ȡ�ڶ��е��ֶ�����
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_types.push_back(ws.cell(2, col).to_string());
        }

        // ��ȡ�����е��ֶα�ע
        for (size_t col = 1; col <= ws.highest_column().index; ++col) 
        {
            field_comments.push_back(ws.cell(3, col).to_string());
        }

        // ����JSON����
        nlohmann::json json_data;

        // �ӵ����п�ʼ��ȡ����
        for (std::size_t row = 4; row <= ws.highest_row(); ++row) 
        {
            nlohmann::json row_data;
            for (std::size_t col = 1; col <= field_names.size(); ++col) 
            {
                std::string field_name = field_names[col - 1];
                std::string cell_value = ws.cell(row, col).to_string();
                row_data[field_name] = cell_value;  // ��ÿ���ֶ��������ݷ���json��
            }
            json_data.push_back(row_data);
        }

        // ��JSON����д���ļ�
        std::ofstream json_file(json_filename);
        if (json_file.is_open()) 
        {
            json_file << json_data.dump(4);  // �������json
            json_file.close();
            std::cout << "�ɹ����л��ļ�: " << excel_file_path << " �� " << json_filename << std::endl;
            return true;
        }
        else 
        {
            std::cerr << "�޷����ļ�����JSON: " << json_filename << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "���л������г���: " << e.what() << std::endl;
        return false;
    }
}