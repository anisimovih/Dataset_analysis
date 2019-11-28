#pragma once
#include <vector>


class AbstractStorage
{
public:
	AbstractStorage() {};
	~AbstractStorage() {};
	virtual void add(long long t_price, long long t_amount) = 0;
	virtual void delete_(long long t_price) = 0;
	virtual long long get_sum_amount() = 0;
	virtual long long get(long long t_price) = 0;
	virtual std::pair<long long, long long> get_first() = 0;
	virtual std::vector<std::pair<long long, long long>> topn(size_t n) = 0;
};
