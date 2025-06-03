#pragma once
#include<array>
#include"vecbasebase.h"
namespace vecbase {
	namespace base{
		extern compvec2_father_t constmaskL[totalsize];
		extern compvec2_father_t constmaskR[totalsize];
			//��ʼ��GPU��������
		extern "C" void GPU_constmask_init();
	}
	
	class compvec2_t:public base::compvec2_father_t {
	public:
		HOST_DEVICE inline constexpr compvec2_t() {}//Ĭ�Ϻ�����ʲô������
		HOST_DEVICE inline constexpr ~compvec2_t() {}//Ĭ�Ϻ�����ʲô������
		HOST_DEVICE inline constexpr compvec2_t(const int& n) { //nֻ��ȡ0��1�����n=0������ȫ0�������n=1������ȫ1����
			if (n == 0)
				reset();
			else
				set();
		}
		HOST_DEVICE inline constexpr compvec2_t(const compvec2_t& b) {//ͨ����һ�������ʼ��
			for (size_t i = 0; i < WORD_SIZE; ++i) {
				v[i] = b.v[i];
			}
		}

		
	public://�߼����� ȡ�Ӽ�
#ifdef __CUDACC__
		HOST_DEVICE inline constexpr compvec2_t subset(const size_t& xy1, const size_t& xy2) const;
		HOST_DEVICE inline constexpr compvec2_t subset(const axis_t& xy1, const axis_t& xy2) const {
			return subset(xy1.xy(), xy2.xy());
		}
		HOST_DEVICE inline constexpr compvec2_t subset(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) const {
			return subset(xy_to_i(x1, y1), xy_to_i(x2, y2));
		}

#else
		HOST_DEVICE inline constexpr compvec2_t subset(const size_t& xy1, const size_t& xy2) const {
			return *this & base::constmaskR[xy1] & base::constmaskL[xy2];
		}
		HOST_DEVICE inline constexpr compvec2_t subset(const axis_t& xy1, const axis_t& xy2) const {
			return subset(xy1.xy(), xy2.xy());
		}
		HOST_DEVICE inline constexpr compvec2_t subset(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) const {
			return subset(xy_to_i(x1, y1), xy_to_i(x2, y2));
		}
		HOST_DEVICE inline static constexpr auto subsetmask(const size_t& xy1, const size_t& xy2)  {
			return base::constmaskR[xy1] & base::constmaskL[xy2];
		}
		HOST_DEVICE inline static constexpr auto subsetmask(const axis_t& xy1, const axis_t& xy2)  {
			return subsetmask(xy1.xy(), xy2.xy());
		}
		HOST_DEVICE inline static constexpr auto subsetmask(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2)  {
			return subsetmask(xy_to_i(x1, y1), xy_to_i(x2, y2));
		}

#endif
	};
}