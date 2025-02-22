#include"tree.h"

// 计算矩阵的哈希值

std::size_t compvec_t::MatrixHash::operator()(const compvec_t& vec) const {
    std::size_t hash = 0;
    for (int i = 0;i < 4;++i) {
        hash ^= std::hash<long long>{}(vec.vec[i])+0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    //std::cout << hash<< std::endl;
    return hash;
}


// 定义矩阵相等的判断方式

bool compvec_t::operator==(const compvec_t& b) const {
    return (vec[0] == b.vec[0])
        && (vec[1] == b.vec[1])
        && (vec[2] == b.vec[2])
        ;
}

compvec_t::compvec_t(const vec_t2& vec) {
    this->vec[0] = 0;this->vec[1] = 0;this->vec[2] = 0;this->vec[3] = 0;
    //std::cout << vec;
    for (int i = 0;i < COLS;++i) {
        for (int j = 0;j < ROWS ;++j) {
            this->vec[i % 4] <<= 1;//左移1位
            this->vec[i % 4] |= (vec[j][i] > 0) ? 1 : 0;//写入最后一位
        }

    }
    //std::cout << this->vec<<std::endl;
}