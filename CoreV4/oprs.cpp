#include "oprs.h"
namespace graph {
	bool oprs_t::is_atleastNnointersect(const opr_t& opr, size_t N) const {
		//如果opr与与任意一个操作符不相交，返回真
		//int counter = N;
		for (size_t i = 0;i < n;++i) {
			if (!(datas[i].is_intersect(opr))) {
				--N;
				if (N == 0) return true;
			}
		}
		return false;
	}//*/
	//此函数会损毁原数组
	void oprs_t::find_max_union_set(opr_t&returnvar) {
		int bitcount[vecbase::rowsize() * vecbase::colsize()] = { 0 };//位计数器
		//对每一个bitcount的bit经行计数
		for (const auto&opr:datas) {
			for (const auto& axis : opr) {
				++bitcount[axis.xy()];
			}
		}
		int max_count = 0, max_id = 0;
		//寻找最大值
		for (int i = 0;i < vecbase::rowsize() * vecbase::colsize();++i) {
			if (bitcount[i] > max_count) {
				max_count = bitcount[i];
				max_id = i;
			}
		}
		returnvar.set(max_id);
	}
	void swap_opr(opr_t& a, opr_t& b) {
		opr_t c = a;
		a = b;
		b = c;
	}
	void oprs_t::sort_as_union_set() {
		for (int i = 0;i < size();++i) {
			opr_t uopr = (*this)[i];
			for (int j = i + 1;j < size();++j) {
				if (uopr.is_intersect((*this)[j])) {
					uopr |= (*this)[j];
					swap_opr((*this)[i + 1], (*this)[j]);
					++i;
				}
			}
		}
	}
}