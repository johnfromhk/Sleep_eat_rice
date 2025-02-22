#pragma once
#include <iostream>
#include <vector>

#ifndef RECT_H
#define RECT_H
typedef std::vector<std::vector<int>> vec_t2;//2ά����
struct axis_t {
	int x = 0;
	int y = 0;
	bool operator==(const axis_t& b) const;
};
struct rect_t {//��������
	int x1;
	int y1;
	int x2;
	int y2;
	int item = -1;//Ԫ��������-1����δ��ʼ��
	int max_deep = -1;//�����ȣ�-1����δ����
	//tree* nextT = nullptr; ��¼�����еĲ�����һ�����޴����£���ᱬ�ڴ�
	std::vector<axis_t> item_list;//Ԫ������
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2);
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const int& item);
	rect_t(const int& x1, const int& y1, const int& x2, const int& y2, const vec_t2& vec);
	void generate_info(const vec_t2& vec);//����Ԫ����Ԫ��������Ԫ������
	//~rect_t();
	bool is_overlap(const rect_t& b) const;
};
struct compvec_t {
	long long int vec[4] = { 0 };//�洢ѹ���������һ��Ԫ����û���õ�
	bool operator==(const compvec_t& b) const;
	compvec_t(const vec_t2& vec);//ѹ��
	struct MatrixHash {
		std::size_t operator()(const compvec_t& vec) const;
	};
};
typedef std::vector<rect_t> rects_t;//��������

//�����
std::ostream& operator<<(std::ostream& out, const rect_t& rect);
std::ostream& operator<<(std::ostream& out, const rects_t& rects);
std::ostream& operator<<(std::ostream& out, const vec_t2& vec);
int count_zeros(const vec_t2& vec);

#endif