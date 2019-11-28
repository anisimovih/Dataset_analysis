#include "Array_storage.h"


int ArrayStorage::binary_search(long long price)
{
	int midd = 0;
	int left = 0;
	int right = pairs_array_.size();
	while (left < right)
	{
		midd = left + (right - left) / 2;

		if (price < pairs_array_[midd].first)
			right = midd;
		else if (price > pairs_array_[midd].first)
			left = midd + 1;
		else
			return midd;
	}

	return -(left + 1);
}


void ArrayStorage::add(long long price, long long amount)
{
	int cell = 0;
	if (pairs_array_.size() > 0)
	{
		cell = binary_search(price);
		if (cell < 0)
		{
			pairs_array_.insert(pairs_array_.begin() + ((cell + 1) * -1), { price, amount });
		}
		else if (amount > 0)
		{
			pairs_array_[cell].second = amount;
			sum_amount_ = sum_amount_ - pairs_array_[cell].second + amount;
		}
		else
		{
			pairs_array_.erase(pairs_array_.begin() + cell);
			sum_amount_ -= amount;
		}
	}
	else
	{
		pairs_array_.push_back({ price, amount });
		sum_amount_ += amount;
	}
}


void ArrayStorage::delete_(long long price)
{
	int cell = binary_search(price);
	if (cell >= 0)
	{
		pairs_array_.erase(pairs_array_.begin() + cell);
	}
}


long long ArrayStorage::get_sum_amount()
{
	return sum_amount_;
}


long long ArrayStorage::get(long long price)
{
	int cell = binary_search(price);
	if (cell >= 0)
		return pairs_array_[cell].second;
	else
		return -1;
}


std::pair<long long, long long> ArrayStorage::get_first()
{
	if (pairs_array_.size() != 0)
		return pairs_array_[0];
	else
		return { 0, 0 };
}


std::vector<std::pair<long long, long long>> ArrayStorage::topn(size_t n)
{
	if (pairs_array_.size() < n)
	{
		n = pairs_array_.size();
	}
	std::vector <std::pair<long long, long long>> topn_vec;
	for (size_t i = 0; i < n; i++)
		topn_vec.insert(topn_vec.end(), pairs_array_[i]);
	return topn_vec;
}