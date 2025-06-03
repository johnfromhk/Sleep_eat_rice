#pragma once
#include "vec.h"
#include "shared_uorder_set.h"
#define DEBUG_INFO

class dfs_t {//������
public://������Ϣ
	graph::vec2_t point;//��ǰ�ڵ�
	graph::oprs_t oprs;//��ǰ�ڵ��µĲ�����
	dfs_t* last_point = nullptr;//��һ�ڵ�
	graph::route_t get_route() const;//��ȡ��ǰ·��
	inline constexpr graph::opr_t get_current_opr()const;
public:
	inline bool operator==(const dfs_t&b)const { return point==b.point; }
public:
	size_t deep_ = 0;
	inline constexpr size_t deep()const { return deep_; }
public:
public://���̹߳������
	static std::mutex maxlock;
	static size_t max_zero_count;//���0������
	static graph::route_t max_zero_route;//���0�Ĳ���
	static size_t vec_at_max_zero_count;//���0������
	static graph::vec2_t get_max_vec();
public:
	static std::atomic<bool> found_target;//�ҵ�Ŀ��ͱ��Ϊ1
	static std::atomic<bool> out_of_t;//��ʱ���Ϊ1
	static std::atomic<bool> giveup;//������֧���Ϊ1
	static int mutthread_start_deep;
public://ȫ��Ŀ����Ϣ
	static size_t target;//Ŀ��ֵ
	static size_t max_thread;//����߳���
	static size_t random_entry_deep;//����߳���
	static size_t max_try_time;//����߳���
	static size_t prealoc_memory;//Ԥ�����ڴ���
	static size_t max_try_count;//����Դ���
public://��������
	void search();
	void finalize() const;
public:
	int fast_det() const;
public://��ʼ��
	void init();

public:
	void random_entry();
	void thread_entry();
	void multi_entry();
};

void get_args(const std::string& filename);