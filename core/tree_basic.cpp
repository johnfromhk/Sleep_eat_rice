
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
    if (lastT == nullptr) {//��ʼ�ڵ㣬�Ծ��ν�����������
    //if(1){
        //����ǰ׺��
        // �������п��ܵľ���
        for (int x1 = 0; x1 < ROWS; ++x1) {
            for (int y1 = 0; y1 < COLS; ++y1) {
                for (int x2 = x1; x2 < ROWS; ++x2) {
                    //��ǰ֦���Լӿ��ٶ�
                    if (prefix.getRectangleSum(x1, y1, x2, y1) > 10)
                        break;
                    if (prefix.getRectangleSum(x1, y1, x2, y1) == 0)
                        continue;
                    for (int y2 = y1; y2 < COLS; ++y2) {
                        auto rsm = prefix.getRectangleSum(x1, y1, x2, y2);
                        //��ǰ֦���Լӿ��ٶ�
                        if (rsm >= 10) {
                            if (rsm == 10) {
                                //if (getRectangleSum(x1, y1, x1, y2) > 0)
                                if (prefix.getRectangleSum(x1, y1, x1, y2) > 0
                                    && prefix.getRectangleSum(x1, y1, x2, y1) > 0
                                    && prefix.getRectangleSum(x2, y1, x2, y2) > 0
                                    && prefix.getRectangleSum(x1, y2, x2, y2) > 0
                                    )
                                    //��Ӳ�����Ԫ������
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
        //������Ŀɲ���������С��������
        std::sort(rects.begin(), rects.end(),
            [](const rect_t& rect1, const rect_t& rect2) {return rect1.item < rect2.item;}
        );
    }
    else {
        //prefix_sum = lastT->prefix_sum;//��ȡ�ϸ��ڵ��prefix_sum
        //calculate_prefix(current_opr.x1,current_opr.y1);//����ǰ׺��,ֻ���¾������Ͻǵ����½ǵľ��κ�
        //�ϸ��ڵ㲻Ϊ�գ�����Ҫ���������ξ�������������
        //�Ȱ��ϸ��ڵ�����������������
        //��֤�����Ƿ����ɿ��ã��������ֱ�����
        for (auto& rect : lastT->rects) {
            if (prefix.getRectangleSum(rect.x1, rect.y1, rect.x2, rect.y2) == 10) {
                rects.push_back(rect);
            }
        }
        //����ֻ��Ҫ���������ھ����ڷ�0Ԫ�صľ��μ���
        //�����Ͻǵ�����Ϊ(0,0)->(axis.x,axis.y)
        //  ���½ǵ�����Ϊ(axis.x,axis.y)->(ROWS,COLS)
        //�Ż�����·��Ϊ(axis.x,axis.y)->(0,0),(axis.x,axis.y)->(ROWS,COLS)
        for (auto& axis : current_oprs[deep-1].item_list) {
            for (int x1 = axis.x; x1 >= 0; --x1) {
                for (int y1 = axis.y; y1 >= 0; --y1) {
                    for (int x2 = axis.x; x2 < ROWS; ++x2) {
                        //��ǰ֦���Լӿ��ٶ�
                        /*if (getRectangleSum(x1, y1, x2, y1) > 10)
                            break;
                        if (getRectangleSum(x1, y1, x2, y1) == 0)
                            continue;*/
                        for (int y2 = axis.y; y2 < COLS; ++y2) {
                            auto rsm = prefix.getRectangleSum(x1, y1, x2, y2);
                            //��ǰ֦���Լӿ��ٶ�
                            if (rsm >= 10) {
                                if (rsm == 10) {
                                    //�ų�4���߶�Ϊ0�������debug
                                    if (prefix.getRectangleSum(x1, y1, x1, y2) > 0
                                        && prefix.getRectangleSum(x1, y1, x2, y1) > 0
                                        && prefix.getRectangleSum(x2, y1, x2, y2) > 0
                                        && prefix.getRectangleSum(x1, y2, x2, y2) > 0
                                        ) {
                                        //��Ӳ�����Ԫ������
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
        
        //������Ŀɲ���������С��������
        std::sort(rects.begin(), rects.end(),
            [](const rect_t& rect1, const rect_t& rect2) {return rect1.item < rect2.item;}
        );
    }
}

