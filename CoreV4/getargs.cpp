#include"dfs.h"
#include <fstream>
#include <sstream>
#include<map>
#include<iostream>

//�����ļ�
int readConfigFile(const std::string& filename, std::map<std::string, std::string>& configMap) {
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

void get_args(const std::string& filename) {
    std::map<std::string, std::string> configMap;
    //* ������ж�ȡ������
    if (readConfigFile(filename, configMap) == 0) {
        for (const auto& pair : configMap) {
            //cout << ":" << pair.first << ":"<<pair.second<<":" << endl;
            if (pair.first == "max_try_time") {
                //cout << ":" << pair.second << ":" << endl;
                //des.max_try = std::stoi(pair.second);
                dfs_t::max_try_time = std::stoi(pair.second);
            }
            if (pair.first == "giveup_on_no_increase") {
                //cout << ":" << pair.second << ":" << endl;
               //dfs_t::giveup_on_no_increase = std::stoi(pair.second);
            }
            if (pair.first == "target") {
                //cout << ":" << pair.second << ":" << endl;
                dfs_t::target = std::stoi(pair.second);
            }
            if (pair.first == "max_thread") {
                //cout << ":" << pair.second << ":" << endl;
                dfs_t::max_thread = std::stoi(pair.second);
            }
            if (pair.first == "random_entry_deep") {
                //cout << ":" << pair.second << ":" << endl;
                dfs_t::random_entry_deep = std::stoull(pair.second);
            }
            if (pair.first == "max_try_count") {
                //cout << ":" << pair.second << ":" << endl;
                dfs_t::max_try_count = std::stoull(pair.second);
            }
            if (pair.first == "prealoc_memory") {
                //cout << ":" << pair.second << ":" << endl;
                dfs_t::prealoc_memory = std::stoull(pair.second);
            }
        }
    }
    // ��¼��ʼʱ��
    std::cout << "Max Try Time:" << dfs_t::max_try_time << std::endl;
    //std::cout << "��������������:" << dfs_t::giveup_on_no_increase << std::endl;
    std::cout << "Random Entry Deep:" << dfs_t::random_entry_deep << std::endl;
    std::cout << "Target Score:" << dfs_t::target << std::endl;
    std::cout << "Number of Thread:" << dfs_t::max_thread << std::endl;
    std::cout << "Max Try Count:" << dfs_t::max_try_count << std::endl;
    std::cout << "Preallocated memory:" << dfs_t::prealoc_memory << std::endl;
}