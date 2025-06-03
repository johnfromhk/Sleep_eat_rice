#pragma once
#include "rect.h"
#include<concepts>
#include <bit>
#include<array>
namespace vecbase {
	struct MatrixHash;
	namespace base {
		//定义操作符的模板
		class compvec2_father_t;
		template<typename T>
		concept Basecompvec2f_t = std::is_base_of_v<compvec2_father_t, T>;
		//基础集，返回值是bool/size_t/void类型的
		class alignas(16) compvec2_father_t  {
			friend MatrixHash;
		protected://私有区域,仅供内部使用
			//压缩存储子矩阵,上取整到合适的值
			static constexpr size_t BITS_PER_WORD = sizeof(size_t) * 8;//一个WORD有多少位
			static constexpr size_t FULL_WORD = totalsize / BITS_PER_WORD;//完全利用上的WORD有几个
			static constexpr size_t REMAIN_BIT = totalsize % BITS_PER_WORD;//最后一个WORD用了几个bit
			static constexpr size_t WORD_SIZE = (totalsize + BITS_PER_WORD - 1) / BITS_PER_WORD;//一共用了几个WORD
			static constexpr size_t SIZE_T_1 = 1;//常数1
			static constexpr size_t REMAINMAX = (SIZE_T_1 << REMAIN_BIT) - 1;//尾数为1，前面没用到的部分是0
			static constexpr size_t USEDMAX = SIZE_T_1 - 2;//全1的bool值
			size_t v[WORD_SIZE];
		public://基本操作 置位,复位与查询
			HOST_DEVICE inline constexpr void set() {//把所有的值都设置成1
				for (size_t i = 0; i < FULL_WORD; ++i) {
					v[i] = USEDMAX;
				}
				if (REMAIN_BIT > 0) {
					v[FULL_WORD] = REMAINMAX;//处理尾数
				}
			};
			HOST_DEVICE inline constexpr void set(const size_t& i) { v[i / BITS_PER_WORD] |= SIZE_T_1 << (i % BITS_PER_WORD); };//把第i个值设置为1
			HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y) { set(xy_to_i(x, y)); };//把(x,y)都设置成1
			HOST_DEVICE inline constexpr void set(const size_t& i, const bool& value) { v[i / BITS_PER_WORD] &= ~(SIZE_T_1 & value) << (i % BITS_PER_WORD); };//把第i个值设置为value
			HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y, const bool& value) { set(xy_to_i(x, y), value); };//把(x,y)都设置成value
			HOST_DEVICE inline constexpr void reset() {//把所有的值都设置成0
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					v[i] = 0;
				}
			}
			HOST_DEVICE inline constexpr void reset(const size_t& i) { v[i / BITS_PER_WORD] &= ~(SIZE_T_1 << (i % BITS_PER_WORD)); }//把第i个值都设置成0
			HOST_DEVICE inline constexpr void reset(const size_t& x, const size_t& y) { reset(xy_to_i(x, y)); }//把(x,y)所有的值都设置成0
			HOST_DEVICE inline constexpr bool getbit(const size_t& i)const { return v[i / BITS_PER_WORD] >> (i % BITS_PER_WORD) & 1; }//访问第i个值
			HOST_DEVICE inline constexpr bool getbit(const axis_t& i)const { return getbit(i.xy()); }//访问第i个值
			HOST_DEVICE inline constexpr bool getbit(const size_t& x, const size_t& y) const { return getbit(xy_to_i(x, y)); }//通过x,y获取值
			HOST_DEVICE inline constexpr bool operator[](const size_t& i)const { return getbit(i); }//operator[]的语法糖
			HOST_DEVICE inline constexpr bool operator[](const axis_t& i)const { return getbit(i); }//operator[]的语法糖
			HOST_DEVICE inline constexpr auto&getinnerdata(const size_t& i)const { return v[i]; }//获取原始数据
			HOST_DEVICE inline static constexpr size_t getinnerdatasize() { return WORD_SIZE; }
		public://赋值运算符

		public://比较运算符
			HOST_DEVICE inline constexpr bool operator==(const int& b) const {
				if (b == 0) {
					for (size_t i = 0; i < WORD_SIZE; ++i) {
						if (v[i] != b)
							return false;
					}
					return true;
				}
				else {
					for (size_t i = 0; i < FULL_WORD; ++i) {
						if (v[i] != USEDMAX)
							return false;
					}
					if (REMAIN_BIT > 0) {
						if (v[FULL_WORD] != REMAINMAX)
							return false;
					}
					return true;
				}
			}
			HOST_DEVICE inline constexpr bool operator!=(const int& b) const {
				if (b == 0) {
					for (size_t i = 0; i < WORD_SIZE; ++i) {
						if (v[i] != 0)
							return true;
					}
					return false;
				}
				else {
					for (size_t i = 0; i < FULL_WORD; ++i) {
						if (v[i] != USEDMAX)
							return true;
					}
					if (REMAIN_BIT > 0) {
						if (v[FULL_WORD] != REMAINMAX)
							return true;
					}
					return false;
				}

			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator==(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] != b.v[i])
						return false;
				}
				return true;
			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator!=(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] != b.v[i])
						return true;
				}
				return false;
			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator<(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] < b.v[i]) return true;
					if (v[i] > b.v[i]) return false;
				}
				return false;
			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator>(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] > b.v[i]) return true;
					if (v[i] < b.v[i]) return false;
				}
				return false;
			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator<=(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] < b.v[i]) return true;
					if (v[i] > b.v[i]) return false;
				}
				return true;
			}
			template<Basecompvec2f_t T1>
			HOST_DEVICE inline constexpr bool operator>=(const T1& b) const {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					if (v[i] > b.v[i]) return true;
					if (v[i] < b.v[i]) return false;
				}
				return true;
			}
		public://位运算符
			//逻辑运算符
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1 operator|(const T1& a, const T2& b) {
				T1 returnvar;
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					returnvar.v[i] = a.v[i] | b.v[i];
				}
				return returnvar;
			}
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1 operator&(const T1& a, const T2& b) {
				T1 returnvar;
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					returnvar.v[i] = a.v[i] & b.v[i];
				}
				return returnvar;
			}
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1 operator^(const T1& a, const T2& b) {
				T1 returnvar;
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					returnvar.v[i] = a.v[i] ^ b.v[i];
				}
				return returnvar;
			}
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1&operator|=(T1& a, const T2& b) {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					a.v[i] |= b.v[i];
				}
				return a;
			}
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1& operator&=(T1& a, const T2& b) {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					a.v[i] &= b.v[i];
				}
				return a;
			}
			template<Basecompvec2f_t T1, Basecompvec2f_t T2 >
			HOST_DEVICE inline friend constexpr T1& operator^=(T1& a, const T2& b) {
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					a.v[i] ^= b.v[i];
				}
				return a;
			}
			template<Basecompvec2f_t T1 >
			HOST_DEVICE inline friend constexpr T1 operator~(const T1&a) {
				T1 returnvar;
				for (size_t i = 0; i < FULL_WORD; ++i) {
					returnvar.v[i] = ~a.v[i];
				}
				if (REMAIN_BIT > 0) {
					returnvar.v[FULL_WORD] = (~a.v[FULL_WORD]) & REMAINMAX;//处理尾数
				}
				return returnvar;
			}
		public://算数运算符
			template<base::Basecompvec2f_t T1 >
			HOST_DEVICE inline constexpr bool is_intersect(const T1& b)const { return (*this & b) != 0; }//判断ab是否相交（两个操作符是否互斥），如果ab相交，那么a&b!=0
			template<base::Basecompvec2f_t T1 >
			HOST_DEVICE inline constexpr bool is_subset_of(const T1& b)const { return (*this & b) == *this; }//判断a是否是b的子集(操作符是否有效，如果a是b的子集，a&b=a
		public://取差集 a-b
			template<base::Basecompvec2f_t T1, base::Basecompvec2f_t T2>
			HOST_DEVICE inline friend constexpr T1 operator-(const T1& a, const T2& b) { return a & ~b; }

		public://计算0和1的个数
			HOST_DEVICE inline constexpr size_t count1() const { //计算一共有几个1
				size_t c = 0;
				for (size_t i = 0; i < WORD_SIZE; ++i) {
#ifdef __CUDA_ARCH__
					c += __popcll(v[i]);
#else
					c += std::popcount(v[i]);
#endif
				}
				return c;
			}
			HOST_DEVICE inline constexpr size_t count0() const { return size() - count1(); }
		public://构造函数

			HOST_DEVICE inline constexpr compvec2_father_t() { }//默认函数，什么都不做
			HOST_DEVICE inline constexpr ~compvec2_father_t() { }//默认函数，什么都不做
			HOST_DEVICE inline constexpr compvec2_father_t(const int& n) { //n只能取0或1，如果n=0，创建全0矩阵，如果n=1，创建全1矩阵
				if (n == 0)
					reset();
				else
					set();
			}
			HOST_DEVICE inline constexpr compvec2_father_t(const compvec2_father_t& b) {//通过另一个对象初始化
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					v[i] = b.v[i];
				}
			}
		public:
			
		};
	}


	

}