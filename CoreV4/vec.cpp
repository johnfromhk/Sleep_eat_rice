#include"vec.h"
#include "removedump.h"
#include"stat.h"
namespace graph {
    thread_local static unsigned int prefix_sum[rowsize() + 1][colsize() + 1];
    //获取矩形和
    static inline unsigned int get_rect_sum(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) {
        return prefix_sum[x2 + 1][y2 + 1] - prefix_sum[x1][y2 + 1] - prefix_sum[x2 + 1][y1] + prefix_sum[x1][y1];
    }
    

    static void calculate_rectsum(const vec2_t& vec) {//计算前缀和
        
        for (size_t i = 1; i <= rowsize(); ++i) {
            for (size_t j = 1; j <= colsize(); ++j) {
                prefix_sum[i][j] = vec(i - 1, j - 1)
                    + prefix_sum[i - 1][j]
                    + prefix_sum[i][j - 1]
                    - prefix_sum[i - 1][j - 1];
            }
        }/*
        for (size_t i = 1; i <= rowsize(); ++i) {
            for (size_t j = 1; j <= colsize(); ++j) {
                prefix_sum[i][j] = vec(i - 1, j - 1)
                    + prefix_sum[i-1][j];
            }
        }
        for (size_t i = 1; i <= rowsize(); ++i) {
            for (size_t j = 1; j <= colsize(); ++j) {
                prefix_sum[i][j] += prefix_sum[i][j-1];
            }
        }*/
    }


    void vec2_t::generate_oprs(oprs_t&oprs)const{
        calculate_rectsum(*this);//计算矩形和
        for (size_t x1 = 0; x1 < rowsize(); ++x1) {
            for (size_t y1 = 0; y1 < colsize(); ++y1) {
                for (size_t x2 = x1; x2 < rowsize(); ++x2) {
                    //提前枝减以加快速度
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = y1; y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //提前枝减以加快速度
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                //添加并计算元素数量
                                if (get_rect_sum(x1, y1, x1, y2) > 0
                                    //&& get_rect_sum(x1, y1, x2, y1) > 0
                                    && get_rect_sum(x2, y1, x2, y2) > 0
                                    && get_rect_sum(x1, y2, x2, y2) > 0
                                    ) {
                                    oprs.insert_old(getopr(x1, y1, x2, y2));
                                }
                            }
                            break;
                        }
                    }

                }
            }
        }
    }
//#define preundump
#define GENERATE_NEWOPRS_V1
#ifdef GENERATE_NEWOPRS_V1
    void vec2_t::generate_newoprs(oprs_t& returnoprs, const opr_t& opr_) const {
        
        //我们只需要搜索包括在矩形内非0元素的矩形即可
        //即左上角的区域为(0,0)->(axis.x,axis.y)
        //  右下角的区域为(axis.x,axis.y)->(ROWS,COLS)
        //优化搜索路径为(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (const auto& axis : opr_) {
            for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
                //if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                //    break;
                for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                    if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                        break;
                    for (size_t x2 = axis.x(); x2 < rowsize(); ++x2) {
                        //提前枝减以加快速度
                        if (get_rect_sum(x1, y1, x2, y1) > 10)
                            break;
                        if (get_rect_sum(x1, y1, x2, y1) == 0)
                            continue;
                        for (size_t y2 = axis.y(); y2 < colsize(); ++y2) {
                            auto rsm = get_rect_sum(x1, y1, x2, y2);
                            //提前枝减以加快速度
                            if (rsm >= 10) {
                                if (rsm == 10) {
                                    if (get_rect_sum(x1, y1, x1, y2) > 0
                                        //&& get_rect_sum(x1, y1, x2, y1) > 0
                                        && get_rect_sum(x2, y1, x2, y2) > 0
                                        && get_rect_sum(x1, y2, x2, y2) > 0
                                        ) {
#ifdef preundump
                                        if (!matrix_set->find(*this*getopr(x1, y1, x2, y2))) {
#endif
                                            returnoprs.insert_new(getopr(x1, y1, x2, y2));

#ifdef preundump
                                        }
                                        else {
#ifdef DEBUG_INFO
                                            dump_counter++;
#endif
                                        }
#endif
                                    }
                                }
                                break;
                            }
                        }

                    }
                }
            }
        }
        //return oprs;
    }
#else 
#ifdef GENERATE_NEWOPRS_V2
    void vec2_t::generate_newoprs(oprs_t& returnoprs, const opr_t& opr_) const {

        //我们只需要搜索包括在矩形内非0元素的矩形即可
        //即左上角的区域为(0,0)->(axis.x,axis.y)
        //  右下角的区域为(axis.x,axis.y)->(ROWS,COLS)
        //优化搜索路径为(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (const auto& axis : opr_) {
            //查找y2的上届
            //2分法搜索y2的上界
            int y2max = colsize();
            {
                int y2min = axis.y();
                for (int i = 0;i < 4;++i) {
                    if (y2min == y2max) break;
                    int mid = (y2min + y2max) / 2;
                    if (get_rect_sum(axis.x(), axis.y(), axis.x(), mid) >= 10) {
                        y2max = mid;
                    }
                    else {
                        y2min = mid + 1;
                    }
                }
            }
            if (y2max == colsize())--y2max;//避免越界

            for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
                if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                    break;
                for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                    if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                        break;
                    int y2 = y2max;//y2的值为y2max
                    for (int x2 = axis.x();x2 < rowsize();++x2) {
                        if (get_rect_sum(x1, y1, x2, y1) == 0)continue;
                        if (get_rect_sum(x1, y1, x2, y2) < 10)continue;
                        
                        //此时f(x2,y2)必然大于等于10
                        //寻找最大的y2满足f(x2,y2)<10
                        //此时y2应该在[y,y2max]之间
                        //满足f(x2,y2)<10,f(x2,y2+1>=10)
                        for (;y2 >= axis.y();--y2) {
                            auto rsm = get_rect_sum(x1, y1, x2, y2);
                            
                            //再x2再增加避让大于10，直接跳过
                            if (rsm > 10) {
                                //y2已经达到最小值，此时rsm任然>10的话
                                if(y2 == axis.y())
                                    goto next_y1;
                                else
                                //rsm任然大于10，继续减少y2的值
                                    continue;
                            }
                            if (rsm == 10) {
                                //最后一行为0，继续减小y2，直到最后一行不为0
                                if (get_rect_sum(x1, y2, x2, y2) == 0) {
                                    //y2已经抵达最小值了，继续减小毫无意义
                                    if (y2 == axis.y()) goto next_y1;
                                    else continue;
                                }
                                    
                                if (get_rect_sum(x1, y1, x1, y2) > 0
                                    && get_rect_sum(x2, y1, x2, y2) > 0
                                    ) {
                                    returnoprs.insert_new(getopr(x1, y1, x2, y2));
                                }
                                //运行到这里可以保证get_rect_sum(x1, y2, x2, y2)>0
                                //且rsm==10，
                                //所以搜索y2-1必然满足rsm<10,可以继续后面的搜索
                                //如果y2抵达边界的话，直接跳过剩下的x2
                                if (y2 == axis.y()) goto next_y1;
                                else --y2;
                            }
                            break;
                        }
                    }
                next_y1:;
                }
            }
        }
        //return oprs;
    }
#else
void vec2_t::generate_newoprs(oprs_t& returnoprs, const opr_t& opr_) const {

    //我们只需要搜索包括在矩形内非0元素的矩形即可
    //即左上角的区域为(0,0)->(axis.x,axis.y)
    //  右下角的区域为(axis.x,axis.y)->(ROWS,COLS)
    //优化搜索路径为(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
    for (const auto& axis : opr_) {
        for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
            if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                break;
            for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                    break;
                for (size_t x2 = axis.x(); x2 < rowsize(); ++x2) {
                    //提前枝减以加快速度
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = axis.y(); y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //提前枝减以加快速度
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                if (get_rect_sum(x1, y1, x1, y2) > 0
                                    && get_rect_sum(x1, y1, x2, y1) > 0
                                    && get_rect_sum(x2, y1, x2, y2) > 0
                                    && get_rect_sum(x1, y2, x2, y2) > 0
                                    ) {
                                    returnoprs.insert_new(getopr(x1, y1, x2, y2));
                                }
                            }
                            break;
                        }
                    }

                }
            }
        }
    }
}

#endif


#endif

    void vec2_t::generate_oldoprs(oprs_t& returnoprs, const oprs_t& oprs) const {
            //prefix_sum = lastT->prefix_sum;//读取上个节点的prefix_sum
            //calculate_prefix(current_opr.x1,current_opr.y1);//计算前缀和,只跟新矩形左上角到右下角的矩形和
            //上个节点不为空，不需要对整个矩形经行完整搜索，
            //先把上个节点的搜索结果拷贝进来
            //验证矩形是否依旧可用，如果可用直接添加
        for (const auto&opr:oprs) {
            if (isvalid(opr))
            {
#ifdef preundump
                if(!matrix_set->find(*this*opr)) {
#endif
                    returnoprs.insert_old(opr);
#ifdef preundump
                }
                else {
#ifdef DEBUG_INFO
                    dump_counter++;
#endif
                }
#endif  
            }
        }
    }
    void vec2_t::generate_oprs(oprs_t& returnopr, const oprs_t& oprs_, const opr_t& opr_) const {
        calculate_rectsum(*this);//计算矩形和
        generate_oldoprs(returnopr, oprs_);
        generate_newoprs(returnopr, opr_);
    }
    bool vec2_t::checkremains()const {
        calculate_rectsum(*this);//计算矩形和
        for (size_t x1 = 0; x1 < rowsize(); ++x1) {
            for (size_t y1 = 0; y1 < colsize(); ++y1) {
                for (size_t x2 = x1; x2 < rowsize(); ++x2) {
                    //提前枝减以加快速度
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = y1; y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //提前枝减以加快速度
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                return true;
                            }
                            break;
                        }
                    }

                }
            }
        }
        return false;
    }

}


