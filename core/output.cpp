#include "rect.h"
using std::cout;
using std::endl;

int count_zeros(const vec_t2&vec) {
    int count = 0;
    for (auto rows : vec) {
        for (auto item : rows) {
            if (item == 0) {
                count++;
            }
        }
    }
    return count;
}

std::ostream& operator<<(std::ostream& out, const rect_t&rect) {
    cout << rect.item << "->(" << rect.x1 << ", " << rect.y1 << ", "
        << rect.x2 << ", " << rect.y2 << "),maxdeep=" << rect.max_deep << "\n";
	return out;
}
std::ostream& operator<<(std::ostream& out, const rects_t&rects) {
    int i = 0;
    for (const auto& rect : rects) {
        cout << i << ":" << rect.item << "->(" << rect.x1 << ", " << rect.y1 << ", "
            << rect.x2 << ", " << rect.y2 << "),maxdeep="<<rect.max_deep<<"\n";
        i++;
    }
	return out;
}
std::ostream& operator<<(std::ostream& out, const vec_t2 &vec) {
    int i = 1;
    cout << "0µÄ¸öÊý£º" << count_zeros(vec) << "\n";
    for (const auto& rows : vec) {
        for (const auto& item : rows)
        {
            cout << item << ",";
        }
        cout << "\n";
    }
	return out;
}

