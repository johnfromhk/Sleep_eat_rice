#include "vec.h"
#include<fstream>
#include<iostream>
#include <string>
#include <sstream>
#include <vector>
namespace graph {
    size_t vec2_t::vec[rowsize()][colsize()] = {0};
    void vec2_t::loadvec(const std::string& filepath) {
        std::ifstream file(filepath);  // ���ļ�
        if (!file) {
            std::cerr << "�޷����ļ���" << std::endl;
            exit(3);
        }

        std::vector<std::vector<int>> data;
        std::string line;

        while (std::getline(file, line)) {  // ���ж�ȡ
            std::vector<int> row;
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {  // �����ŷָ�
                row.push_back(std::stoi(value));  // ת��Ϊ int
            }
            data.push_back(row);
        }

        file.close();  // �ر��ļ�
        if (data.size() < rowsize())exit(2);//�ļ��쳣
        for (size_t i = 0; i < rowsize(); ++i) {
            if (data[i].size() < colsize())exit(2);//�ļ��쳣
            for (size_t j = 0; j < colsize(); ++j) {
                vec[i][j] = data[i][j];
            }
        }
        //�����ݿ�����GPU
        //GPU_loadvec();
    }
}
