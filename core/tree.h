#pragma once
#include <atomic>
#include"constdef.h"
#include"shared_uorder_set.h"
#include"rect.h"
#ifndef TREE_H
#define TREE_H

class tree {
private://公共区域
	static const vec_t2 empty_vec;//16*10全0数组
private://基本区域
	vec_t2 vec = empty_vec;//值
	int deep = 0;//当前深度
	int current_zero = 0;
	rects_t current_oprs;
	tree* lastT = nullptr;//上一节点
private://可选路径分类
	rects_t rects;//所有可操作矩阵
private:
	void generate_oprs();//初始化可操作项

public://基本功能
	vec_t2& get_vec();
	rects_t& get_rects();
	tree(std::string path);//文件初始化
	tree(const vec_t2& vec);//数组初始化
	tree(tree* lastT, rect_t& opr);//迭代初始化
	~tree();
	int count_zero();//返回当前0的数量
	size_t size();//返回rects的元素数量
	tree operator[](const int& id);//访问第i条路径
private://深度优先搜索区域
	typedef shared_unordered_set<compvec_t, compvec_t::MatrixHash> dump_t;
	struct DFS_g {//递归传递参数,全分支公用
		size_t counter = 0;//计数器
		int max_zero_count = 0;//最大0的数量
		rects_t max_zero_operator;//最大0的操作
		size_t max_zero_count_at_counter = 0;//找到最大0时候的计数器
		vec_t2 max_zero_vec;//最大0时的矩阵
	};
	static dump_t* matrix_set; //记忆内容 去重用
	static std::atomic<bool> found_target;//找到目标就标记为1
	struct DFS_l {//递归传递参数，子分支专用
		int min_zero_count = 160;
	};
public:
	struct deep_desition {
		int target = 125;
		int max_try = 1000000;
		int max_thread = 32;
		int random_entry_deep = 25;
		int giveup_on_no_increase = 100000;
	};
private:
	int get_min_rect_oprs_index_max();
	tree random_entry(const int& random_entry_deep);
	void DFS1(const deep_desition& desition, DFS_g& gol, DFS_l& loc);//深度优先迭代内部入口

public:
	DFS_g DFS(const deep_desition& desition, const int& thread_id = 0);
	DFS_g MutithreadDFS(const deep_desition& desition);
};

#endif