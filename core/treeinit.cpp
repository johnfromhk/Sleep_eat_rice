#include"tree.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
using namespace std;


//��ʼ����������
const vec_t2 tree::empty_vec(ROWS, std::vector<int>(COLS, 0));

tree::tree(std::string filepath) {
    std::ifstream file(filepath);  // ���ļ�
    if (!file) {
        std::cerr << "�޷����ļ���" << std::endl;
        exit(1);
    }

    vec_t2 data;
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
    vec = data;
    generate_oprs();
    this->current_zero = count_zeros(vec);
}

tree::tree(const vec_t2&vec) {
    this->vec = vec;
    this->deep = 0;
    this->current_zero = count_zeros(vec);
    generate_oprs();
}

tree::tree(tree* lastT, rect_t& rect) {
    this->lastT = lastT;
    //rect.nextT = this;
    current_oprs = lastT->current_oprs;
    current_oprs.push_back(rect);
    vec = lastT->vec;
    deep = lastT->deep+1;
    //�������
#define check checksum
#ifdef check
    int check = 0;
#endif
    for (int i = rect.x1;i <= rect.x2;++i) {
        for (int j = rect.y1;j <= rect.y2;++j) {
#ifdef check
            check += vec[i][j];
#endif
            vec[i][j] = 0;
        }
    }
#ifdef check
    if (check != 10) {
        exit(1);
    }
#endif
    //current_zero += rect.item;//0�ĸ���ֱ�Ӽ��ϲ�����
    current_zero = count_zeros(vec);
    //generate_oprs();
}


tree::~tree() {
    
}

