#include"tocout.h"
#include <fstream>
#include <sstream>


namespace graph {
    //����������ǰѽ��д���ļ���û��Ҫ����
    void saveResult(const route_t& oprs) {
        std::ofstream outfile;
        outfile.open("result.txt"); // �򿪻򴴽��ļ�
        for (const auto& opr : oprs) {
            auto rect = opr.getrect();
            outfile << rect.x1() << "," << rect.y1() << ","
                << rect.x2() << "," << rect.y2() << "\n";
        }
        outfile.close(); // �ر��ļ�
    }
}