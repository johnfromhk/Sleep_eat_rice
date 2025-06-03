#include "dfs.h"
#include "stat.h"
#include <thread>
void dfs_t::multi_entry() {
	std::vector<std::thread> threads;//�洢threads��vector
	threads.reserve(max_thread);
	//�������߳�
	for (int i = 0;i < max_thread;++i) {
		threads.push_back(
			std::thread([&](int id) {
				thread_entry();
				}, i)
		);
	}

	auto sleep_thread =//��ʱ����ʱ�䵽�Ժ���
		std::thread([&](int id) {
		for (int i = max_try_time / 1000;i > 0;--i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));//˯һ��
			if (gcounter > max_try_count) giveup = 1;//��������Դ��� ����
			if (giveup || found_target || out_of_t) return;//�ҵ�Ŀ�����߳�ʱ��  ����
		}
		out_of_t = 1;//��ǳ�ʱ
			}, max_thread);
	//ͬ���߳�
	for (auto& t : threads) {
		t.join();
	}
	sleep_thread.join();
}