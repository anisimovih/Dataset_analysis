#pragma once
#include "./Storages/Array_storage.h"
#include "./Storages/Map_storage.h"
#include "./Storages/Avl_tree_storage.h"
#include <iostream>
#include <string>

class Tests
{
public:
	Tests(std::string file_to_open, int measurements_number, int topn_random_range);

private:
	template<typename object>
	void RealSituationTests(object &our_struct, std::string file_to_open, std::string struct_name);

	template<typename object>
	void FunctionTesting(object &our_struct, int measurements_number, int topn_random_range, std::string struct_name);
};
