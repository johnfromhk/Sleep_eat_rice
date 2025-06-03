#include"stat.h"

#ifdef DEBUG_INFO

std::atomic<int> bottom_counter;//每次走到底+1
std::atomic<int> nbottom_counter;//每次走到底+1
std::atomic<int> dump_counter;//每次重复+1
std::atomic<int> gcounter;//每次发现一个节点+1
#endif
