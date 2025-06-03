#pragma once
#pragma once
//#include<set>
#include"opr.h"

namespace graph {
	

	/*class route_t {
	private:
		opr_t oprs[80];//存储当前路径
		size_t n = 0;//目前用了几个opr
		inline static constexpr size_t nmax = 80;//最大可以放几个opr
	public://插入，查询，删除
		inline constexpr size_t find(const opr_t& opr)const {
			for (size_t i = 0;i < n;++i) {
				if (oprs[i] == opr)
					return i;
			}
			return n;
		}
		inline constexpr void insert(const opr_t& opr) {
			if (find(opr) == n) //==n代表没有重复
				oprs[n] = opr; ++n;
		}
		inline constexpr void erase(const opr_t& opr) {
			auto fid = find(opr);
			if (fid != n) {//找到了
				oprs[fid] = oprs[n - 1];//交换最后一个值和查询到的值，然后计数器-1
				--n;
			}
		}
	public:
		//迭代器
		inline constexpr auto begin() const { return &oprs[0]; }
		inline constexpr auto end() const { return &oprs[n]; }
		inline constexpr auto& operator[](const size_t& i) const { return oprs[i]; }
		inline constexpr auto& size() const { return n; }
	};

	//*/

}