#pragma once

#ifdef __CUDACC__
//核函数使用NVCC编译，确保启用c++20
	#define HOST_DEVICE __host__ __device__
#else
//主机函数使用MSVC编译
	#define HOST_DEVICE 
#endif
#include <cstddef>

namespace vecbase {
	//基本操作:大小
	static constexpr size_t rsize = 16;//行
	static constexpr size_t csize = 10;//列
	static constexpr size_t totalsize = rsize * csize;
	HOST_DEVICE inline constexpr size_t rowsize() { return rsize; }
	HOST_DEVICE inline constexpr size_t colsize() { return csize; }
	HOST_DEVICE inline constexpr size_t size() { return totalsize; }
	//坐标转换
	HOST_DEVICE inline constexpr size_t xy_to_i(const size_t& x, const size_t& y) { return y * rowsize() + x; }
	HOST_DEVICE inline constexpr size_t i_to_x(const size_t& xy) { return xy % rowsize(); }
	HOST_DEVICE inline constexpr size_t i_to_y(const size_t& xy) { return xy / rowsize(); }
	//最值
	HOST_DEVICE inline constexpr size_t max(const size_t& a, const size_t& b) { return a > b ? a : b; }
	HOST_DEVICE inline constexpr size_t min(const size_t& a, const size_t& b) { return a < b ? a : b; }
	//基本坐标
	class axis_t {//返回坐标用的内部类
	private:
		size_t _xy;
	public://构造函数
		HOST_DEVICE inline constexpr axis_t() { ; }//默认构造函数，什么什么都不干，注意,此时没有初始化
		HOST_DEVICE inline constexpr axis_t(const size_t& xy) { _xy = xy; }
		HOST_DEVICE inline constexpr axis_t(const axis_t& xy) { _xy = xy._xy; }
		HOST_DEVICE inline constexpr axis_t(const size_t& x, const size_t& y) { _xy = xy_to_i(x, y); }
	public://设置函数
		HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y) { _xy = xy_to_i(x, y); }
		//inline constexpr void operator=(const axis_t& xy) { _xy = xy._xy; }
		HOST_DEVICE inline constexpr void operator=(const size_t& xy) { _xy = xy; }
	public://基本操作
		HOST_DEVICE inline constexpr size_t xy() const { return _xy; }//获取xy坐标
		HOST_DEVICE inline constexpr size_t x() const { return i_to_x(_xy); }//获取x坐标
		HOST_DEVICE inline constexpr size_t y() const { return i_to_y(_xy); }//获取y坐标
	public://没用的操作符
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