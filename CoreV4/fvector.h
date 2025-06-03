#pragma once
namespace vecbase {
	//基本静态数组支持
	template<typename T, int maxsize>
	class fvector {
	public:
		T datas[maxsize];//存储元素
		size_t n = 0;//目前用了几个元素
		inline static constexpr size_t nmax = maxsize;//最大可以放几个元素
	public://插入，查询，删除
		HOST_DEVICE inline constexpr size_t find(const T& data)const {
			for (size_t i = 0;i < n;++i) {
				if (datas[i] == data)
					return i;
			}
			return n;
		}
		HOST_DEVICE inline constexpr void insert(const T& data) {
			if (find(data) == n) //==n代表没有重复
			{
				datas[n] = data;
				++n;
			}
		}
		HOST_DEVICE inline constexpr void clear() {
			n=0;//删除所有数据
		}
		HOST_DEVICE inline constexpr void erase(size_t& i) {
			datas[i] = datas[n - 1];//交换最后一个值和要删除的值，然后计数器-1
			--n;
		}
		HOST_DEVICE inline constexpr void erase(const T& data) {
			auto fid = find(data);
			if (fid != n) //找到了
				erase(fid);
		}
	public:
		//迭代器
		HOST_DEVICE inline constexpr auto begin() const { return &datas[0]; }
		HOST_DEVICE inline constexpr auto begin() { return &datas[0]; }
		HOST_DEVICE inline constexpr auto end() const { return &datas[n]; }
		HOST_DEVICE inline constexpr T& operator[](const size_t& i) { return datas[i]; }
		HOST_DEVICE inline constexpr auto& size() const { return n; }
		HOST_DEVICE inline static constexpr auto& maxsize()  { return nmax; }
	};

}