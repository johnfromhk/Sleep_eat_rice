#pragma once
#include"vecbasebase.h"
#include"xxhash.h"
//¹þÏ£Ö§³Ö
namespace vecbase {
	struct MatrixHash {
		template<base::Basecompvec2f_t T1 >
		inline constexpr std::size_t operator()(const T1& comp) const {
			{
				/*std::size_t hash = 0;
				for (int i = 0;i < base::compvec2_father_t::getinnerdatasize();++i) {
					hash ^= std::hash<long long>{}(comp.getinnerdata(i))+0x9e3779b9 + (hash << 6) + (hash >> 2);
				}
				return hash;*/
				return xxh::xxhash<64>(&comp.v, &comp.v + comp.WORD_SIZE);
			}
		}
	};

}