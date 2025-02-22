#include"tree.h"

#include<cstdlib>//�����
#include<ctime>//ʱ��
//�������ĳһ�ڵ�
int tree::get_min_rect_oprs_index_max() {

	for (int i = 1;i < rects.size();++i) {
		//��Ϊrects�ǰ�item�Ӵ�С���������һ����ǰһ����ֱ�ӷ��ص�ǰ��i����
		if (rects[i].item > rects[i - 1].item) return i;
	}
	//���ֻ��1��0���ɲ���ѡ�ֱ�ӷ���1��0����,������еĴ�С��һ����ͬ���������size
	return rects.size();
}
tree tree::random_entry(const int& random_entry_deep) {
	if (rects.size() == 0) generate_oprs();
	std::srand(std::time(0));//�Ե�ǰʱ���趨�������
	tree np = *this;//��ǰ�ڵ�
	if (matrix_set != nullptr) {//���ر�ǿ��ظ�����
		do {
			for (int i = 0;i < random_entry_deep;++i) {
				if (np.rects.size() == 0) return np;//���ɲ���������û�п��ò������Żص�ǰ�ڵ�
				auto np1 = np[std::rand() % np.get_min_rect_oprs_index_max()];//�������С�Ĳ���ѡ����ѡһ��
				np1.generate_oprs();//���ɲ���ѡ��
				np = np1;
			}
		} while (!(matrix_set->insert(compvec_t(np.vec))));//����ظ��˾���������һ��
		return np;
	}
	else {//���ر�Ϊ��������ֱ������
		for (int i = 0;i < random_entry_deep;++i) {
			if (np.rects.size() == 0) return np;//���ɲ���������û�п��ò������Żص�ǰ�ڵ�
			auto np1 = np[std::rand() % np.get_min_rect_oprs_index_max()];//�������С�Ĳ���ѡ����ѡһ��
			np1.generate_oprs();//���ɲ���ѡ��
			np = np1;
		}
		return np;
	}
}