#pragma once
#include<string>
//#include"vec_base.h"
#include"opr.h"
#include"oprs.h"
namespace graph {
	using vecbase::MatrixHash;
#ifdef __CUDACC__
	//GPU�˵�ԭʼ����
	__constant__ size_t GPU_vec[rowsize()][colsize()];
#endif
	class vec2_t:public compvec2_t{
	public://ԭʼ����
		//using compvec2_t::v ;
		static size_t vec[rowsize()][colsize()];//16*10��ԭʼ����
	public://��������
		//ͨ��x,y��ȡ�����ֵ
		HOST_DEVICE inline constexpr size_t operator()(const auto& x, const auto& y) const {
#ifdef __CUDA_ARCH__
			//GPU���ȡ����
			return GPU_vec[x][y] * getbit(vecbase::xy_to_i(x, y));
#else
			//CPU���ȡ����
			return vec[x][y] * getbit(vecbase::xy_to_i(x, y)); 
#endif
		}
		//��ȡtxt�ļ�
		void static loadvec(const std::string& filepath);
		inline constexpr size_t count() const{ return count0(); }
	public://���캯��
		HOST_DEVICE inline constexpr vec2_t() { }//Ĭ�ϣ�ʲô������
		HOST_DEVICE inline constexpr vec2_t(const compvec2_t& b):compvec2_t(b) { ; }//ͨ��compvec����
		inline void setinitialcomp() {
			reset();
			for (size_t i = 0;i < rowsize();++i) {
				for (size_t j = 0;j < colsize();++j)
					if (vec[i][j] > 0)
						set(i, j);
			}
		}
	public://·������
		//�ж�һ�������Ƿ���Ч,���ж�opr�Ƿ���vec���Ӽ�
		HOST_DEVICE inline constexpr bool isvalid(const opr_t& opr) const { return opr.is_subset_of(*this); }
		//����·��
		HOST_DEVICE inline constexpr vec2_t operator*(const opr_t& opr) const{return *this - opr;}
	public:
		//��ȡ������,��ȡ�Ӽ�
		HOST_DEVICE inline constexpr opr_t getopr(const auto& x1, const auto& y1, const auto& x2, const auto& y2) const { return subset(x1, y1, x2, y2); }
	public://���ɲ����� CPU��
		void generate_oprs(oprs_t& returnoprs) const;//��ʼ�����ɲ�����
		void generate_oprs(oprs_t&returnoprs,const oprs_t& oprs, const opr_t& opr) const;//�������в����������²�����
		void generate_newoprs(oprs_t& returnoprs, const opr_t& opr_) const;
		void generate_oldoprs(oprs_t& returnoprs, const oprs_t&oprs) const;
		bool checkremains() const;//����������п��������Ĳ���
	};
	//����GPU����newoprs
	//��һ������Ϊoprs_t���ͣ������ŷ���ֵ��
	//�ڶ�������Ϊvec2_t���ͣ������ŵ�ǰ�ڵ�
	//����������Ϊoprs_t���ͣ���������Ҫ�Ų��oprs
	//���ĸ�����Ϊopr_t ���ͣ������ŵ�ǰ������
	extern "C" void GPU_generate_oprs(void* returnoprs, void* currentvec, void* oprs, void* opr);
	extern "C" void GPU_loadvec();
}
