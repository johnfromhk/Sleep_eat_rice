#include "dfs.h"
#include "tocout.h"
#include"stat.h"
#include <iostream>
#include<algorithm>
#include"removedump.h"
//全分支共享，避免构造析构
thread_local vecbase::fvector<dfs_t, 80> dfsss[80];
void dfs_t::finalize() const{
    auto current_zero = point.count();
    if (current_zero > max_zero_count) {//找到更优的0
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
        bottom_counter++;//真底
    else
        nbottom_counter++;//假底
    return;
#endif

}

void dfs_t::search() {
#ifdef DEBUG_INFO
    gcounter++;
    //std::cout << "当前深度:" << deep() <<"\n";
#endif
    if (giveup || found_target || out_of_t) return;//找到目标解或者超时了  返回

    
        //
        for (const auto& opr : oprs) {
            if (matrix_set->insert(point * opr)) {
            }
            else {//重复则移除
                oprs.erase(opr);
#ifdef DEBUG_INFO
                dump_counter++;
#endif
            }
        }
        if (oprs.size() > 5)oprs.sort_as_union_set();
        //std::vector<dfs_t> dfss;//进入下一节点的信息
        //dfss.reserve(oprs.size());
        auto& dfss = dfsss[deep()];
        dfss.clear();
        for (int i = 0;i < oprs.size();++i) {
            dfss.insert(dfs_t());
            dfss[i].point = point * oprs[i];//写入节点信息
            dfss[i].point.generate_oprs(dfss[i].oprs, oprs, oprs[i]);//生成操作符
            dfss[i].last_point = this;//设定链表
            dfss[i].deep_ = deep_ + 1;//设定层信息
        }


        if (oprs.size()>12) {
            //计算size为0的opr的数量
            graph::opr_t maxopr;
            {
                graph::oprs_t zero_member;
                //获取所有size为0的oprs
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
    //更进一步，如果Xa=A,那么可以排除X下所有的a，即XYaZ。
    //因为根据交换律，XYaZ=XaYZ=AYZ，是重复的节点

    //由于交换律的存在，如果a下有b操作，则可以排除b下的a操作（如果存在）
    //即XabY=XbYa=XbY1aY2
    //如果如果a下没b，那么b下一定没a，这两操作互斥的，所以可以直接尝试删除
    //不存在a下b，b下没a的情况
        for (int i = 0;i < dfss.size();++i) {
            //要删除的操作符
            //const auto& ts_opr = dfss[i].current_opr();
            for (int j = i + 1;j < dfss.size();++j) {
                dfss[j].oprs.erase(dfss[i].get_current_opr());//移除操作符
            }
        }
#ifdef DEBUG_INFO
        int nobottomcount = 0;
        int ti = 0;
#endif
        for (auto& t : dfss) {//深度优先搜索
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
                    std::cout << "当前分支id:" << deep() << ":" << ti << "/" << dfss.size() << ", "
                        << t.get_current_opr() << "走到底数量：" << bottom_counter - current_bottom_counter << "\n";
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



#include<cstdlib>//随机数
#include<ctime>//时间
int init_rand_seed() {
//#ifndef DEBUG_INFO
    std::srand(std::time(0));//以当前时间设定随机种子
//#endif
    return 0;
}
int init_rand_seed_status = init_rand_seed();//以当前时间设定随机种子
void dfs_t::random_entry() {
    std::srand(std::time(0));
#ifdef DEBUG_INFO
    gcounter++;
    //std::cout << "当前深度:" << deep() <<"\n";
#endif
    if (giveup || found_target || out_of_t) return;//找到目标解或者超时了  返回


    
    auto& dfss = dfsss[deep()];
    dfss.clear();
    for (int i = 0;i < oprs.size();++i) {
        dfss.insert(dfs_t());
        dfss[i].point = point * oprs[i];//写入节点信息
        dfss[i].point.generate_oprs(dfss[i].oprs, oprs, oprs[i]);//生成操作符
        dfss[i].last_point = this;//设定链表
        dfss[i].deep_ = deep_ + 1;//设定层信息
    }

#ifdef DEBUG_INFO
    int nobottomcount = 0;
    int ti = 0;
#endif
    
    for (int i = 0;i < dfss.size();++i) {//随机进入
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
        if (giveup || found_target || out_of_t) return;//找到目标解或者超时了  返回
    }
}