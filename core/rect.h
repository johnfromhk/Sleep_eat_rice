#pragma once
#include <iostream>
#include <vector>

#ifndef RECT_H
#define RECT_H
typedef std::vector<std::vector<int>> vec_t2;//2维数组
struct axis_t {
	int x = 0;
	int y = 0;
	bool operator==(const axis_t& b) const;
};
struct rect_t {//矩形坐标
	int x1;
	int y1;
	int x2;
	int y2;
	int item = -1;//元素数量，-1代表未初始化
	int max_deep = -1;//最大深度，-1代表未搜索
	//tree* nextT = nullptr; 记录下所有的操作是一件很愚蠢的事，这会爆内存
	std::vector<axis_t> item_list;//元素坐标
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2);
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const int& item);
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const vec_t2& vec);
	void generate_info(const vec_t2& vec);//计算元素内元素数量和元素坐标
	//~rect_t();
	bool is_overlap(const rect_t& b) const;
};
struct compvec_t {
	long long int vec[4] = { 0 };//存储压缩矩阵，最后一个元素是没有用的
	bool operator==(const compvec_t& b) const;
	compvec_t(const vec_t2& vec);//压缩
	struct MatrixHash {
		std::size_t operator()(const compvec_t& vec) const;
	};
};
typedef std::vector<rect_t> rects_t;//矩形数组

//输出流
std::ostream& operator<<(std::ostream& out, const rect_t& rect);
std::ostream& operator<<(std::ostream& out, const rects_t& rects);
std::ostream& operator<<(std::ostream& out, const vec_t2& vec);
int count_zeros(const vec_t2& vec);

#endif