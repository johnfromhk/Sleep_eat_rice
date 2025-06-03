#include "dfs.h"
#include <iostream>
int dfs_t::fast_det() const{
	graph::opr_t allopr;
	graph::oprs_t returnopr;
	for (const auto& opr : oprs) {
		returnopr.insert(opr);
	}
	graph::vec2_t vecmax = point;
	//�����в������ں�һ��
	while (1) {
		allopr = 0;
		for (const auto& opr : returnopr) {
			allopr *= opr;
		}
		vecmax = vecmax * allopr;
		returnopr.clear();
		vecmax.generate_newoprs(returnopr, allopr);
		if (returnopr.size() == 0) {
			if (vecmax.checkremains()) return 0;//�����������������0
			else return 160;//vecmax.count0();//��֮����0������
			//std::cout << vecmax.count0() <<"\n";
			//return vecmax.count0();
		}
	}
}