

#include <iostream>
#include <chrono>
#include "io.h"
#include"dfs.h"
#include"stat.h"
#include"vec.h"
#include"tocout.h"
#define Disable_GPU
using std::cout;
using std::endl;
using namespace std::chrono;
using namespace graph;
//#include<Windows.h>
//#include <io.h>
//#include <fcntl.h>
int main()
{
    //_setmode(_fileno(stdout), _O_U8TEXT);
    //SetConsoleOutputCP(65001);
    //初始化参数
    get_args("config.txt");
#ifndef Disable_GPU
    vecbase::base::GPU_constmask_init();//加载mask到GPU
#endif
    dfs_t root;
    root.init();
#ifndef Disable_GPU
    GPU_loadvec();//把题目加载到GPU
#endif
    root.point.setinitialcomp();
    root.point.generate_oprs(root.oprs);
    
    cout << "Input Matrix\n" << root.point;
    auto start = high_resolution_clock::now();
    // 记录结束时间
    //root.search();
    //root.random_entry();
    root.multi_entry();
    //auto r1oprs = r1.generate_oprs();
    auto end = high_resolution_clock::now();
    // 计算运行时间
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Trace: \n" << root.max_zero_route ;
    cout << "Solution Matrix\n" << root.get_max_vec() << endl;
    // 输出运行时间（单位：毫秒）
    cout << "Program Runtime " << duration.count() << "ms" << endl;
    //输出


#ifdef DEBUG_INFO
    cout << "number of bottom" << bottom_counter << "\n";
    cout << "number of max zero bottom" << root.vec_at_max_zero_count << "\n";
    cout << "number of fake bottom" << nbottom_counter << "\n";
    cout << "number of dumplicate" << dump_counter << "\n";
    cout << "number of global counter" << gcounter << "\n";
#endif
    //cout << "可选操作\n" << oprs;
    //cout << "r1\n" << r1;
    saveResult(root.max_zero_route);
    return 0;
}
