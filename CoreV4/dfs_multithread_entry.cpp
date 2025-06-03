#include "dfs.h"
#include "stat.h"
#include <thread>
void dfs_t::multi_entry() {
	std::vector<std::thread> threads;//存储threads的vector
	threads.reserve(max_thread);
	//开启多线程
	for (int i = 0;i < max_thread;++i) {
		threads.push_back(
			std::thread([&](int id) {
				thread_entry();
				}, i)
		);
	}

	auto sleep_thread =//定时器，时间到以后起床
		std::thread([&](int id) {
		for (int i = max_try_time / 1000;i > 0;--i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));//睡一觉
			if (gcounter > max_try_count) giveup = 1;//超过最大尝试次数 放弃
			if (giveup || found_target || out_of_t) return;//找到目标解或者超时了  返回
		}
		out_of_t = 1;//标记超时
			}, max_thread);
	//同步线程
	for (auto& t : threads) {
		t.join();
	}
	sleep_thread.join();
}