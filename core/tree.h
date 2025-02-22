#pragma once
#include <atomic>
#include"constdef.h"
#include"shared_uorder_set.h"
#include"rect.h"
#ifndef TREE_H
#define TREE_H

class tree {
private://��������
	static const vec_t2 empty_vec;//16*10ȫ0����
private://��������
	vec_t2 vec = empty_vec;//ֵ
	int deep = 0;//��ǰ���
	int current_zero = 0;
	rects_t current_oprs;
	tree* lastT = nullptr;//��һ�ڵ�
private://��ѡ·������
	rects_t rects;//���пɲ�������
private:
	void generate_oprs();//��ʼ���ɲ�����

public://��������
	vec_t2& get_vec();
	rects_t& get_rects();
	tree(std::string path);//�ļ���ʼ��
	tree(const vec_t2& vec);//�����ʼ��
	tree(tree* lastT, rect_t& opr);//������ʼ��
	~tree();
	int count_zero();//���ص�ǰ0������
	size_t size();//����rects��Ԫ������
	tree operator[](const int& id);//���ʵ�i��·��
private://���������������
	typedef shared_unordered_set<compvec_t, compvec_t::MatrixHash> dump_t;
	struct DFS_g {//�ݹ鴫�ݲ���,ȫ��֧����
		size_t counter = 0;//������
		int max_zero_count = 0;//���0������
		rects_t max_zero_operator;//���0�Ĳ���
		size_t max_zero_count_at_counter = 0;//�ҵ����0ʱ��ļ�����
		vec_t2 max_zero_vec;//���0ʱ�ľ���
	};
	static dump_t* matrix_set; //�������� ȥ����
	static std::atomic<bool> found_target;//�ҵ�Ŀ��ͱ��Ϊ1
	struct DFS_l {//�ݹ鴫�ݲ������ӷ�֧ר��
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
	void DFS1(const deep_desition& desition, DFS_g& gol, DFS_l& loc);//������ȵ����ڲ����

public:
	DFS_g DFS(const deep_desition& desition, const int& thread_id = 0);
	DFS_g MutithreadDFS(const deep_desition& desition);
};

#endif