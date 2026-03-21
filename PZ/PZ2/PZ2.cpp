#include <iostream>
#include <limits>

int task_2_1();
int task_2_2();
int task_2_3();
int task_2_4();
int task_2_5();

int main()
{
	task_2_1();
	task_2_2();
	task_2_3();
	task_2_4();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	task_2_5();

	return 0;
}