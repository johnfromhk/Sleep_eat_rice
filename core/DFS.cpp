#include"tree.h"
#include <algorithm>
using std::cout;
using std::endl;
tree::dump_t* tree::matrix_set = nullptr;
std::atomic<bool> tree::found_target = false;
void tree::DFS1(const deep_desition& desition, DFS_g&gol, DFS_l&loc) {
    ++gol.counter;
    if (gol.counter > desition.max_try) return;//超过最大尝试次数，返回
    if (gol.counter - gol.max_zero_count_at_counter > desition.giveup_on_no_increase)
        return;//尝试过多次没有找到新镇的解，返回
    if (found_target) return;//找到目标解了  返回
    if (current_zero > gol.max_zero_count) {//找到更优的0
        gol.max_zero_count = current_zero;
        gol.max_zero_operator = current_oprs;
        gol.max_zero_count_at_counter = gol.counter;
        gol.max_zero_vec = vec;
        if (current_zero > 100)
            std::cout << gol.max_zero_count << std::endl;
        if (current_zero > desition.target)
            found_target = true;
    }
    int fail_try = 0;//每次发现重复计数
    int nobotcount = 0;//无法走到低的计数
    for (int i = 0;i < size();++i) {
        DFS_l sub_loc;
        auto nt = (*this)[i];
        if (matrix_set->insert(compvec_t(nt.vec))) {//如果没有重复的路径，就执行搜索
            nt.generate_oprs();
            nt.DFS1(desition, gol, sub_loc);
            if (sub_loc.min_zero_count == 160)
                nobotcount++;
            else
                loc.min_zero_count = std::min(loc.min_zero_count, sub_loc.min_zero_count);
        }
        else {//这部分在深层会起更大作用
           fail_try++;//每次发现重复路径，这个值就+1
           if (fail_try > 3) break;//当重复路劲过多时，放弃该分支
        }
        //这部分在浅层会起更大作用
        if (nobotcount > 3) break;//无法走到底的路径过多，剩余路径无法到底的概率一样很高，直接忽略掉，选择别的路径走    
    }
    if (size() == 0) loc.min_zero_count = std::min(loc.min_zero_count, current_zero);
}
tree::DFS_g tree::DFS(const deep_desition&desition,const int&thread_id) {
    //公共变量 最小值unorderset为64
    DFS_g gol;
    //空指针时设定新去重集
    if (matrix_set == nullptr) 
        matrix_set = new 
        dump_t(dump_t(desition.max_thread * 4 > 64 ? desition.max_thread * 4 : 64));
    while (1) {
        DFS_g sgol;
        sgol.counter = gol.counter;//同步计数器
        sgol.max_zero_count_at_counter = gol.counter;
        DFS_l loc;//私有变量
        auto re = random_entry(desition.random_entry_deep);
        re.DFS1(desition, gol, loc);
        gol.max_zero_count_at_counter = gol.counter;
        /*gol.counter = sgol.counter;
        if (sgol.max_zero_count > gol.max_zero_count) {//如果找到最大值就写入
            gol.max_zero_count = sgol.max_zero_count;
            gol.max_zero_operator = sgol.max_zero_operator;
            gol.max_zero_vec = sgol.max_zero_vec;
        }*/
        //超过最大次数，终止尝试
        if (gol.counter > desition.max_try) { 
            std::cout <<"threadid:"<< thread_id << "超过最大尝试次数,返回 \n";
            break;
        }else if (found_target) {//找到目标解了
            std::cout << "找到目标解了,等待其它线程退出后写入文件\n";
            break;
        }
        else {
            //std::cout <<"threadid:"<< thread_id << "这个分支找不到更好的解了，换个分支试试运气 \n";
        }
    }
    return gol;
}

tree::DFS_g tree::MutithreadDFS(const deep_desition& desition) {

    //设定公共变量
    deep_desition tdesition = desition;
    tdesition.max_try /= desition.max_thread;//为每个贤臣分配相同的尝试次数
    std::vector<std::thread> threads;//线程池
    std::vector <DFS_g> gols;
    gols.resize(desition.max_thread);//为每个线程分配独立的返回值
    //启动线程
    for (int i = 0;i < desition.max_thread;++i) {
        threads.push_back(std::thread([&](int id) {
            gols[id] = DFS(tdesition, id);
            },i)
        );
    }
    //等待所有线程返回
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