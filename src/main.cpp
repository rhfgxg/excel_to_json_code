#include <iostream>
#include <string>
#include <vector>
#include <sstream>  // �ַ�����
#include <fstream>  // �ļ���
#include <filesystem>   // c++17���룬�����ļ��м����ļ���
#include <regex>    // ������ʽ��
// ��������
#include <xlsxio_read.h> // ����excel�ļ�
#include <nlohmann/json.hpp>    // ����json�ļ�

// ����excel�����������л�Ϊjson
// ������excel��·��������json�ļ�·��
bool excel_to_json(const std::string& excel_file_path, const std::string& json_path);

// תc++
bool excel_to_cpp(const std::string& excel_file_path, const std::string& code_path);

int main(int argc, char* argv[]) 
{
    // Ĭ�ϳ���ֵ
    std::string code_mode = "cpp";  // �����������ԣ�Ĭ����cpp
    std::string path_excel = "./test/excel/";   // excel·��
    std::string path_template = "./trmplate/";  // ģ���ļ�·��
    std::string path_json_out = "./test/json/"; // ����json����·��
    std::string path_code_out = "./test/code/"; // ���ɴ��뱣��·��

    // �������в�����ȡ����
    //if (argc < 6)
    //{
    //    std::cout << "�����ò���";
    //}
    //else if (argc >= 5)
    //{
    //    // argv[0]Ϊ��ִ���ļ���
    //    code_mode = argv[1];  // ��һ������Ϊ������������
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
            excel_to_json(excel_file, path_json_out); // ת��Ϊjson�ļ�

            if (code_mode == "cpp")
            {
                excel_to_cpp(excel_file, path_code_out);
            }
            else
            {
                std::cerr << "�����ڵ�����ģʽ" << std::endl;
            }
            
        }
    }

    // ���ݴ���Ĵ�������ģʽ��ѡ���Ӧ��ִ�к���

    return 0;
}

bool excel_to_json(const std::string& excel_file_path, const std::string& json_path)
{
    try
    {
        // �� .xlsx �ļ�
        xlsxioreader xlsxioread;
        if ((xlsxioread = xlsxioread_open(excel_file_path.c_str())) == NULL) {
            std::cerr << "�� Excel �ļ�ʧ��: " << excel_file_path << std::endl;
            return false;
        }

        // ��ȡ��һ����������
        const char* sheet_name = NULL; // ����
        xlsxioreadersheet sheet;    // ��

        nlohmann::json json_data; // ���� JSON ���ݶ���
        std::cout << "�������л�" << excel_file_path << "\n";

        // ��ȡ���б�
        xlsxioreadersheetlist sheetlist;
        if ((sheetlist = xlsxioread_sheetlist_open(xlsxioread)) != NULL) 
        {
            // ��ȡ��һ������
            sheet_name = xlsxioread_sheetlist_next(sheetlist);
            if (sheet_name == NULL) 
            {
                std::cerr << "δ�ҵ�����" << std::endl;
                return false;
            }
            // �رձ��б�
            xlsxioread_sheetlist_close(sheetlist);
        }
        else 
        {
            std::cerr << "�޷���ȡ���б�" << std::endl;
            return false;
        }

        // �򿪵�һ����
        // XLSXIOREAD_SKIP_EMPTY_ROWS���Ƿ��������У�true
        if ((sheet = xlsxioread_sheet_open(xlsxioread, sheet_name, XLSXIOREAD_SKIP_EMPTY_ROWS)) != NULL) {
            // ��ȡ�ֶ�������һ�У�
            std::vector<std::string> headers;
            char* value;

            // ��ȡ��һ�У��ֶ�����
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    headers.push_back(value);
                    xlsxioread_free(value);
                }
            }

            // �����ڶ��У��������ͣ�
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    xlsxioread_free(value);  // ���Եڶ�������
                }
            }

            // ���������У�ע�ͣ�
            if (xlsxioread_sheet_next_row(sheet)) {
                while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                    xlsxioread_free(value);  // ���Ե���������
                }
            }

            // ��ȡʵ�����ݣ��ӵ����п�ʼ��
            while (xlsxioread_sheet_next_row(sheet)) {
                nlohmann::json row_data; // ����ÿһ�е� JSON ����
                for (size_t i = 0; i < headers.size(); ++i) {
                    if ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                        row_data[headers[i]] = value; // ���ֶ��������ݶ�Ӧ
                        xlsxioread_free(value);
                    }
                }
                json_data.push_back(row_data); // ����������ӵ� JSON ������
            }
            xlsxioread_sheet_close(sheet);  // �رձ�
        }

        // �ر� Excel �ļ�
        xlsxioread_close(xlsxioread);

        if (sheet_name == NULL) // ��ֹ����Ϊ��
        {
            sheet_name = "null";
        }

        // ���ݹ������������� JSON �ļ���
        std::string json_file_full_path = json_path + "DT_" + std::string(sheet_name) + ".json"; // ƴ�����ɵ�json�ļ�����·��+��һ������+.json
        std::ofstream json_file(json_file_full_path);
        if (json_file.is_open()) {
            json_file << json_data.dump(4); // ʹ�� 4 ���ո�����������
            json_file.close();
            std::cout << "���л����>>" << json_file_full_path << std::endl;
            return true;
        }
        else {
            std::cerr << "���� JSON �ļ�ʧ��: " << json_file_full_path << std::endl;
            return false;
        }
    }
    catch (const std::exception& errpr)
    {
        std::cerr << "���л������г���: " << errpr.what() << std::endl;
        return false;
    }
}

bool excel_to_cpp(const std::string& excel_file_path, const std::string& code_path)
{
    try {
        // 1. �� Excel �ļ�����ȡ��ṹ
        xlsxioreader xlsxioread;
        if ((xlsxioread = xlsxioread_open(excel_file_path.c_str())) == NULL) {
            std::cerr << "�� Excel �ļ�ʧ��: " << excel_file_path << std::endl;
            return false;
        }

        const char* sheet_name = NULL;
        xlsxioreadersheet sheet;
        std::vector<std::string> headers, types, comments;

        // ��ȡ��һ������
        xlsxioreadersheetlist sheetlist = xlsxioread_sheetlist_open(xlsxioread);
        if ((sheet_name = xlsxioread_sheetlist_next(sheetlist)) == NULL) {
            std::cerr << "δ�ҵ�����" << std::endl;
            return false;
        }
        xlsxioread_sheetlist_close(sheetlist);

        // �򿪵�һ��������������
        sheet = xlsxioread_sheet_open(xlsxioread, sheet_name, XLSXIOREAD_SKIP_EMPTY_ROWS);

        // ��ȡ�ֶ�������һ�У�
        char* value;
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                headers.push_back(value);
                xlsxioread_free(value);
            }
        }

        // ��ȡ�������ͣ��ڶ��У�
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                types.push_back(value);
                xlsxioread_free(value);
            }
        }

        // ��ȡע�ͣ������У�
        if (xlsxioread_sheet_next_row(sheet)) {
            while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
                comments.push_back(value);
                xlsxioread_free(value);
            }
        }

        // �رձ��� Excel �ļ�
        xlsxioread_sheet_close(sheet);
        xlsxioread_close(xlsxioread);

        // 2. ��ȡģ���ļ�����
        std::ifstream header_template("cpp_h.hbs");
        std::ifstream cpp_template("cpp_cpp.hbs");
        std::stringstream header_buffer, cpp_buffer;

        header_buffer << header_template.rdbuf();
        cpp_buffer << cpp_template.rdbuf();

        std::string header_template_str = header_buffer.str();
        std::string cpp_template_str = cpp_buffer.str();

        // 3. �滻ģ����
        std::string class_name = sheet_name;
        header_template_str = std::regex_replace(header_template_str, std::regex("\\{\\{CLASS_NAME\\}\\}"), class_name);
        cpp_template_str = std::regex_replace(cpp_template_str, std::regex("\\{\\{CLASS_NAME\\}\\}"), class_name);

        // �滻 {{FIELDS}}
        std::string fields_str;
        for (size_t i = 0; i < headers.size(); ++i) {
            fields_str += "    " + types[i] + " " + headers[i] + "; // " + comments[i] + "\n";
        }
        header_template_str = std::regex_replace(header_template_str, std::regex("\\{\\{FIELDS\\}\\}"), fields_str);

        // 4. �������ɵĴ��뵽ͷ�ļ���ʵ���ļ�
        std::string header_file_path = code_path + class_name + ".h";
        std::ofstream header_file(header_file_path);
        if (header_file.is_open()) {
            header_file << header_template_str;
            header_file.close();
            std::cout << "����ͷ�ļ�: " << header_file_path << std::endl;
        }
        else {
            std::cerr << "�޷�����ͷ�ļ�: " << header_file_path << std::endl;
            return false;
        }

        std::string cpp_file_path = code_path + class_name + ".cpp";
        std::ofstream cpp_file(cpp_file_path);
        if (cpp_file.is_open()) {
            cpp_file << cpp_template_str;
            cpp_file.close();
            std::cout << "����ʵ���ļ�: " << cpp_file_path << std::endl;
        }
        else {
            std::cerr << "�޷�����ʵ���ļ�: " << cpp_file_path << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& ex) {
        std::cerr << "���� C++ ����ʱ����: " << ex.what() << std::endl;
        return false;
    }
}
