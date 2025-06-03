#pragma once
#include<string>
//#include"vec_base.h"
#include"opr.h"
#include"oprs.h"
namespace graph {
	using vecbase::MatrixHash;
#ifdef __CUDACC__
	//GPU端的原始数组
	__constant__ size_t GPU_vec[rowsize()][colsize()];
#endif
	class vec2_t:public compvec2_t{
	public://原始矩阵
		//using compvec2_t::v ;
		static size_t vec[rowsize()][colsize()];//16*10的原始矩阵
	public://基本操作
		//通过x,y获取矩阵的值
		HOST_DEVICE inline constexpr size_t operator()(const auto& x, const auto& y) const {
#ifdef __CUDA_ARCH__
			//GPU版读取数据
			return GPU_vec[x][y] * getbit(vecbase::xy_to_i(x, y));
#else
			//CPU版读取数据
			return vec[x][y] * getbit(vecbase::xy_to_i(x, y)); 
#endif
		}
		//读取txt文件
		void static loadvec(const std::string& filepath);
		inline constexpr size_t count() const{ return count0(); }
	public://构造函数
		HOST_DEVICE inline constexpr vec2_t() { }//默认，什么都不干
		HOST_DEVICE inline constexpr vec2_t(const compvec2_t& b):compvec2_t(b) { ; }//通过compvec构造
		inline void setinitialcomp() {
			reset();
			for (size_t i = 0;i < rowsize();++i) {
				for (size_t j = 0;j < colsize();++j)
					if (vec[i][j] > 0)
						set(i, j);
			}
		}
	public://路径操作
		//判断一个操作是否有效,即判断opr是否是vec的子集
		HOST_DEVICE inline constexpr bool isvalid(const opr_t& opr) const { return opr.is_subset_of(*this); }
		//进入路径
		HOST_DEVICE inline constexpr vec2_t operator*(const opr_t& opr) const{return *this - opr;}
	public:
		//获取操作符,即取子集
		HOST_DEVICE inline constexpr opr_t getopr(const auto& x1, const auto& y1, const auto& x2, const auto& y2) const { return subset(x1, y1, x2, y2); }
	public://生成操作符 CPU版
		void generate_oprs(oprs_t& returnoprs) const;//初始化生成操作符
		void generate_oprs(oprs_t&returnoprs,const oprs_t& oprs, const opr_t& opr) const;//根据现有操作符生成新操作符
		void generate_newoprs(oprs_t& returnoprs, const opr_t& opr_) const;
		void generate_oldoprs(oprs_t& returnoprs, const oprs_t&oprs) const;
		bool checkremains() const;//返回真代表还有可以消除的部分
	};
	//利用GPU查找newoprs
	//第一个参数为oprs_t类型，用来放返回值的
	//第二个参数为vec2_t类型，用来放当前节点
	//第三个参数为oprs_t类型，用来放需要排查的oprs
	//第四个参数为opr_t 类型，用来放当前操作符
	extern "C" void GPU_generate_oprs(void* returnoprs, void* currentvec, void* oprs, void* opr);
	extern "C" void GPU_loadvec();
}
