#include "Map_storage.h"

void MapStorage::add(long long price, long long amount)
{
	if (amount != 0)
	{
		if (pairs_map_.find(price) == pairs_map_.end())
		{
			sum_amount_ += amount;
			pairs_map_.emplace(price, amount);
		}
		else
		{
			sum_amount_ = sum_amount_ - pairs_map_[price] + amount;
			pairs_map_[price] = amount;
		}
	}
	else
	{
		pairs_map_.erase(price);
		sum_amount_ -= amount;
	}
}

void MapStorage::delete_(long long price)
{
	pairs_map_.erase(price);
}


long long MapStorage::get_sum_amount()
{
	return sum_amount_;
}


long long MapStorage::get(long long price)
{
	auto iter = pairs_map_.find(price);
	if (iter != pairs_map_.end())
		return iter->second;
	else
		return -1;
}


std::pair<long long, long long> MapStorage::get_first()
{
	auto it = pairs_map_.begin();
	//return std::make_pair(it->first, it->second);
	return { it->first, it->second };
}


std::vector<std::pair<long long, long long>> MapStorage::topn(size_t n)
{
	auto vi = pairs_map_.begin();
	if (pairs_map_.size() < n)
		n = pairs_map_.size();
	advance(vi, n);

	std::vector <std::pair<long long, long long>> topn_vec;
	for (auto it = pairs_map_.begin(); it != vi; ++it)
		topn_vec.insert(topn_vec.end(), { it->first, it->second });

	return topn_vec;
}