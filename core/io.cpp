#include "io.h"
#include <fstream>
#include <sstream>
#include<map>

//�����ļ�
int readConfigFile(const std::string& filename, std::map<std::string, std::string>&configMap) {
    std::ifstream file(filename);

    // ����ļ��Ƿ�򿪳ɹ�
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        //ȥ�����ܵĿո�
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        // ���ҵȺŵ�λ��
        size_t eqPos = line.find('=');
        //std::cout << line<<endl;
        if (eqPos != std::string::npos) {
            // ��ȡ�������Ͳ���ֵ
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            if (key == "") continue;
            // ����ֵ�Դ���map
            configMap[key] = value;
        }
    }

    file.close();
    return 0;
}
#include "tree.h"
tree::deep_desition get_args(const std::string& filename) {
    std::map<std::string, std::string> configMap;
    tree::deep_desition des;
    //* ������ж�ȡ������
    if (readConfigFile(filename, configMap) == 0) {
        for (const auto& pair : configMap) {
            //cout << ":" << pair.first << ":"<<pair.second<<":" << endl;
            if (pair.first == "max_try") {
                //cout << ":" << pair.second << ":" << endl;
                des.max_try = std::stoi(pair.second);
            }
            if (pair.first == "giveup_on_no_increase") {
                //cout << ":" << pair.second << ":" << endl;
                des.giveup_on_no_increase = std::stoi(pair.second);
            }
            if (pair.first == "target") {
                //cout << ":" << pair.second << ":" << endl;
                des.target = std::stoi(pair.second);
            }
            if (pair.first == "max_thread") {
                //cout << ":" << pair.second << ":" << endl;
                des.max_thread = std::stoi(pair.second);
            }
            if (pair.first == "random_entry_deep") {
                //cout << ":" << pair.second << ":" << endl;
                des.random_entry_deep = std::stoi(pair.second);
            }
        }
    }
    // ��¼��ʼʱ��
    std::cout << "�������:" << des.max_try << std::endl;
    std::cout << "��������������:" << des.giveup_on_no_increase << std::endl;
    std::cout << "����������:" << des.random_entry_deep << std::endl;
    std::cout << "Ŀ��ֵ:" << des.target << std::endl;
    std::cout << "����߳���:" << des.max_thread << std::endl;
    return des;
}

void saveResult(const rects_t& results) {
    std::ofstream outfile;
    outfile.open("result.txt"); // �򿪻򴴽��ļ�
    for (const auto& rect : results) {
        outfile << rect.x1 << "," << rect.y1 << ","
            << rect.x2 << "," << rect.y2 << std::endl;
    }
    outfile.close(); // �ر��ļ�
}