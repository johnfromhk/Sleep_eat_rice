#include"vec_base.h"
namespace vecbase {
	namespace base {
		//����,ȡ�Ӽ��õ�
		compvec2_father_t constmaskL[totalsize];
		compvec2_father_t constmaskR[totalsize];
		inline static constexpr compvec2_father_t calculate_mask_L(const size_t& x, const size_t& y) {//(��(0,0)��(x,y)ȫ��1
			compvec2_father_t returnvar;
			returnvar.reset();
			for (size_t i = 0; i <= x; ++i) {
				for (size_t j = 0; j <= y; ++j) {
					returnvar.set(i, j);
				}
			}
			return returnvar;
		}
		inline static constexpr compvec2_father_t calculate_mask_R(const size_t& x, const size_t& y) {//(��(x,y)��(rsize,csize)ȫ��1
			compvec2_father_t returnvar;
			returnvar.reset();
			for (size_t i = x; i < rsize; ++i) {
				for (size_t j = y; j < csize; ++j) {
					returnvar.set(i, j);
				}
			}
			return returnvar;
		}
		static void CPU_constmask_init() {
			for (size_t i = 0; i < totalsize; i++) {
				constmaskL[i] = calculate_mask_L(i_to_x(i), i_to_y(i));
				constmaskR[i] = calculate_mask_R(i_to_x(i), i_to_y(i));
			}
		}
		//��ʼ��CPU��������
		class INIT_CPU_constmask_init {
		public:
			INIT_CPU_constmask_init() {
				CPU_constmask_init();
			}
		};
		INIT_CPU_constmask_init INIT_CPU_constmask_init_;
	}
}