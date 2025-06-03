#include "dfs.h"
#include "removedump.h"
size_t dfs_t::max_zero_count=0;//最大0的数量
graph::route_t dfs_t::max_zero_route;//最大0的操作
dump_t* matrix_set=nullptr; //记忆内容 去重用
std::atomic<bool> dfs_t::found_target=0;//找到目标就标记为1
std::atomic<bool> dfs_t::out_of_t=0;//超时标记为1
std::atomic<bool> dfs_t::giveup = 0;//放弃分支标记为1
size_t dfs_t::vec_at_max_zero_count = 0;//最大0的数量
std::mutex dfs_t::maxlock;

size_t dfs_t::target=160;//目标值
size_t dfs_t::max_thread = 16;//最大线程数
size_t dfs_t::random_entry_deep = 25;//最大线程数
size_t dfs_t::max_try_time = 50000;//最大尝试时间
size_t dfs_t::prealoc_memory = 33554432 * 16;//预留的内存大小
size_t dfs_t::max_try_count = 999999999999ULL;

void dfs_t::init() {
	graph::vec2_t::loadvec("input.txt");//加载输入
	matrix_set = new
		dump_t(max_thread * 4 > 64 ? max_thread * 4 : 64, prealoc_memory);//设置线程池
		//dump_t(max_thread * 1);//设置线程池
}