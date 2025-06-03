#pragma once
#include <vector>
#include <unordered_set>
#include <mutex>
#include<shared_mutex>
#include "robin_hood.h"
#ifndef SHARED_H
#define SHARED_H
template <typename T, typename Hasher = std::hash<T>>
class shared_unordered_set {
private:
	//std::vector<std::unordered_set<T, Hasher>> shards;//无序表集
	std::vector < robin_hood::unordered_set<T, Hasher>>shards;
	std::vector<std::mutex> mutexes;//锁集
	size_t Numshards;
	Hasher hasher;//哈希函数对象
	size_t get_shard_index(const T& key) const {//返回分片
		return hasher(key) % Numshards;
	}
public:
	shared_unordered_set(size_t Numshards,size_t MemoryReserved) :
		shards(Numshards), mutexes(Numshards) {
		this->Numshards = Numshards;
		for (int i = 0;i < Numshards;++i) {//预分配内存
			shards[i].reserve(MemoryReserved / Numshards);
			//shards[i].reserve(33554432*16 / Numshards);
			//shards[i].reserve(33554432 / Numshards);
		}

	}
public:
	//#define REHASH_CHECK //检查发生REHASH的次数
#ifdef REHASH_CHECK
	std::atomic<int> rehash_counter = 0;
#endif
	bool insert(const T& key) {
		size_t idx = get_shard_index(key);
		std::lock_guard<std::mutex> lock(mutexes[idx]);

#ifdef REHASH_CHECK
		auto last_bucket_count = shards[idx].bucket_count();
#endif
		auto returnvar = (shards[idx].insert(key)).second;
#ifdef REHASH_CHECK
		if (last_bucket_count != shards[idx].bucket_count())
			rehash_counter++;
#endif
		return returnvar;
	}
	bool find(const T& key) {//如果找到了返回真，没找到返回假
		size_t idx = get_shard_index(key);
		return shards[idx].find(key) != shards[idx].end();
	}

};

#endif