#pragma once
#include<iostream>
#include"vec.h"
#include"opr.h"
#include"oprs.h"
namespace graph {
    //输出流
    inline std::ostream& operator<<(std::ostream& out, const vec2_t& vec) {
        int i = 1;
        std::cout << "Number of 0:" << vec.count() << "\n";
        for (size_t i = 0;i < rowsize();++i) {
            for (size_t j = 0;j < colsize();++j)
            {
                std::cout << vec(i, j) << ",";
            }
            std::cout << "\n";
        }
        return out;
    }
    inline std::ostream& operator<<(std::ostream& out, const opr_t& opr) {
        auto rect = opr.getrect();
        std::cout << opr.size() << "->(" << rect.x1() << ", " << rect.y1() << ", "
            << rect.x2() << ", " << rect.y2() << ")\n";//,maxdeep="<<rect.max_deep<<"\n";
        return out;
    }
    inline std::ostream& operator<<(std::ostream& out, const route_t& oprs) {
        int i = 0;
        for (const auto& opr : oprs) {
            auto rect = opr.getrect();
            std::cout << i << ":" << opr.size() << "->(" << rect.x1() << ", " << rect.y1() << ", "
                << rect.x2() << ", " << rect.y2() << ")\n";//,maxdeep="<<rect.max_deep<<"\n";
            i++;
        }
        return out;
    }

    //输出流
    inline std::wostream& operator<<(std::wostream& out, const vec2_t& vec) {
        int i = 1;
        std::wcout << L"0的个数：" << vec.count() << L"\n";
        for (size_t i = 0;i < rowsize();++i) {
            for (size_t j = 0;j < colsize();++j)
            {
                std::wcout << vec(i, j) << ",";
            }
            std::wcout << L"\n";
        }
        return out;
    }
    inline std::wostream& operator<<(std::wostream& out, const opr_t& opr) {
        auto rect = opr.getrect();
        std::wcout << opr.size() << L"->(" << rect.x1() << L", " << rect.y1() << L", "
            << rect.x2() << L", " << rect.y2() << L")\n";//,maxdeep="<<rect.max_deep<<"\n";
        return out;
    }
    inline std::wostream& operator<<(std::wostream& out, const route_t& oprs) {
        int i = 0;
        for (const auto& opr : oprs) {
            auto rect = opr.getrect();
            std::wcout << i << L":" << opr.size() << L"->(" << rect.x1() << L", " << rect.y1() << L", "
                << rect.x2() << L", " << rect.y2() << L")\n";//,maxdeep="<<rect.max_deep<<"\n";
            i++;
        }
        return out;
    }


    void saveResult(const route_t& oprs);
}