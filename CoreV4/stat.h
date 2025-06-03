#pragma once
#include <atomic>
#define DEBUG_INFO

#ifdef DEBUG_INFO
//统计信息
extern std::atomic<int> bottom_counter;//每次走到底+1
extern std::atomic<int> nbottom_counter;//每次走到底+1
extern std::atomic<int> dump_counter;//每次重复+1
extern std::atomic<int> gcounter;//每次发现一个节点+1
#endif
