#pragma once
#pragma once
//#include<set>
#include"opr.h"

namespace graph {
	

	/*class route_t {
	private:
		opr_t oprs[80];//�洢��ǰ·��
		size_t n = 0;//Ŀǰ���˼���opr
		inline static constexpr size_t nmax = 80;//�����Էż���opr
	public://���룬��ѯ��ɾ��
		inline constexpr size_t find(const opr_t& opr)const {
			for (size_t i = 0;i < n;++i) {
				if (oprs[i] == opr)
					return i;
			}
			return n;
		}
		inline constexpr void insert(const opr_t& opr) {
			if (find(opr) == n) //==n����û���ظ�
				oprs[n] = opr; ++n;
		}
		inline constexpr void erase(const opr_t& opr) {
			auto fid = find(opr);
			if (fid != n) {//�ҵ���
				oprs[fid] = oprs[n - 1];//�������һ��ֵ�Ͳ�ѯ����ֵ��Ȼ�������-1
				--n;
			}
		}
	public:
		//������
		inline constexpr auto begin() const { return &oprs[0]; }
		inline constexpr auto end() const { return &oprs[n]; }
		inline constexpr auto& operator[](const size_t& i) const { return oprs[i]; }
		inline constexpr auto& size() const { return n; }
	};

	//*/

}