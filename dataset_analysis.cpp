#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <time.h> 

class AbstractStorage
{
public:
	virtual void add(long long t_price, long long t_amount) = 0;
	virtual void delete_(long long t_price) = 0;
	virtual long long get_sum_amount() = 0;
	virtual long long get(long long t_price) = 0;
	virtual std::pair<long long, long long> get_first() = 0;
	virtual std::vector<std::pair<long long, long long>> topn(size_t n) = 0;
	virtual ~AbstractStorage() {};
};


class ArrayStorage :public AbstractStorage
{
	std::vector <std::pair<long long, long long>> pairs_array_;
	long long sum_amount_ = 0;


	int binary_search(long long price)
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


public:

	void add(long long price, long long amount) override
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


	void delete_(long long price) override
	{
		int cell = binary_search(price);
		if (cell >= 0)
		{
			pairs_array_.erase(pairs_array_.begin() + cell);
		}
	}


	long long get_sum_amount() override
	{
		return sum_amount_;
	}


	long long get(long long price) override
	{
		int cell = binary_search(price);
		if (cell >= 0)
			return pairs_array_[cell].second;
		else
			return -1;
	}


	std::pair<long long, long long> get_first()
	{
		if (pairs_array_.size() != 0)
			return pairs_array_[0];
		else
			return { 0, 0 };
	}


	std::vector<std::pair<long long, long long>> topn(size_t n) override
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
};


class MapStorage :public AbstractStorage
{
	std::map<long long, long long> pairs_map_;
	long long sum_amount_ = 0;


public:

	void add(long long price, long long amount) override
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

	void delete_(long long price) override
	{
		pairs_map_.erase(price);
	}


	long long get_sum_amount() override
	{
		return sum_amount_;
	}


	long long get(long long price) override
	{
		auto iter = pairs_map_.find(price);
		if (iter != pairs_map_.end())
			return iter->second;
		else
			return -1;
	}


	std::pair<long long, long long> get_first() override
	{
		auto it = pairs_map_.begin();
		//return std::make_pair(it->first, it->second);
		return { it->first, it->second };
	}


	std::vector<std::pair<long long, long long>> topn(size_t n) override
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
};


class AVLTree :public AbstractStorage
{

	struct TreeStruct {

		TreeStruct(std::pair<long long, long long> value) : value(value)
		{
			count = 1;
			height = 1;

			left = nullptr;
			right = nullptr;
		}

		TreeStruct *left;
		TreeStruct *right;
		std::pair<long long, long long> value;
		int count;  // how many nodes are there in this subtree
		int height;


		void UpdateValues()
		{
			count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;

			int val1 = left != nullptr ? left->height : 0;
			int val2 = right != nullptr ? right->height : 0;

			height = ((val1 > val2) ? val1 : val2) + 1;
		}

		int BalanceFactor()
		{
			return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height : 0);
		}

		TreeStruct* LeftRotate()
		{
			TreeStruct* R = right;
			right = right->left;
			R->left = this;

			this->UpdateValues();
			R->UpdateValues();

			return R;
		}

		TreeStruct* RightRotate()
		{
			TreeStruct* L = left;
			left = left->right;
			L->right = this;

			this->UpdateValues();
			L->UpdateValues();

			return L;
		}
	};


	int nodes_num_;
	long long sum_amount_;
	TreeStruct *root;
	long long sum_;
	std::vector <std::pair<long long, long long>> topn_vec_;


	void balance(std::vector<TreeStruct**> path)
	{
		std::reverse(path.begin(), path.end());

		for (auto indirect : path) {
			(*indirect)->UpdateValues();

			// left - left
			if ((*indirect)->BalanceFactor() >= 2 and (*indirect)->left->BalanceFactor() >= 1)
				*indirect = (*indirect)->RightRotate();

			// left - right
			else if ((*indirect)->BalanceFactor() >= 2) {
				(*indirect)->left = (*indirect)->left->LeftRotate();
				*indirect = (*indirect)->RightRotate();
			}

			// right - right
			else if ((*indirect)->BalanceFactor() <= -2 and (*indirect)->right->BalanceFactor() <= -1)
				*indirect = (*indirect)->LeftRotate();

			// right - left
			else if ((*indirect)->BalanceFactor() <= -2) {
				(*indirect)->right = ((*indirect)->right)->RightRotate();
				*indirect = (*indirect)->LeftRotate();
			}
		}
	}


	void topn_calc(TreeStruct* cur, size_t &size)
	{
		if (cur->left)
			topn_calc(cur->left, size);
		if (size > 0)
		{
			size--;
			topn_vec_.insert(topn_vec_.end(), cur->value);
		}
		else
			return void();
		if (cur->right)
			topn_calc(cur->right, size);
	}


	int find(long long price) const
	{
		TreeStruct *direct = root;
		int idx = 0;

		while (direct != nullptr and direct->value.first != price) {
			if (direct->value.first > price)
				direct = direct->left;
			else {
				idx += (direct->left ? direct->left->count : 0) + 1;
				direct = direct->right;
			}
		}

		if (direct == nullptr)
			return -1;

		else
			return idx + (direct->left ? direct->left->count : 0);
	}


public:

	void add(long long price, long long amount) override
	{
		if (amount != 0)
		{
			TreeStruct **indirect = &root;
			std::vector<TreeStruct**> path;

			while (*indirect != nullptr) {
				path.push_back(indirect);

				if ((*indirect)->value.first > price)
					indirect = &((*indirect)->left);
				else if ((*indirect)->value.first < price)
					indirect = &((*indirect)->right);
				else
				{
					sum_amount_ = sum_amount_ - (*indirect)->value.second + amount;
					(*indirect)->value.second = amount;
					return void();
				}
			}

			sum_amount_ += amount;
			*indirect = new TreeStruct({ price, amount });
			path.push_back(indirect);

			balance(path);
			nodes_num_++;
		}
		else
		{
			sum_amount_ -= amount;
			delete_(price);
		}
	}


	void delete_(long long price) override
	{
		TreeStruct **indirect = &root;
		std::vector<TreeStruct**> path;

		while (*indirect != nullptr and (*indirect)->value.first != price) {
			path.push_back(indirect);

			if ((*indirect)->value.first > price)
				indirect = &((*indirect)->left);
			else
				indirect = &((*indirect)->right);
		}

		if (*indirect == nullptr)
			return;

		else
			path.push_back(indirect);

		std::size_t index = path.size();

		if ((*indirect)->left == nullptr and (*indirect)->right == nullptr) {
			delete *indirect;
			*indirect = nullptr;
			path.pop_back();
		}

		else if ((*indirect)->right == nullptr) {
			TreeStruct *toRemove = *indirect;

			(*indirect) = (*indirect)->left;
			delete toRemove;

			path.pop_back();
		}

		else {
			TreeStruct **successor = &((*indirect)->right);

			while ((*successor)->left != nullptr) {
				path.push_back(successor);
				successor = &((*successor)->left);
			}

			if (*successor == (*indirect)->right) {
				(*successor)->left = (*indirect)->left;

				TreeStruct *toRemove = *indirect;
				*indirect = *successor;
				delete toRemove;
			}

			else {
				TreeStruct *tmp = *path.back(), *suc = *successor;

				tmp->left = (*successor)->right;
				suc->left = (*indirect)->left;
				suc->right = (*indirect)->right;

				delete *indirect;
				*indirect = suc;
				path[index] = &(suc->right);
			}
		}

		balance(path);
		nodes_num_--;
	}


	long long get_sum_amount() override
	{
		return sum_amount_;
	}


	long long get(long long price) override
	{
		TreeStruct *direct = root;

		while (direct != nullptr && direct->value.first != price) {
			if (direct->value.first > price)
				direct = direct->left;
			else {
				direct = direct->right;
			}
		}

		if (direct == nullptr)
			return -1;

		else
			return direct->value.second;
	}


	std::pair<long long, long long> get_first() override
	{
		TreeStruct *cur = root;
		int idx = 0;
		int left = cur->left != nullptr ? cur->left->count : 0;

		while (left != idx) {
			if (left < idx) {
				idx -= left + 1;

				cur = cur->right;
				left = cur->left != nullptr ? cur->left->count : 0;
			}

			else {
				cur = cur->left;
				left = cur->left != nullptr ? cur->left->count : 0;
			}
		}

		return cur->value;
	}


	std::vector <std::pair<long long, long long>> topn(size_t n) override
	{
		topn_vec_.clear();
		topn_calc(root, n);

		return topn_vec_;
	}
};


class Tests
{
public:
	Tests(std::string file_to_open, int measurements_number, int topn_random_range)
	{
		std::cout << "Добавляем в структуру данные из файла " << file_to_open << std::endl << std::endl;

		ArrayStorage pair_struct_object;
		RealSituationTests(pair_struct_object, file_to_open, "массив");
		FunctionTesting(pair_struct_object, measurements_number, topn_random_range, "массива");

		MapStorage map_object;
		RealSituationTests(map_object, file_to_open, "std::map");
		FunctionTesting(map_object, measurements_number, topn_random_range, "std::map");

		AVLTree avl_tree_object;
		RealSituationTests(avl_tree_object, file_to_open, "AVL дерев");
		FunctionTesting(avl_tree_object, measurements_number, topn_random_range, "AVL дерева");
	}

private:
	template<typename object>
	void RealSituationTests(object &our_struct, std::string file_to_open, std::string struct_name)
	{
		clock_t start, end, operation_time;
		long long price;
		long long amount;
		std::string save_file = "c:\\DataSet\\save_struct.log";
		std::ifstream file(file_to_open);
		int iter = 1;

		if (struct_name != "std::map")
			struct_name += "а";

		start = clock();
		while (!file.eof())
		{
			file >> price;
			file >> amount;
			our_struct.add(price, amount);
			our_struct.get_first();
			our_struct.get_sum_amount();
			if (iter % 2 == 0)
			{
				our_struct.get(iter);
				if (iter % 20 == 0)
				{
					our_struct.topn(10);
				}
			}
			iter++;
		}
		end = clock();

		double seconds = (double)(end - start) / CLOCKS_PER_SEC;
		std::cout << "Тестирование работы в реальной ситуации для " << struct_name << " заняло: " << seconds << " секунд" << std::endl;
		file.close();
	}

	template<typename object>
	void FunctionTesting(object &our_struct, int measurements_number, int topn_random_range, std::string struct_name)
	{
		std::cout << "Тестирование работы функций на " << measurements_number << " запросов:" << std::endl << std::endl;
		clock_t start, end, add_result = 0, delete_result = 0, get_first_result = 0, get_sum_amount_result = 0, get_result = 0, topn_result = 0;
		topn_random_range -= 10;

		if (struct_name != "std::map")
		{
			struct_name.pop_back();
			struct_name += "е";
		}

		for (int i = 0; i < measurements_number; i++)
		{
			int first = rand() % 20000;
			int second = rand() % 1000;

			start = clock();
			our_struct.add(first, second);
			end = clock();
			add_result += end - start;

			start = clock();
			our_struct.get_sum_amount();
			end = clock();
			get_sum_amount_result += end - start;

			start = clock();
			our_struct.get(first);
			end = clock();
			get_result += end - start;

			start = clock();
			our_struct.delete_(first);
			end = clock();
			delete_result += end - start;

			start = clock();
			our_struct.topn(10 + rand() % topn_random_range);
			end = clock();
			topn_result += end - start;

			start = clock();
			our_struct.get_first();
			end = clock();
			get_first_result += end - start;
		}

		std::cout << "Среднее выполнение метода       add      в заполненном " << struct_name << " заняло: " << (double)add_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl;
		std::cout << "Среднее выполнение метода      delete    в заполненном " << struct_name << " заняло: " << (double)delete_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl;
		std::cout << "Среднее выполнение метода    get_first   в заполненном " << struct_name << " заняло: " << (double)get_first_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl;
		std::cout << "Среднее выполнение метода get_sum_amount в заполненном " << struct_name << " заняло: " << (double)get_sum_amount_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl;
		std::cout << "Среднее выполнение метода       get      в заполненном " << struct_name << " заняло: " << (double)get_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl;
		std::cout << "Среднее выполнение метода  topn(10-100)  в заполненном " << struct_name << " заняло: " << (double)topn_result / CLOCKS_PER_SEC / measurements_number << " секунд" << std::endl << std::endl << std::endl << std::endl;

	}
};



int main()
{
	setlocale(LC_ALL, "Russian");

	Tests test("c:\\DataSet\\dataset4.log", 10000, 90);
}