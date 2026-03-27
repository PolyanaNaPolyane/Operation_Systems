#include <cstring>
#include <iostream>
#include <limits>

struct date
{
	int day;
	char* month;
	int year;
};

char* copyString(const char source[])
{
	const size_t length = std::strlen(source);
	char* destination = new char[length + 1];
	strcpy_s(destination, length + 1, source);
	return destination;
}

int readPositiveNumber(const char prompt[])
{
	int value = 0;

	while (true)
	{
		std::cout << prompt;

		if (std::cin >> value && value > 0)
		{
			return value;
		}

		std::cout << "Error: enter a positive number.\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

date make_date()
{
	char monthName[20];

	date value{};

	value.day = readPositiveNumber("Day? ");
	std::cout << "Month? ";
	std::cin >> monthName;
	value.year = readPositiveNumber("Year? ");

	value.month = copyString(monthName);
	return value;
}

void print_date(const date& value)
{
	std::cout << "Day: " << value.day
			  << "\tMonth: " << value.month
			  << "\tYear: " << value.year << '\n';
}

void free_date(date& value)
{
	delete[] value.month;
	value.month = nullptr;
}

date clone_date(const date& source)
{
	date copy{};
	copy.day = source.day;
	copy.month = copyString(source.month);
	copy.year = source.year;
	return copy;
}

date* createDateArray(int size)
{
	date* values = new date[size];

	for (int index = 0; index < size; ++index)
	{
		std::cout << "\nElement " << index + 1 << ":\n";
		values[index] = make_date();
	}

	return values;
}

void printDateArray(const date values[], int size, const char title[])
{
	std::cout << '\n' << title << '\n';

	if (size == 0)
	{
		std::cout << "Array is empty.\n";
		return;
	}

	for (int index = 0; index < size; ++index)
	{
		std::cout << index + 1 << ". ";
		print_date(values[index]);
	}
}

bool isSummerMonth(const char month[])
{
	return std::strcmp(month, "June") == 0
		|| std::strcmp(month, "July") == 0
		|| std::strcmp(month, "August") == 0
		|| std::strcmp(month, "june") == 0
		|| std::strcmp(month, "july") == 0
		|| std::strcmp(month, "august") == 0
		|| std::strcmp(month, "червень") == 0
		|| std::strcmp(month, "липень") == 0
		|| std::strcmp(month, "серпень") == 0;
}

int countSummerDates(const date values[], int size)
{
	int count = 0;

	for (int index = 0; index < size; ++index)
	{
		if (isSummerMonth(values[index].month))
		{
			++count;
		}
	}

	return count;
}

date* filterSummerDates(const date values[], int size, int& filteredSize)
{
	filteredSize = countSummerDates(values, size);

	if (filteredSize == 0)
	{
		return nullptr;
	}

	date* filteredValues = new date[filteredSize];
	int filteredIndex = 0;

	for (int index = 0; index < size; ++index)
	{
		if (isSummerMonth(values[index].month))
		{
			filteredValues[filteredIndex] = clone_date(values[index]);
			++filteredIndex;
		}
	}

	return filteredValues;
}

void deleteDateArray(date*& values, int size)
{
	if (values == nullptr)
	{
		return;
	}

	for (int index = 0; index < size; ++index)
	{
		free_date(values[index]);
	}

	delete[] values;
	values = nullptr;
}

char** createStringArray(int size)
{
	char** values = new char*[size];
	char buffer[256];

	for (int index = 0; index < size; ++index)
	{
		std::cout << "Line " << index + 1 << ": ";
		std::cin >> buffer;
		values[index] = copyString(buffer);
	}

	return values;
}

void printStringArray(char* const values[], int size, const char title[])
{
	std::cout << '\n' << title << '\n';

	if (size == 0)
	{
		std::cout << "String array is empty.\n";
		return;
	}

	for (int index = 0; index < size; ++index)
	{
		std::cout << index + 1 << ". " << values[index] << '\n';
	}
}

void deleteStringArray(char**& values, int size)
{
	if (values == nullptr)
	{
		return;
	}

	for (int index = 0; index < size; ++index)
	{
		delete[] values[index];
	}

	delete[] values;
	values = nullptr;
}

bool removeStringByNumber(char**& values, int& size, int number)
{
	if (size == 0)
	{
		std::cout << "Error: cannot remove a line from an empty array.\n";
		return false;
	}

	if (number < 1 || number > size)
	{
		std::cout << "Error: line number exceeds the number of existing lines.\n";
		return false;
	}

	delete[] values[number - 1];

	char** newValues = nullptr;
	if (size - 1 > 0)
	{
		newValues = new char*[size - 1];
	}

	for (int sourceIndex = 0, targetIndex = 0; sourceIndex < size; ++sourceIndex)
	{
		if (sourceIndex == number - 1)
		{
			continue;
		}

		newValues[targetIndex] = values[sourceIndex];
		++targetIndex;
	}

	delete[] values;
	values = newValues;
	--size;
	return true;
}

void runDateTask()
{
	int size = readPositiveNumber("Enter the number of dates: ");
	date* dates = createDateArray(size);
	int filteredSize = 0;
	date* summerDates = nullptr;

	printDateArray(dates, size, "Created array of dates:");

	summerDates = filterSummerDates(dates, size, filteredSize);
	if (summerDates == nullptr)
	{
		std::cout << "\nElements with summer months were not found.\n";
	}
	else
	{
		printDateArray(summerDates, filteredSize, "Array of dates with summer months:");
	}

	deleteDateArray(summerDates, filteredSize);
	deleteDateArray(dates, size);
}

void runStringTask()
{
	int size = readPositiveNumber("\nEnter the number of lines: ");
	char** lines = createStringArray(size);
	int numberToDelete = 0;

	printStringArray(lines, size, "Created array of strings:");

	std::cout << "\nEnter the line number to delete: ";
	std::cin >> numberToDelete;
	removeStringByNumber(lines, size, numberToDelete);

	printStringArray(lines, size, "String array after processing:");

	deleteStringArray(lines, size);
}

int main()
{
	runDateTask();
	runStringTask();
	return 0;
}
