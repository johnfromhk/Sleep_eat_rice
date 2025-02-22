
#include "tree.h"

vec_t2& tree::get_vec() {
    return tree::vec;
}
rects_t& tree::get_rects() {
    return tree::rects;
}
size_t tree::size() {
    return rects.size();
}
int tree::count_zero() {
    return current_zero;
}


#include<algorithm>
#include "fast_vec_sum.h"
void tree::generate_oprs() 
{
    prefix_t prefix(vec);
    if (lastT == nullptr) {//初始节点，对矩形进行完整计算
    //if(1){
        //计算前缀和
        // 遍历所有可能的矩形
        for (int x1 = 0; x1 < ROWS; ++x1) {
            for (int y1 = 0; y1 < COLS; ++y1) {
                for (int x2 = x1; x2 < ROWS; ++x2) {
                    //提前枝减以加快速度
                    if (prefix.getRectangleSum(x1, y1, x2, y1) > 10)
                        break;
                    if (prefix.getRectangleSum(x1, y1, x2, y1) == 0)
                        continue;
                    for (int y2 = y1; y2 < COLS; ++y2) {
                        auto rsm = prefix.getRectangleSum(x1, y1, x2, y2);
                        //提前枝减以加快速度
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                //if (getRectangleSum(x1, y1, x1, y2) > 0)
                                if (prefix.getRectangleSum(x1, y1, x1, y2) > 0
                                    && prefix.getRectangleSum(x1, y1, x2, y1) > 0
                                    && prefix.getRectangleSum(x2, y1, x2, y2) > 0
                                    && prefix.getRectangleSum(x1, y2, x2, y2) > 0
                                    )
                                    //添加并计算元素数量
                                {
                                    rects.push_back(rect_t(x1, y1, x2, y2, vec));
                                }
                            }
                            break;
                        }
                    }

                }
            }
        }
        //对输出的可操作数按从小到大排序
        std::sort(rects.begin(), rects.end(),
            [](const rect_t& rect1, const rect_t& rect2) {return rect1.item < rect2.item;}
        );
    }
    else {
        //prefix_sum = lastT->prefix_sum;//读取上个节点的prefix_sum
        //calculate_prefix(current_opr.x1,current_opr.y1);//计算前缀和,只跟新矩形左上角到右下角的矩形和
        //上个节点不为空，不需要对整个矩形经行完整搜索，
        //先把上个节点的搜索结果拷贝进来
        //验证矩形是否依旧可用，如果可用直接添加
        for (auto& rect : lastT->rects) {
            if (prefix.getRectangleSum(rect.x1, rect.y1, rect.x2, rect.y2) == 10) {
                rects.push_back(rect);
            }
        }
        //我们只需要搜索包括在矩形内非0元素的矩形即可
        //即左上角的区域为(0,0)->(axis.x,axis.y)
        //  右下角的区域为(axis.x,axis.y)->(ROWS,COLS)
        //优化搜索路径为(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (auto& axis : current_oprs[deep-1].item_list) {
            for (int x1 = axis.x; x1 >= 0; --x1) {
                for (int y1 = axis.y; y1 >= 0; --y1) {
                    for (int x2 = axis.x; x2 < ROWS; ++x2) {
                        //提前枝减以加快速度
                        /*if (getRectangleSum(x1, y1, x2, y1) > 10)
                            break;
                        if (getRectangleSum(x1, y1, x2, y1) == 0)
                            continue;*/
                        for (int y2 = axis.y; y2 < COLS; ++y2) {
                            auto rsm = prefix.getRectangleSum(x1, y1, x2, y2);
                            //提前枝减以加快速度
                            if (rsm >= 10) {
                                if (rsm == 10) {
                                    //排除4条边都为0的情况，debug
                                    if (prefix.getRectangleSum(x1, y1, x1, y2) > 0
                                        && prefix.getRectangleSum(x1, y1, x2, y1) > 0
                                        && prefix.getRectangleSum(x2, y1, x2, y2) > 0
                                        && prefix.getRectangleSum(x1, y2, x2, y2) > 0
                                        ) {
                                        //添加并计算元素数量
                                        rect_t pushback = rect_t(x1, y1, x2, y2, vec);
                                        rects.push_back(pushback);
                                    }
                                }
                                break;
                            }
                        }

                    }
                }
            }
        }
        
        //对输出的可操作数按从小到大排序
        std::sort(rects.begin(), rects.end(),
            [](const rect_t& rect1, const rect_t& rect2) {return rect1.item < rect2.item;}
        );
    }
}

