#pragma once
#include "rect.h"
#ifndef fast_vec_sumH
#define fast_vec_sumH
class prefix_t {
private:
	static const vec_t2 empty_prefix_sum;//17*11全0数组
	vec_t2 prefix_sum = empty_prefix_sum;
public:
	prefix_t(const vec_t2& vec);//计算前缀和
	int getRectangleSum(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) const;//获取前缀和
};


#endif