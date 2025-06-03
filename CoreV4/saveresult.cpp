#include"tocout.h"
#include <fstream>
#include <sstream>


namespace graph {
    //这个函数就是把结果写入文件，没必要解释
    void saveResult(const route_t& oprs) {
        std::ofstream outfile;
        outfile.open("result.txt"); // 打开或创建文件
        for (const auto& opr : oprs) {
            auto rect = opr.getrect();
            outfile << rect.x1() << "," << rect.y1() << ","
                << rect.x2() << "," << rect.y2() << "\n";
        }
        outfile.close(); // 关闭文件
    }
}