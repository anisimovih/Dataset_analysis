#pragma once
#include "AbstractStorage.h"


class ArrayStorage :public AbstractStorage
{
	std::vector <std::pair<long long, long long>> pairs_array_;
	long long sum_amount_ = 0;
	int binary_search(long long price);

public:
	void add(long long price, long long amount) override;
	void delete_(long long price) override;
	long long get_sum_amount() override;
	long long get(long long price) override;
	std::pair<long long, long long> get_first();
	std::vector<std::pair<long long, long long>> topn(size_t n) override;
};
