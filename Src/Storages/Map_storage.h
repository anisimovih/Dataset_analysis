#pragma once
#include "AbstractStorage.h"
#include <map>


class MapStorage :public AbstractStorage
{
	std::map<long long, long long> pairs_map_;
	long long sum_amount_ = 0;

public:
	void add(long long price, long long amount) override;
	void delete_(long long price) override;
	long long get_sum_amount() override;
	long long get(long long price) override;
	std::pair<long long, long long> get_first() override;
	std::vector<std::pair<long long, long long>> topn(size_t n) override;
};
