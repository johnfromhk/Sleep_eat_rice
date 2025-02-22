#include "io.h"
#include <fstream>
#include <sstream>
#include<map>

//配置文件
int readConfigFile(const std::string& filename, std::map<std::string, std::string>&configMap) {
    std::ifstream file(filename);

    // 检查文件是否打开成功
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(file, line)) {
        //去除可能的空格
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        // 查找等号的位置
        size_t eqPos = line.find('=');
        //std::cout << line<<endl;
        if (eqPos != std::string::npos) {
            // 提取参数名和参数值
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            if (key == "") continue;
            // 将键值对存入map
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
    //* 输出所有读取的配置
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
    // 记录开始时间
    std::cout << "最大尝试数:" << des.max_try << std::endl;
    std::cout << "无增长放弃分数:" << des.giveup_on_no_increase << std::endl;
    std::cout << "随机进入深度:" << des.random_entry_deep << std::endl;
    std::cout << "目标值:" << des.target << std::endl;
    std::cout << "最大线程数:" << des.max_thread << std::endl;
    return des;
}

void saveResult(const rects_t& results) {
    std::ofstream outfile;
    outfile.open("result.txt"); // 打开或创建文件
    for (const auto& rect : results) {
        outfile << rect.x1 << "," << rect.y1 << ","
            << rect.x2 << "," << rect.y2 << std::endl;
    }
    outfile.close(); // 关闭文件
}