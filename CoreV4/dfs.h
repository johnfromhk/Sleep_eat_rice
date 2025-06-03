#pragma once
#include "vec.h"
#include "shared_uorder_set.h"
#define DEBUG_INFO

class dfs_t {//迭代类
public://基本信息
	graph::vec2_t point;//当前节点
	graph::oprs_t oprs;//当前节点下的操作符
	dfs_t* last_point = nullptr;//上一节点
	graph::route_t get_route() const;//获取当前路径
	inline constexpr graph::opr_t get_current_opr()const;
public:
	inline bool operator==(const dfs_t&b)const { return point==b.point; }
public:
	size_t deep_ = 0;
	inline constexpr size_t deep()const { return deep_; }
public:
public://多线程共享变量
	static std::mutex maxlock;
	static size_t max_zero_count;//最大0的数量
	static graph::route_t max_zero_route;//最大0的操作
	static size_t vec_at_max_zero_count;//最大0的数量
	static graph::vec2_t get_max_vec();
public:
	static std::atomic<bool> found_target;//找到目标就标记为1
	static std::atomic<bool> out_of_t;//超时标记为1
	static std::atomic<bool> giveup;//放弃分支标记为1
	static int mutthread_start_deep;
public://全局目标信息
	static size_t target;//目标值
	static size_t max_thread;//最大线程数
	static size_t random_entry_deep;//最大线程数
	static size_t max_try_time;//最大线程数
	static size_t prealoc_memory;//预分配内存数
	static size_t max_try_count;//最大尝试次数
public://搜索函数
	void search();
	void finalize() const;
public:
	int fast_det() const;
public://初始话
	void init();

public:
	void random_entry();
	void thread_entry();
	void multi_entry();
};

void get_args(const std::string& filename);