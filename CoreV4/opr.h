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
		friend vec2_t;//��vec2_t ���촰
		class Iterator;
		friend Iterator;//�Ե�ǰ��ĵ��������촰
	private:
	public://��������
		HOST_DEVICE inline constexpr size_t size() const { return count1(); }//��ǰ����������
		HOST_DEVICE inline constexpr opr_t() { }//Ĭ�ϳ�ʼ���������е�ֵ������Ϊ0
		HOST_DEVICE inline constexpr opr_t(const compvec2_t& b) :compvec2_t(b) { }//ͨ��comp����
		HOST_DEVICE inline constexpr opr_t(const int& n) : compvec2_t(n) { }
	public://�����
		HOST_DEVICE inline constexpr bool operator<(const opr_t& b)const { //�ȽϷ� �����ֵ���
			return size() == b.size() ? compvec2_t::operator<(b) : size() < b.size();
		}
		//���������н����ɺͽ���ɣ�����֤�����Ƿ񻥳�/����
		HOST_DEVICE inline constexpr opr_t operator*(const opr_t& b) const { return *this | b; }
		HOST_DEVICE inline constexpr opr_t&operator*=(const opr_t& b) { return (*this) |= b; }

		//�ж������������Ƿ񻥳�
		using compvec2_t::is_intersect;
		
	private://forѭ��֧��
		class Iterator {//������ ����֧��for()ѭ��
		private:
			const opr_t* opr;
			axis_t current_value;
		public://���������
			HOST_DEVICE inline constexpr bool operator!=(const Iterator& b) const { return current_value != b.current_value; }
			HOST_DEVICE inline constexpr bool operator==(const Iterator& b) const { return current_value == b.current_value; }
			HOST_DEVICE inline constexpr Iterator&operator++() {
				for (++current_value; current_value < vecbase::size(); ++current_value) //Ѱ����һ��1��λ��
					if ((*opr)[current_value]) 
						return *this;
				//û�ҵ�,����
				return *this; 
			}
			HOST_DEVICE inline constexpr Iterator&operator--() {
				for (; current_value >= 0; --current_value) //Ѱ����һ��1��λ��
					if ((*opr)[current_value])
						return *this;
				//û�ҵ�,����
				return *this;
			}
			HOST_DEVICE inline const axis_t& operator*() const{ return current_value; }

		private:
			HOST_DEVICE inline constexpr void first() { current_value = -1; operator++(); }
		public://���캯��
			HOST_DEVICE inline constexpr Iterator(const opr_t* opr) { this->opr = opr; first(); }//����begin
			HOST_DEVICE inline constexpr Iterator(const opr_t* opr, int) { this->opr = opr; current_value = vecbase::size(); }//����end
		};
	public:
		HOST_DEVICE inline constexpr Iterator begin() const { return Iterator(this); }
		HOST_DEVICE inline constexpr Iterator end() const { return Iterator(this,0); }
	public:
		//��ȡ��������
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