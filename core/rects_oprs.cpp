#include"constdef.h"
#include"rect.h"
rect_t::rect_t(const int& x1, const int& y1, const int& x2, const int& y2) {
	this->x1 = x1;this->x2 = x2;this->y1 = y1;this->y2 = y2;
}
rect_t::rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const int& item) {
	this->x1 = x1;this->x2 = x2;this->y1 = y1;this->y2 = y2;this->item = item;
}
rect_t::rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const vec_t2& vec) {
	this->x1 = x1;this->x2 = x2;this->y1 = y1;this->y2 = y2;
	generate_info(vec);
}

/*rect_t::~rect_t() {
	if (nextT != nullptr) {
		delete nextT;
		nextT = nullptr;
	}
}*/

void rect_t::generate_info(const vec_t2& vec) {
	item = 0;
	for (int i = x1;i <= x2;++i) {
		for (int j = y1;j <= y2;++j) {
			if (vec[i][j] != 0) { 
				item++; //找到非0元素时计数器+1
				item_list.push_back({ i,j });//把找到的元素坐标放入item_list中
			}
		}
	}

}
bool axis_t::operator==(const axis_t& b) const {
	return (x == b.x) && (y == b.y); //两个坐标相等
}
bool rect_t::is_overlap(const rect_t& b) const {
	for (auto& itema : item_list) {
		for (auto& itemb : b.item_list) {
			if (itema == itemb) return true;//如果两个矩形内的元素相同，则返回相交
		}
	}
	return false;//反之，不相交
}