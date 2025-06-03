#pragma once
//#include"vec.h"
#include"vec_base.h"

namespace graph {
	using vecbase::xy_to_i;
	using vecbase::i_to_x;
	using vecbase::i_to_y;
	using vecbase::compvec2_t;
	using vecbase::rowsize;
	using vecbase::colsize;
	using vecbase::axis_t;
	using vecbase::rect_t;
	class vec2_t;
	class opr_t: public compvec2_t {
		friend vec2_t;//对vec2_t 开天窗
		class Iterator;
		friend Iterator;//对当前类的迭代器开天窗
	private:
	public://基本功能
		HOST_DEVICE inline constexpr size_t size() const { return count1(); }//当前操作的数量
		HOST_DEVICE inline constexpr opr_t() { }//默认初始化，把所有的值都设置为0
		HOST_DEVICE inline constexpr opr_t(const compvec2_t& b) :compvec2_t(b) { }//通过comp构造
		HOST_DEVICE inline constexpr opr_t(const int& n) : compvec2_t(n) { }
	public://运算符
		HOST_DEVICE inline constexpr bool operator<(const opr_t& b)const { //比较符 按照字典序
			return size() == b.size() ? compvec2_t::operator<(b) : size() < b.size();
		}
		//操作符具有交换律和结合律，不验证它们是否互斥/依赖
		HOST_DEVICE inline constexpr opr_t operator*(const opr_t& b) const { return *this | b; }
		HOST_DEVICE inline constexpr opr_t&operator*=(const opr_t& b) { return (*this) |= b; }

		//判断两个操作符是否互斥
		using compvec2_t::is_intersect;
		
	private://for循环支持
		class Iterator {//迭代器 用于支持for()循环
		private:
			const opr_t* opr;
			axis_t current_value;
		public://基本运算符
			HOST_DEVICE inline constexpr bool operator!=(const Iterator& b) const { return current_value != b.current_value; }
			HOST_DEVICE inline constexpr bool operator==(const Iterator& b) const { return current_value == b.current_value; }
			HOST_DEVICE inline constexpr Iterator&operator++() {
				for (++current_value; current_value < vecbase::size(); ++current_value) //寻找下一个1的位置
					if ((*opr)[current_value]) 
						return *this;
				//没找到,返回
				return *this; 
			}
			HOST_DEVICE inline constexpr Iterator&operator--() {
				for (; current_value >= 0; --current_value) //寻找下一个1的位置
					if ((*opr)[current_value])
						return *this;
				//没找到,返回
				return *this;
			}
			HOST_DEVICE inline const axis_t& operator*() const{ return current_value; }

		private:
			HOST_DEVICE inline constexpr void first() { current_value = -1; operator++(); }
		public://构造函数
			HOST_DEVICE inline constexpr Iterator(const opr_t* opr) { this->opr = opr; first(); }//用于begin
			HOST_DEVICE inline constexpr Iterator(const opr_t* opr, int) { this->opr = opr; current_value = vecbase::size(); }//用于end
		};
	public:
		HOST_DEVICE inline constexpr Iterator begin() const { return Iterator(this); }
		HOST_DEVICE inline constexpr Iterator end() const { return Iterator(this,0); }
	public:
		//获取矩形坐标
		HOST_DEVICE inline constexpr rect_t getrect() const {
			size_t x1 = rowsize() - 1, y1 = colsize() - 1, x2 = 0, y2 = 0;
			for (const auto&axis:*this) {
				x1 = vecbase::min(x1, axis.x());
				y1 = vecbase::min(y1, axis.y());
				x2 = vecbase::max(x2, axis.x());
				y2 = vecbase::max(y2, axis.y());
			}
			return rect_t(x1, y1, x2, y2);
		}
	};
}