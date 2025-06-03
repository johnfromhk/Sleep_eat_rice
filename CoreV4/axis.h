#pragma once

#ifdef __CUDACC__
//�˺���ʹ��NVCC���룬ȷ������c++20
	#define HOST_DEVICE __host__ __device__
#else
//��������ʹ��MSVC����
	#define HOST_DEVICE 
#endif
#include <cstddef>

namespace vecbase {
	//��������:��С
	static constexpr size_t rsize = 16;//��
	static constexpr size_t csize = 10;//��
	static constexpr size_t totalsize = rsize * csize;
	HOST_DEVICE inline constexpr size_t rowsize() { return rsize; }
	HOST_DEVICE inline constexpr size_t colsize() { return csize; }
	HOST_DEVICE inline constexpr size_t size() { return totalsize; }
	//����ת��
	HOST_DEVICE inline constexpr size_t xy_to_i(const size_t& x, const size_t& y) { return y * rowsize() + x; }
	HOST_DEVICE inline constexpr size_t i_to_x(const size_t& xy) { return xy % rowsize(); }
	HOST_DEVICE inline constexpr size_t i_to_y(const size_t& xy) { return xy / rowsize(); }
	//��ֵ
	HOST_DEVICE inline constexpr size_t max(const size_t& a, const size_t& b) { return a > b ? a : b; }
	HOST_DEVICE inline constexpr size_t min(const size_t& a, const size_t& b) { return a < b ? a : b; }
	//��������
	class axis_t {//���������õ��ڲ���
	private:
		size_t _xy;
	public://���캯��
		HOST_DEVICE inline constexpr axis_t() { ; }//Ĭ�Ϲ��캯����ʲôʲô�����ɣ�ע��,��ʱû�г�ʼ��
		HOST_DEVICE inline constexpr axis_t(const size_t& xy) { _xy = xy; }
		HOST_DEVICE inline constexpr axis_t(const axis_t& xy) { _xy = xy._xy; }
		HOST_DEVICE inline constexpr axis_t(const size_t& x, const size_t& y) { _xy = xy_to_i(x, y); }
	public://���ú���
		HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y) { _xy = xy_to_i(x, y); }
		//inline constexpr void operator=(const axis_t& xy) { _xy = xy._xy; }
		HOST_DEVICE inline constexpr void operator=(const size_t& xy) { _xy = xy; }
	public://��������
		HOST_DEVICE inline constexpr size_t xy() const { return _xy; }//��ȡxy����
		HOST_DEVICE inline constexpr size_t x() const { return i_to_x(_xy); }//��ȡx����
		HOST_DEVICE inline constexpr size_t y() const { return i_to_y(_xy); }//��ȡy����
	public://û�õĲ�����
		HOST_DEVICE inline constexpr bool operator<(const axis_t& b)  const { return _xy < b._xy; }
		HOST_DEVICE inline constexpr bool operator<=(const axis_t& b) const { return _xy <= b._xy; }
		HOST_DEVICE inline constexpr bool operator>(const axis_t& b)  const { return _xy > b._xy; }
		HOST_DEVICE inline constexpr bool operator>=(const axis_t& b) const { return _xy >= b._xy; }
		HOST_DEVICE inline constexpr bool operator==(const axis_t& b) const { return _xy == b._xy; }
		HOST_DEVICE inline constexpr bool operator!=(const axis_t& b) const { return _xy != b._xy; }
		HOST_DEVICE inline constexpr bool operator<(const size_t& b)  const { return _xy < b; }
		HOST_DEVICE inline constexpr bool operator<=(const size_t& b) const { return _xy <= b; }
		HOST_DEVICE inline constexpr bool operator>(const size_t& b)  const { return _xy > b; }
		HOST_DEVICE inline constexpr bool operator>=(const size_t& b) const { return _xy >= b; }
		HOST_DEVICE inline constexpr bool operator==(const size_t& b) const { return _xy == b; }
		HOST_DEVICE inline constexpr bool operator!=(const size_t& b) const { return _xy != b; }
		HOST_DEVICE inline constexpr axis_t& operator++() { ++_xy; return *this; }
		HOST_DEVICE inline constexpr axis_t operator++(int) { auto temp = _xy; ++_xy; return temp; }
		HOST_DEVICE inline constexpr axis_t& operator--() { --_xy; return *this; }
		HOST_DEVICE inline constexpr axis_t operator--(int) { auto temp = _xy; --_xy; return temp; }
	};

}