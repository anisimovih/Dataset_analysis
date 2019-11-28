#pragma once
#include "AbstractStorage.h"


class AVLTree :public AbstractStorage
{
	struct TreeStruct;
	int nodes_num_;
	long long sum_amount_;
	TreeStruct *root;
	long long sum_;
	std::vector <std::pair<long long, long long>> topn_vec_;
	void balance(std::vector<TreeStruct**> path);
	void topn_calc(TreeStruct* cur, size_t &size);
	int find(long long price) const;


public:
	void add(long long price, long long amount) override;
	void delete_(long long price) override;
	long long get_sum_amount() override;
	long long get(long long price) override;
	std::pair<long long, long long> get_first() override;
	std::vector <std::pair<long long, long long>> topn(size_t n) override;
};
