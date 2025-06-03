#pragma once
#include"axis.h"
namespace vecbase {

	//矩形坐标
	class rect_t {
	protected:
		axis_t xy1_;//左上角
		axis_t xy2_;//右下角
	public:
		HOST_DEVICE inline constexpr rect_t() { ; }//默认构造函数，没有初始化，以下操作没有任何意义
		HOST_DEVICE inline constexpr rect_t(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) { xy1_.set(x1, y1); xy2_.set(x2, y2); }
		HOST_DEVICE inline constexpr rect_t(const size_t& xy1_, const size_t& xy2_) { this->xy1_ = xy1_; this->xy2_ = xy2_; }
		HOST_DEVICE inline constexpr rect_t(const axis_t& xy1_, const axis_t& xy2_) { this->xy1_ = xy1_; this->xy2_ = xy2_; }
		HOST_DEVICE inline constexpr void set(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) { xy1_.set(x1, y1); xy2_.set(x2, y2); }
		HOST_DEVICE inline constexpr void set(const size_t& xy1_, const size_t& xy2_) { this->xy1_ = xy1_; this->xy2_ = xy2_; }
		HOST_DEVICE inline constexpr void set(const axis_t& xy1_, const axis_t& xy2_) { this->xy1_ = xy1_; this->xy2_ = xy2_; }
	public://获取坐标
		HOST_DEVICE inline constexpr size_t x1()const { return xy1_.x(); }
		HOST_DEVICE inline constexpr size_t x2()const { return xy2_.x(); }
		HOST_DEVICE inline constexpr size_t y1()const { return xy1_.y(); }
		HOST_DEVICE inline constexpr size_t y2()const { return xy2_.y(); }
		HOST_DEVICE inline constexpr axis_t xy1()const { return xy1_; }
		HOST_DEVICE inline constexpr axis_t xy2()const { return xy2_; }
	};
}