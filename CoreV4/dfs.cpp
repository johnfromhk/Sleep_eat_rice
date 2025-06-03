#include "dfs.h"
#include "tocout.h"
#include"stat.h"
#include <iostream>
#include<algorithm>
#include"removedump.h"
//ȫ��֧�������⹹������
thread_local vecbase::fvector<dfs_t, 80> dfsss[80];
void dfs_t::finalize() const{
    auto current_zero = point.count();
    if (current_zero > max_zero_count) {//�ҵ����ŵ�0
        std::lock_guard<std::mutex> lock(maxlock);
        max_zero_count = current_zero;
        max_zero_route = get_route();
        if (point.count() > 100)
            std::cout << max_zero_count << "\n";
        if (current_zero > target)
            found_target = true;
        vec_at_max_zero_count = 1;
    }
    if (current_zero == max_zero_count)
        ++vec_at_max_zero_count;


#ifdef DEBUG_INFO
    if (!point.checkremains())
        bottom_counter++;//���
    else
        nbottom_counter++;//�ٵ�
    return;
#endif

}

void dfs_t::search() {
#ifdef DEBUG_INFO
    gcounter++;
    //std::cout << "��ǰ���:" << deep() <<"\n";
#endif
    if (giveup || found_target || out_of_t) return;//�ҵ�Ŀ�����߳�ʱ��  ����

    
        //
        for (const auto& opr : oprs) {
            if (matrix_set->insert(point * opr)) {
            }
            else {//�ظ����Ƴ�
                oprs.erase(opr);
#ifdef DEBUG_INFO
                dump_counter++;
#endif
            }
        }
        if (oprs.size() > 5)oprs.sort_as_union_set();
        //std::vector<dfs_t> dfss;//������һ�ڵ����Ϣ
        //dfss.reserve(oprs.size());
        auto& dfss = dfsss[deep()];
        dfss.clear();
        for (int i = 0;i < oprs.size();++i) {
            dfss.insert(dfs_t());
            dfss[i].point = point * oprs[i];//д��ڵ���Ϣ
            dfss[i].point.generate_oprs(dfss[i].oprs, oprs, oprs[i]);//���ɲ�����
            dfss[i].last_point = this;//�趨����
            dfss[i].deep_ = deep_ + 1;//�趨����Ϣ
        }


        if (oprs.size()>12) {
            //����sizeΪ0��opr������
            graph::opr_t maxopr;
            {
                graph::oprs_t zero_member;
                //��ȡ����sizeΪ0��oprs
                for (size_t i = 0;i < dfss.size();++i) {
                    if (dfss[i].oprs.newoprcount == 0)
                        zero_member.insert(dfss[i].get_current_opr());
                }
                zero_member.find_max_union_set(maxopr);

            }
            for (size_t i = dfss.size() - 1;i < dfss.size();--i) {
                if (dfss[i].oprs.newoprcount == 0
                    && maxopr.is_intersect(dfss[i].get_current_opr())
                    && oprs.is_atleastNnointersect(dfss[i].get_current_opr(), 1)
                    )
                {
                    dfss.erase(i);
                }
            }
        }
        else {
            graph::opr_t removedoprs(1);
            for (size_t i = dfss.size() - 1;i < dfss.size();--i) {
                if (dfss[i].oprs.newoprcount == 0
                    && removedoprs.is_intersect(dfss[i].get_current_opr())
                    && oprs.is_atleastNnointersect(dfss[i].get_current_opr(), 1)
                    )
                {
                    removedoprs &= (dfss[i].get_current_opr());
                    dfss.erase(i);
                }
            }
        }//*/
        /*for (size_t i = dfss.size() - 1;i < dfss.size();--i) {
            if (dfss[i].oprs.newoprcount == 0
                &&dfss.size()>2
                )
            {
                dfss.erase(dfss.begin() + i);
            }
        }*/
    //����һ�������Xa=A,��ô�����ų�X�����е�a����XYaZ��
    //��Ϊ���ݽ����ɣ�XYaZ=XaYZ=AYZ�����ظ��Ľڵ�

    //���ڽ����ɵĴ��ڣ����a����b������������ų�b�µ�a������������ڣ�
    //��XabY=XbYa=XbY1aY2
    //������a��ûb����ôb��һ��ûa��������������ģ����Կ���ֱ�ӳ���ɾ��
    //������a��b��b��ûa�����
        for (int i = 0;i < dfss.size();++i) {
            //Ҫɾ���Ĳ�����
            //const auto& ts_opr = dfss[i].current_opr();
            for (int j = i + 1;j < dfss.size();++j) {
                dfss[j].oprs.erase(dfss[i].get_current_opr());//�Ƴ�������
            }
        }
#ifdef DEBUG_INFO
        int nobottomcount = 0;
        int ti = 0;
#endif
        for (auto& t : dfss) {//�����������
#ifdef DEBUG_INFO
            int current_bottom_counter = bottom_counter;
#endif
            if (t.oprs.size() == 0
                //&& t.point.count0()>max_zero_count
                ) {
                t.finalize();
            }
            else {
                t.search();
#ifdef DEBUG_INFO
                ti++;
                //*
                if (deep() < 1) {
                    std::cout << "��ǰ��֧id:" << deep() << ":" << ti << "/" << dfss.size() << ", "
                        << t.get_current_opr() << "�ߵ���������" << bottom_counter - current_bottom_counter << "\n";
                }//*/
#endif
                if (bottom_counter == current_bottom_counter) nobottomcount++;
                //if (ti == 1 && nobottomcount > 0&&oprs.size()>5)break;
                /*
                if (nobottomcount > 0 && oprs.size() > 14) break;
                if (nobottomcount > 1 && oprs.size() > 7) break;
                if (nobottomcount > 2 && oprs.size() > 4) break;
                if (nobottomcount > 5) break;//*/
            }
        }
}



#include<cstdlib>//�����
#include<ctime>//ʱ��
int init_rand_seed() {
//#ifndef DEBUG_INFO
    std::srand(std::time(0));//�Ե�ǰʱ���趨�������
//#endif
    return 0;
}
int init_rand_seed_status = init_rand_seed();//�Ե�ǰʱ���趨�������
void dfs_t::random_entry() {
    std::srand(std::time(0));
#ifdef DEBUG_INFO
    gcounter++;
    //std::cout << "��ǰ���:" << deep() <<"\n";
#endif
    if (giveup || found_target || out_of_t) return;//�ҵ�Ŀ�����߳�ʱ��  ����


    
    auto& dfss = dfsss[deep()];
    dfss.clear();
    for (int i = 0;i < oprs.size();++i) {
        dfss.insert(dfs_t());
        dfss[i].point = point * oprs[i];//д��ڵ���Ϣ
        dfss[i].point.generate_oprs(dfss[i].oprs, oprs, oprs[i]);//���ɲ�����
        dfss[i].last_point = this;//�趨����
        dfss[i].deep_ = deep_ + 1;//�趨����Ϣ
    }

#ifdef DEBUG_INFO
    int nobottomcount = 0;
    int ti = 0;
#endif
    
    for (int i = 0;i < dfss.size();++i) {//�������
        auto& t = dfss[i];
        if (t.get_current_opr().size() == 2) {
            if (std::rand() % dfss.size() <= i) {
                if (deep() < random_entry_deep)
                    t.random_entry();
                else
                    t.search();
                break;
            }
        }
    }
}

void dfs_t::thread_entry() {
    while (1) {
        random_entry();
        if (giveup || found_target || out_of_t) return;//�ҵ�Ŀ�����߳�ʱ��  ����
    }
}