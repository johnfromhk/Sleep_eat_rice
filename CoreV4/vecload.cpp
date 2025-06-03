#include "vec.h"
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include <vector>
namespace graph {
    size_t vec2_t::vec[rowsize()][colsize()] = {0};
    void vec2_t::loadvec(const std::string& filepath) {
        std::ifstream file(filepath);  // 打开文件
        if (!file) {
            std::cerr << "无法打开文件！" << std::endl;
            exit(3);
        }

        std::vector<std::vector<int>> data;
        std::string line;

        while (std::getline(file, line)) {  // 按行读取
            std::vector<int> row;
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {  // 按逗号分割
                row.push_back(std::stoi(value));  // 转换为 int
            }
            data.push_back(row);
        }

        file.close();  // 关闭文件
        if (data.size() < rowsize())exit(2);//文件异常
        for (size_t i = 0; i < rowsize(); ++i) {
            if (data[i].size() < colsize())exit(2);//文件异常
            for (size_t j = 0; j < colsize(); ++j) {
                vec[i][j] = data[i][j];
            }
        }
        //把数据拷贝到GPU
        //GPU_loadvec();
    }
}
