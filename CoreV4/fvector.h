#pragma once
namespace vecbase {
	//������̬����֧��
	template<typename T, int maxsize>
	class fvector {
	public:
		T datas[maxsize];//�洢Ԫ��
		size_t n = 0;//Ŀǰ���˼���Ԫ��
		inline static constexpr size_t nmax = maxsize;//�����Էż���Ԫ��
	public://���룬��ѯ��ɾ��
		HOST_DEVICE inline constexpr size_t find(const T& data)const {
			for (size_t i = 0;i < n;++i) {
				if (datas[i] == data)
					return i;
			}
			return n;
		}
		HOST_DEVICE inline constexpr void insert(const T& data) {
			if (find(data) == n) //==n����û���ظ�
			{
				datas[n] = data;
				++n;
			}
		}
		HOST_DEVICE inline constexpr void clear() {
			n=0;//ɾ����������
		}
		HOST_DEVICE inline constexpr void erase(size_t& i) {
			datas[i] = datas[n - 1];//�������һ��ֵ��Ҫɾ����ֵ��Ȼ�������-1
			--n;
		}
		HOST_DEVICE inline constexpr void erase(const T& data) {
			auto fid = find(data);
			if (fid != n) //�ҵ���
				erase(fid);
		}
	public:
		//������
		HOST_DEVICE inline constexpr auto begin() const { return &datas[0]; }
		HOST_DEVICE inline constexpr auto begin() { return &datas[0]; }
		HOST_DEVICE inline constexpr auto end() const { return &datas[n]; }
		HOST_DEVICE inline constexpr T& operator[](const size_t& i) { return datas[i]; }
		HOST_DEVICE inline constexpr auto& size() const { return n; }
		HOST_DEVICE inline static constexpr auto& maxsize()  { return nmax; }
	};

}