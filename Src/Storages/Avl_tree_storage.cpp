#include "Avl_tree_storage.h"

struct AVLTree::TreeStruct {

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


void AVLTree::balance(std::vector<TreeStruct**> path)
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


void AVLTree::topn_calc(TreeStruct* cur, size_t &size)
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


int AVLTree::find(long long price) const
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


void AVLTree::add(long long price, long long amount)
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


void AVLTree::delete_(long long price)
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


long long AVLTree::get_sum_amount()
{
	return sum_amount_;
}


long long AVLTree::get(long long price)
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


std::pair<long long, long long> AVLTree::get_first()
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


std::vector <std::pair<long long, long long>> AVLTree::topn(size_t n)
{
	topn_vec_.clear();
	topn_calc(root, n);

	return topn_vec_;
}
