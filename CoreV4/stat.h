#pragma once
#include <atomic>
#define DEBUG_INFO

#ifdef DEBUG_INFO
//ͳ����Ϣ
extern std::atomic<int> bottom_counter;//ÿ���ߵ���+1
extern std::atomic<int> nbottom_counter;//ÿ���ߵ���+1
extern std::atomic<int> dump_counter;//ÿ���ظ�+1
extern std::atomic<int> gcounter;//ÿ�η���һ���ڵ�+1
#endif
