#include"tree.h"

#include<cstdlib>//随机数
#include<ctime>//时间
//随机进入某一节点
int tree::get_min_rect_oprs_index_max() {

	for (int i = 1;i < rects.size();++i) {
		//因为rects是按item从大到小排序，如果后一个比前一个大，直接返回当前的i即可
		if (rects[i].item > rects[i - 1].item) return i;
	}
	//如果只有1或0个可操作选项，直接返回1或0即可,如果所有的大小都一样，同样返回最大size
	return rects.size();
}
tree tree::random_entry(const int& random_entry_deep) {
	if (rects.size() == 0) generate_oprs();
	std::srand(std::time(0));//以当前时间设定随机种子
	tree np = *this;//当前节点
	if (matrix_set != nullptr) {//排重表非空重复生成
		do {
			for (int i = 0;i < random_entry_deep;++i) {
				if (np.rects.size() == 0) return np;//生成操作后依旧没有可用操作，放回当前节点
				auto np1 = np[std::rand() % np.get_min_rect_oprs_index_max()];//随机在最小的操作选项中选一个
				np1.generate_oprs();//生成操作选项
				np = np1;
			}
		} while (!(matrix_set->insert(compvec_t(np.vec))));//如果重复了就重新生成一次
		return np;
	}
	else {//排重表为空则不排重直接生成
		for (int i = 0;i < random_entry_deep;++i) {
			if (np.rects.size() == 0) return np;//生成操作后依旧没有可用操作，放回当前节点
			auto np1 = np[std::rand() % np.get_min_rect_oprs_index_max()];//随机在最小的操作选项中选一个
			np1.generate_oprs();//生成操作选项
			np = np1;
		}
		return np;
	}
}