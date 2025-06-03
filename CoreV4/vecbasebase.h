#pragma once
#include "rect.h"
#include<concepts>
#include <bit>
#include<array>
namespace vecbase {
	struct MatrixHash;
	namespace base {
		//�����������ģ��
		class compvec2_father_t;
		template<typename T>
		concept Basecompvec2f_t = std::is_base_of_v<compvec2_father_t, T>;
		//������������ֵ��bool/size_t/void���͵�
		class alignas(16) compvec2_father_t  {
			friend MatrixHash;
		protected://˽������,�����ڲ�ʹ��
			//ѹ���洢�Ӿ���,��ȡ�������ʵ�ֵ
			static constexpr size_t BITS_PER_WORD = sizeof(size_t) * 8;//һ��WORD�ж���λ
			static constexpr size_t FULL_WORD = totalsize / BITS_PER_WORD;//��ȫ�����ϵ�WORD�м���
			static constexpr size_t REMAIN_BIT = totalsize % BITS_PER_WORD;//���һ��WORD���˼���bit
			static constexpr size_t WORD_SIZE = (totalsize + BITS_PER_WORD - 1) / BITS_PER_WORD;//һ�����˼���WORD
			static constexpr size_t SIZE_T_1 = 1;//����1
			static constexpr size_t REMAINMAX = (SIZE_T_1 << REMAIN_BIT) - 1;//β��Ϊ1��ǰ��û�õ��Ĳ�����0
			static constexpr size_t USEDMAX = SIZE_T_1 - 2;//ȫ1��boolֵ
			size_t v[WORD_SIZE];
		public://�������� ��λ,��λ���ѯ
			HOST_DEVICE inline constexpr void set() {//�����е�ֵ�����ó�1
				for (size_t i = 0; i < FULL_WORD; ++i) {
					v[i] = USEDMAX;
				}
				if (REMAIN_BIT > 0) {
					v[FULL_WORD] = REMAINMAX;//����β��
				}
			};
			HOST_DEVICE inline constexpr void set(const size_t& i) { v[i / BITS_PER_WORD] |= SIZE_T_1 << (i % BITS_PER_WORD); };//�ѵ�i��ֵ����Ϊ1
			HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y) { set(xy_to_i(x, y)); };//��(x,y)�����ó�1
			HOST_DEVICE inline constexpr void set(const size_t& i, const bool& value) { v[i / BITS_PER_WORD] &= ~(SIZE_T_1 & value) << (i % BITS_PER_WORD); };//�ѵ�i��ֵ����Ϊvalue
			HOST_DEVICE inline constexpr void set(const size_t& x, const size_t& y, const bool& value) { set(xy_to_i(x, y), value); };//��(x,y)�����ó�value
			HOST_DEVICE inline constexpr void reset() {//�����е�ֵ�����ó�0
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					v[i] = 0;
				}
			}
			HOST_DEVICE inline constexpr void reset(const size_t& i) { v[i / BITS_PER_WORD] &= ~(SIZE_T_1 << (i % BITS_PER_WORD)); }//�ѵ�i��ֵ�����ó�0
			HOST_DEVICE inline constexpr void reset(const size_t& x, const size_t& y) { reset(xy_to_i(x, y)); }//��(x,y)���е�ֵ�����ó�0
			HOST_DEVICE inline constexpr bool getbit(const size_t& i)const { return v[i / BITS_PER_WORD] >> (i % BITS_PER_WORD) & 1; }//���ʵ�i��ֵ
			HOST_DEVICE inline constexpr bool getbit(const axis_t& i)const { return getbit(i.xy()); }//���ʵ�i��ֵ
			HOST_DEVICE inline constexpr bool getbit(const size_t& x, const size_t& y) const { return getbit(xy_to_i(x, y)); }//ͨ��x,y��ȡֵ
			HOST_DEVICE inline constexpr bool operator[](const size_t& i)const { return getbit(i); }//operator[]���﷨��
			HOST_DEVICE inline constexpr bool operator[](const axis_t& i)const { return getbit(i); }//operator[]���﷨��
			HOST_DEVICE inline constexpr auto&getinnerdata(const size_t& i)const { return v[i]; }//��ȡԭʼ����
			HOST_DEVICE inline static constexpr size_t getinnerdatasize() { return WORD_SIZE; }
		public://��ֵ�����

		public://�Ƚ������
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
		public://λ�����
			//�߼������
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
					returnvar.v[FULL_WORD] = (~a.v[FULL_WORD]) & REMAINMAX;//����β��
				}
				return returnvar;
			}
		public://���������
			template<base::Basecompvec2f_t T1 >
			HOST_DEVICE inline constexpr bool is_intersect(const T1& b)const { return (*this & b) != 0; }//�ж�ab�Ƿ��ཻ�������������Ƿ񻥳⣩�����ab�ཻ����ôa&b!=0
			template<base::Basecompvec2f_t T1 >
			HOST_DEVICE inline constexpr bool is_subset_of(const T1& b)const { return (*this & b) == *this; }//�ж�a�Ƿ���b���Ӽ�(�������Ƿ���Ч�����a��b���Ӽ���a&b=a
		public://ȡ� a-b
			template<base::Basecompvec2f_t T1, base::Basecompvec2f_t T2>
			HOST_DEVICE inline friend constexpr T1 operator-(const T1& a, const T2& b) { return a & ~b; }

		public://����0��1�ĸ���
			HOST_DEVICE inline constexpr size_t count1() const { //����һ���м���1
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
		public://���캯��

			HOST_DEVICE inline constexpr compvec2_father_t() { }//Ĭ�Ϻ�����ʲô������
			HOST_DEVICE inline constexpr ~compvec2_father_t() { }//Ĭ�Ϻ�����ʲô������
			HOST_DEVICE inline constexpr compvec2_father_t(const int& n) { //nֻ��ȡ0��1�����n=0������ȫ0�������n=1������ȫ1����
				if (n == 0)
					reset();
				else
					set();
			}
			HOST_DEVICE inline constexpr compvec2_father_t(const compvec2_father_t& b) {//ͨ����һ�������ʼ��
				for (size_t i = 0; i < WORD_SIZE; ++i) {
					v[i] = b.v[i];
				}
			}
		public:
			
		};
	}


	

}