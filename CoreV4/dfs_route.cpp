#include "dfs.h"
inline constexpr graph::opr_t dfs_t::get_current_opr()const {//获取当前操作符
	if (last_point != nullptr) {
		return point ^ last_point->point;
	}
	else {
		return point ^ point;
	}
}


graph::route_t dfs_t::get_route() const {
	graph::route_t route;
	route.n = deep();
	auto this_point = this;
	for (size_t i = route.n-1;i < route.n;--i) {
		route[i] = this_point->get_current_opr();//当前操作符等于两个节点取亦或(差集)
		this_point = this_point->last_point;//继续往上走
	}
	return route;
}
graph::vec2_t dfs_t::get_max_vec() {
	graph::vec2_t v(1);
	for (int i = 0;i < max_zero_route.size();++i) {
		v = v * max_zero_route[i];
	}
	return v;
}