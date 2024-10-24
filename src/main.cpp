#include <iostream>
#include <string>
#include <fstream>  // �ļ���
#include <filesystem>   // c++17���룬�����ļ��м����ļ���
// ��������
#include <xlsxio_read.h> // ����excel�ļ�
#include <nlohmann/json.hpp>    // ����json�ļ�

// ����excel�����������л�Ϊjson
// ������excel��·��������json�ļ�·��
bool to_json(const std::string& excel_file_path, const std::string& json_path);

// תc++
bool to_cpp(const std::string& excel_file_path, const std::string& code_path);

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
            to_json(excel_file, path_json_out); // ת��Ϊjson�ļ�

            if (code_mode == "cpp")
            {
                to_cpp(excel_file, path_code_out);
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

bool to_json(const std::string& excel_file_path, const std::string& json_path)
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

bool to_cpp(const std::string& excel_file_path, const std::string& code_path)
{
    // �� .xlsx �ļ�
    xlsxioreader xlsxioread;
    if ((xlsxioread = xlsxioread_open(excel_file_path.c_str())) == NULL) {
        std::cerr << "�޷��� Excel �ļ�: " << excel_file_path << std::endl;
        return false;
    }

    // ��ȡ��һ������
    const char* sheetname = NULL;
    xlsxioreadersheetlist sheetlist;
    if ((sheetlist = xlsxioread_sheetlist_open(xlsxioread)) != NULL) {
        sheetname = xlsxioread_sheetlist_next(sheetlist);
        xlsxioread_sheetlist_close(sheetlist);
    }
    if (sheetname == NULL) {
        std::cerr << "�޷���ȡ������" << std::endl;
        xlsxioread_close(xlsxioread);
        return false;
    }

    // �򿪵�һ����
    xlsxioreadersheet sheet;
    if ((sheet = xlsxioread_sheet_open(xlsxioread, sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS)) == NULL) {
        std::cerr << "�޷��򿪱�: " << sheetname << std::endl;
        xlsxioread_close(xlsxioread);
        return false;
    }

    // ׼���������洢�ֶ���������
    std::vector<std::string> field_names;
    std::vector<std::string> field_types;

    // ��ȡ������
    char* value;
    int row_index = 0;

    // ��ȡ��һ�У��ֶ�����
    if (xlsxioread_sheet_next_row(sheet)) {
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
            field_names.push_back(value);
            xlsxioread_free(value);
        }
        row_index++;
    }

    // ��ȡ�ڶ��У��ֶ����ͣ�
    if (xlsxioread_sheet_next_row(sheet)) {
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
            field_types.push_back(value);
            xlsxioread_free(value);
        }
        row_index++;
    }

    // ����ֶ������ֶ����������Ƿ�ƥ��
    if (field_names.size() != field_types.size()) {
        std::cerr << "�ֶ������ֶ�����������ƥ��" << std::endl;
        xlsxioread_sheet_close(sheet);
        xlsxioread_close(xlsxioread);
        return false;
    }

    // �رձ��� Excel �ļ�
    xlsxioread_sheet_close(sheet);
    xlsxioread_close(xlsxioread);

    // ������������������Ϊ����������ĸ��д
    std::string class_name = sheetname;
    class_name[0] = toupper(class_name[0]);

    // ��������Ĵ����ļ�·��
    std::string code_file_path = code_path + class_name + ".h";

    // ���ļ���
    std::ofstream outfile(code_file_path);
    if (!outfile.is_open()) {
        std::cerr << "�޷������ļ�: " << code_file_path << std::endl;
        return false;
    }

    // д�� C++ ��ģ�嵽�ļ�
    outfile << "#ifndef " << class_name << "_H\n";
    outfile << "#define " << class_name << "_H\n\n";
    outfile << "#include <string>\n\n";
    outfile << "class " << class_name << " {\n";
    outfile << "public:\n";
    outfile << "    // ���캯��\n";
    outfile << "    " << class_name << "() = default;\n\n";
    outfile << "    // ��Ա����\n";

    // д���Ա����
    for (size_t i = 0; i < field_names.size(); ++i) {
        outfile << "    " << field_types[i] << " " << field_names[i] << ";\n";
    }

    outfile << "\n};\n\n";
    outfile << "#endif //" << class_name << "_H\n";

    // �ر��ļ���
    outfile.close();

    std::cout << "C++ �ඨ���ѳɹ�����: " << code_file_path << std::endl;
    return true;
}
