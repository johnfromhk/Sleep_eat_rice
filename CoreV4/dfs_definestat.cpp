#include "dfs.h"
#include "removedump.h"
size_t dfs_t::max_zero_count=0;//���0������
graph::route_t dfs_t::max_zero_route;//���0�Ĳ���
dump_t* matrix_set=nullptr; //�������� ȥ����
std::atomic<bool> dfs_t::found_target=0;//�ҵ�Ŀ��ͱ��Ϊ1
std::atomic<bool> dfs_t::out_of_t=0;//��ʱ���Ϊ1
std::atomic<bool> dfs_t::giveup = 0;//������֧���Ϊ1
size_t dfs_t::vec_at_max_zero_count = 0;//���0������
std::mutex dfs_t::maxlock;

size_t dfs_t::target=160;//Ŀ��ֵ
size_t dfs_t::max_thread = 16;//����߳���
size_t dfs_t::random_entry_deep = 25;//����߳���
size_t dfs_t::max_try_time = 50000;//�����ʱ��
size_t dfs_t::prealoc_memory = 33554432 * 16;//Ԥ�����ڴ��С
size_t dfs_t::max_try_count = 999999999999ULL;

void dfs_t::init() {
	graph::vec2_t::loadvec("input.txt");//��������
	matrix_set = new
		dump_t(max_thread * 4 > 64 ? max_thread * 4 : 64, prealoc_memory);//�����̳߳�
		//dump_t(max_thread * 1);//�����̳߳�
}