#pragma once
#include <vector>
#include <unordered_set>
#include <mutex>
#include<shared_mutex>
#ifndef SHARED_H
#define SHARED_H
template <typename T,typename Hasher = std::hash<T>>
class shared_unordered_set {
private:
	std::vector<std::unordered_set<T, Hasher>> shards;//无序表集
	std::vector<std::mutex> mutexes;//锁集
	size_t Numshards;
	Hasher hasher;//哈希函数对象
	size_t get_shard_index(const T& key) const {//返回分片
		return hasher(key) % Numshards;
	}
public:
	shared_unordered_set(size_t Numshards):
		shards(Numshards),mutexes(Numshards){
		this->Numshards = Numshards;
	}
public:
	bool insert(const T& key) {
		size_t idx = get_shard_index(key);
		std::lock_guard<std::mutex> lock(mutexes[idx]);
		return (shards[idx].insert(key)).second;
	}

};

#endif