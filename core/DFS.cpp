#include"tree.h"
#include <algorithm>
using std::cout;
using std::endl;
tree::dump_t* tree::matrix_set = nullptr;
std::atomic<bool> tree::found_target = false;
void tree::DFS1(const deep_desition& desition, DFS_g&gol, DFS_l&loc) {
    ++gol.counter;
    if (gol.counter > desition.max_try) return;//��������Դ���������
    if (gol.counter - gol.max_zero_count_at_counter > desition.giveup_on_no_increase)
        return;//���Թ����û���ҵ�����Ľ⣬����
    if (found_target) return;//�ҵ�Ŀ�����  ����
    if (current_zero > gol.max_zero_count) {//�ҵ����ŵ�0
        gol.max_zero_count = current_zero;
        gol.max_zero_operator = current_oprs;
        gol.max_zero_count_at_counter = gol.counter;
        gol.max_zero_vec = vec;
        if (current_zero > 100)
            std::cout << gol.max_zero_count << std::endl;
        if (current_zero > desition.target)
            found_target = true;
    }
    int fail_try = 0;//ÿ�η����ظ�����
    int nobotcount = 0;//�޷��ߵ��͵ļ���
    for (int i = 0;i < size();++i) {
        DFS_l sub_loc;
        auto nt = (*this)[i];
        if (matrix_set->insert(compvec_t(nt.vec))) {//���û���ظ���·������ִ������
            nt.generate_oprs();
            nt.DFS1(desition, gol, sub_loc);
            if (sub_loc.min_zero_count == 160)
                nobotcount++;
            else
                loc.min_zero_count = std::min(loc.min_zero_count, sub_loc.min_zero_count);
        }
        else {//�ⲿ�����������������
           fail_try++;//ÿ�η����ظ�·�������ֵ��+1
           if (fail_try > 3) break;//���ظ�·������ʱ�������÷�֧
        }
        //�ⲿ����ǳ������������
        if (nobotcount > 3) break;//�޷��ߵ��׵�·�����࣬ʣ��·���޷����׵ĸ���һ���ܸߣ�ֱ�Ӻ��Ե���ѡ����·����    
    }
    if (size() == 0) loc.min_zero_count = std::min(loc.min_zero_count, current_zero);
}
tree::DFS_g tree::DFS(const deep_desition&desition,const int&thread_id) {
    //�������� ��СֵunordersetΪ64
    DFS_g gol;
    //��ָ��ʱ�趨��ȥ�ؼ�
    if (matrix_set == nullptr) 
        matrix_set = new 
        dump_t(dump_t(desition.max_thread * 4 > 64 ? desition.max_thread * 4 : 64));
    while (1) {
        DFS_g sgol;
        sgol.counter = gol.counter;//ͬ��������
        sgol.max_zero_count_at_counter = gol.counter;
        DFS_l loc;//˽�б���
        auto re = random_entry(desition.random_entry_deep);
        re.DFS1(desition, gol, loc);
        gol.max_zero_count_at_counter = gol.counter;
        /*gol.counter = sgol.counter;
        if (sgol.max_zero_count > gol.max_zero_count) {//����ҵ����ֵ��д��
            gol.max_zero_count = sgol.max_zero_count;
            gol.max_zero_operator = sgol.max_zero_operator;
            gol.max_zero_vec = sgol.max_zero_vec;
        }*/
        //��������������ֹ����
        if (gol.counter > desition.max_try) { 
            std::cout <<"threadid:"<< thread_id << "��������Դ���,���� \n";
            break;
        }else if (found_target) {//�ҵ�Ŀ�����
            std::cout << "�ҵ�Ŀ�����,�ȴ������߳��˳���д���ļ�\n";
            break;
        }
        else {
            //std::cout <<"threadid:"<< thread_id << "�����֧�Ҳ������õĽ��ˣ�������֧�������� \n";
        }
    }
    return gol;
}

tree::DFS_g tree::MutithreadDFS(const deep_desition& desition) {

    //�趨��������
    deep_desition tdesition = desition;
    tdesition.max_try /= desition.max_thread;//Ϊÿ���ͳ�������ͬ�ĳ��Դ���
    std::vector<std::thread> threads;//�̳߳�
    std::vector <DFS_g> gols;
    gols.resize(desition.max_thread);//Ϊÿ���̷߳�������ķ���ֵ
    //�����߳�
    for (int i = 0;i < desition.max_thread;++i) {
        threads.push_back(std::thread([&](int id) {
            gols[id] = DFS(tdesition, id);
            },i)
        );
    }
    //�ȴ������̷߳���
    for (auto& thread : threads) {
        thread.join();
    }
    DFS_g returngol;
    for (auto& gol : gols) {
        if (gol.max_zero_count > returngol.max_zero_count) {
            returngol.max_zero_count = gol.max_zero_count;
            returngol.max_zero_operator = gol.max_zero_operator;
            returngol.max_zero_vec = gol.max_zero_vec;
        }
    }
    return returngol;
}