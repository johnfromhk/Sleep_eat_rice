#pragma once
//#include<set>
#include"opr.h"
#include "fvector.h"
#include <vector>
namespace graph {
	typedef vecbase::fvector<opr_t, 80> oprs_based_t;
	typedef vecbase::fvector<vec2_t, 80> vec2s_t;
	typedef vecbase::fvector<opr_t, 80> route_t;
	
	class oprs_t :public oprs_based_t {
	public:
		int newoprcount = 0;
		HOST_DEVICE inline constexpr void insert_old(const opr_t& opr) {
			insert(opr);
		}
		HOST_DEVICE inline void insert_new(const opr_t& opr) {
			insert(opr);
			newoprcount++;
		}


	public:
		HOST_DEVICE bool is_atleastNnointersect(const opr_t& opr, size_t N) const;
	public:
		HOST_DEVICE void find_max_union_set(opr_t&maxopr);
	public:
		HOST_DEVICE void sort_as_union_set();
	};
}