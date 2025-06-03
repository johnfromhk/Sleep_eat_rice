#include"vec.h"
#include "removedump.h"
#include"stat.h"
namespace graph {
    thread_local static unsigned int prefix_sum[rowsize() + 1][colsize() + 1];
    //��ȡ���κ�
    static inline unsigned int get_rect_sum(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) {
        return prefix_sum[x2 + 1][y2 + 1] - prefix_sum[x1][y2 + 1] - prefix_sum[x2 + 1][y1] + prefix_sum[x1][y1];
    }
    

    static void calculate_rectsum(const vec2_t& vec) {//����ǰ׺��
        
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
        calculate_rectsum(*this);//������κ�
        for (size_t x1 = 0; x1 < rowsize(); ++x1) {
            for (size_t y1 = 0; y1 < colsize(); ++y1) {
                for (size_t x2 = x1; x2 < rowsize(); ++x2) {
                    //��ǰ֦���Լӿ��ٶ�
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = y1; y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //��ǰ֦���Լӿ��ٶ�
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                //��Ӳ�����Ԫ������
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
        
        //����ֻ��Ҫ���������ھ����ڷ�0Ԫ�صľ��μ���
        //�����Ͻǵ�����Ϊ(0,0)->(axis.x,axis.y)
        //  ���½ǵ�����Ϊ(axis.x,axis.y)->(ROWS,COLS)
        //�Ż�����·��Ϊ(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (const auto& axis : opr_) {
            for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
                //if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                //    break;
                for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                    if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                        break;
                    for (size_t x2 = axis.x(); x2 < rowsize(); ++x2) {
                        //��ǰ֦���Լӿ��ٶ�
                        if (get_rect_sum(x1, y1, x2, y1) > 10)
                            break;
                        if (get_rect_sum(x1, y1, x2, y1) == 0)
                            continue;
                        for (size_t y2 = axis.y(); y2 < colsize(); ++y2) {
                            auto rsm = get_rect_sum(x1, y1, x2, y2);
                            //��ǰ֦���Լӿ��ٶ�
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

        //����ֻ��Ҫ���������ھ����ڷ�0Ԫ�صľ��μ���
        //�����Ͻǵ�����Ϊ(0,0)->(axis.x,axis.y)
        //  ���½ǵ�����Ϊ(axis.x,axis.y)->(ROWS,COLS)
        //�Ż�����·��Ϊ(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (const auto& axis : opr_) {
            //����y2���Ͻ�
            //2�ַ�����y2���Ͻ�
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
            if (y2max == colsize())--y2max;//����Խ��

            for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
                if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                    break;
                for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                    if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                        break;
                    int y2 = y2max;//y2��ֵΪy2max
                    for (int x2 = axis.x();x2 < rowsize();++x2) {
                        if (get_rect_sum(x1, y1, x2, y1) == 0)continue;
                        if (get_rect_sum(x1, y1, x2, y2) < 10)continue;
                        
                        //��ʱf(x2,y2)��Ȼ���ڵ���10
                        //Ѱ������y2����f(x2,y2)<10
                        //��ʱy2Ӧ����[y,y2max]֮��
                        //����f(x2,y2)<10,f(x2,y2+1>=10)
                        for (;y2 >= axis.y();--y2) {
                            auto rsm = get_rect_sum(x1, y1, x2, y2);
                            
                            //��x2�����ӱ��ô���10��ֱ������
                            if (rsm > 10) {
                                //y2�Ѿ��ﵽ��Сֵ����ʱrsm��Ȼ>10�Ļ�
                                if(y2 == axis.y())
                                    goto next_y1;
                                else
                                //rsm��Ȼ����10����������y2��ֵ
                                    continue;
                            }
                            if (rsm == 10) {
                                //���һ��Ϊ0��������Сy2��ֱ�����һ�в�Ϊ0
                                if (get_rect_sum(x1, y2, x2, y2) == 0) {
                                    //y2�Ѿ��ִ���Сֵ�ˣ�������С��������
                                    if (y2 == axis.y()) goto next_y1;
                                    else continue;
                                }
                                    
                                if (get_rect_sum(x1, y1, x1, y2) > 0
                                    && get_rect_sum(x2, y1, x2, y2) > 0
                                    ) {
                                    returnoprs.insert_new(getopr(x1, y1, x2, y2));
                                }
                                //���е�������Ա�֤get_rect_sum(x1, y2, x2, y2)>0
                                //��rsm==10��
                                //��������y2-1��Ȼ����rsm<10,���Լ������������
                                //���y2�ִ�߽�Ļ���ֱ������ʣ�µ�x2
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

    //����ֻ��Ҫ���������ھ����ڷ�0Ԫ�صľ��μ���
    //�����Ͻǵ�����Ϊ(0,0)->(axis.x,axis.y)
    //  ���½ǵ�����Ϊ(axis.x,axis.y)->(ROWS,COLS)
    //�Ż�����·��Ϊ(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
    for (const auto& axis : opr_) {
        for (size_t x1 = axis.x(); x1 < rowsize(); --x1) {
            if (get_rect_sum(x1, axis.y(), axis.x(), axis.y()) > 10)
                break;
            for (size_t y1 = axis.y(); y1 < colsize(); --y1) {
                if (get_rect_sum(x1, y1, axis.x(), axis.y()) > 10)
                    break;
                for (size_t x2 = axis.x(); x2 < rowsize(); ++x2) {
                    //��ǰ֦���Լӿ��ٶ�
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = axis.y(); y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //��ǰ֦���Լӿ��ٶ�
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
            //prefix_sum = lastT->prefix_sum;//��ȡ�ϸ��ڵ��prefix_sum
            //calculate_prefix(current_opr.x1,current_opr.y1);//����ǰ׺��,ֻ���¾������Ͻǵ����½ǵľ��κ�
            //�ϸ��ڵ㲻Ϊ�գ�����Ҫ���������ξ�������������
            //�Ȱ��ϸ��ڵ�����������������
            //��֤�����Ƿ����ɿ��ã��������ֱ�����
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
        calculate_rectsum(*this);//������κ�
        generate_oldoprs(returnopr, oprs_);
        generate_newoprs(returnopr, opr_);
    }
    bool vec2_t::checkremains()const {
        calculate_rectsum(*this);//������κ�
        for (size_t x1 = 0; x1 < rowsize(); ++x1) {
            for (size_t y1 = 0; y1 < colsize(); ++y1) {
                for (size_t x2 = x1; x2 < rowsize(); ++x2) {
                    //��ǰ֦���Լӿ��ٶ�
                    if (get_rect_sum(x1, y1, x2, y1) > 10)
                        break;
                    if (get_rect_sum(x1, y1, x2, y1) == 0)
                        continue;
                    for (size_t y2 = y1; y2 < colsize(); ++y2) {
                        auto rsm = get_rect_sum(x1, y1, x2, y2);
                        //��ǰ֦���Լӿ��ٶ�
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


