#include "Tests.h"
#include <time.h> 
#include <fstream>


Tests::Tests(std::string file_to_open, int measurements_number, int topn_random_range)
{
	std::cout << "��������� � ��������� ������ �� ����� " << file_to_open << std::endl << std::endl;

	ArrayStorage pair_struct_object;
	RealSituationTests(pair_struct_object, file_to_open, "������");
	FunctionTesting(pair_struct_object, measurements_number, topn_random_range, "�������");

	MapStorage map_object;
	RealSituationTests(map_object, file_to_open, "std::map");
	FunctionTesting(map_object, measurements_number, topn_random_range, "std::map");

	AVLTree avl_tree_object;
	RealSituationTests(avl_tree_object, file_to_open, "AVL ������");
	FunctionTesting(avl_tree_object, measurements_number, topn_random_range, "AVL ������");
}


template<typename object>
void Tests::RealSituationTests(object &our_struct, std::string file_to_open, std::string struct_name)
{
	clock_t start, end, operation_time;
	long long price;
	long long amount;
	std::string save_file = "c:\\DataSet\\save_struct.log";
	std::ifstream file(file_to_open);
	int iter = 1;

	if (struct_name != "std::map")
		struct_name += "�";

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
	std::cout << "������������ ������ � �������� �������� ��� " << struct_name << " ������: " << seconds << " ������" << std::endl;
	file.close();
}


template<typename object>
void Tests::FunctionTesting(object &our_struct, int measurements_number, int topn_random_range, std::string struct_name)
{
	std::cout << "������������ ������ ������� �� " << measurements_number << " ��������:" << std::endl << std::endl;
	clock_t start, end, add_result = 0, delete_result = 0, get_first_result = 0, get_sum_amount_result = 0, get_result = 0, topn_result = 0;
	topn_random_range -= 10;

	if (struct_name != "std::map")
	{
		struct_name.pop_back();
		struct_name += "�";
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

	std::cout << "������� ���������� ������       add      � ����������� " << struct_name << " ������: " << (double)add_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl;
	std::cout << "������� ���������� ������      delete    � ����������� " << struct_name << " ������: " << (double)delete_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl;
	std::cout << "������� ���������� ������    get_first   � ����������� " << struct_name << " ������: " << (double)get_first_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl;
	std::cout << "������� ���������� ������ get_sum_amount � ����������� " << struct_name << " ������: " << (double)get_sum_amount_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl;
	std::cout << "������� ���������� ������       get      � ����������� " << struct_name << " ������: " << (double)get_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl;
	std::cout << "������� ���������� ������  topn(10-100)  � ����������� " << struct_name << " ������: " << (double)topn_result / CLOCKS_PER_SEC / measurements_number << " ������" << std::endl << std::endl << std::endl << std::endl;

}