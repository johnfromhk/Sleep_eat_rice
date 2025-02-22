
#include "tree.h"
#include <iostream>
#include <chrono>
#include "io.h"
using std::cout;
using std::endl;
using namespace std::chrono;

int main()
{
    //初始化参数
    auto args = get_args("config.txt");
    tree t0("input.txt");
    cout << "原始矩阵\n" << t0.get_vec();
    auto start = high_resolution_clock::now();
    auto result = t0.MutithreadDFS(args);
    // 记录结束时间
    auto end = high_resolution_clock::now();
    // 计算运行时间
    auto duration = duration_cast<milliseconds>(end - start);

    // 输出运行时间（单位：毫秒）
    cout << "程序运行时间: " << duration.count() << " ms" << endl;
    //cout << t0.get_vec() << t0.get_rects();
    cout << result.max_zero_vec << result.max_zero_operator;
    //输出
    if (count_zeros(result.max_zero_vec) < args.target) {
        cout << "是不是你的要求定的太高了？设备太差了？" << endl;
        cout << "找不到就是找不到，别努力了，下一把" << endl;
    }
    saveResult(result.max_zero_operator);


    return 0;
}
