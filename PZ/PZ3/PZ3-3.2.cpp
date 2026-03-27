#include <ctime>
#include <iostream>

void initRandom()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int readPositive(const char prompt[])
{
	int n;
	do
	{
		std::cout << prompt;
		std::cin >> n;
	} while (n <= 0);
	return n;
}

void fillArrayRandom(int arr[], int n, int minValue, int maxValue)
{
	for (int i = 0; i < n; ++i)
	{
		arr[i] = minValue + std::rand() % (maxValue - minValue + 1);
	}
}

void printArray(const int arr[], int n, const char title[])
{
	std::cout << title << "\n";

	if (n == 0)
	{
		std::cout << "Array is empty.\n\n";
		return;
	}

	for (int i = 0; i < n; ++i)
	{
		std::cout << arr[i] << ' ';
	}
	std::cout << "\n\n";
}

void removeEvenElements(int arr[], int& n)
{
	int writeIndex = 0;

	for (int readIndex = 0; readIndex < n; ++readIndex)
	{
		if (arr[readIndex] % 2 != 0)
		{
			arr[writeIndex] = arr[readIndex];
			++writeIndex;
		}
	}

	n = writeIndex;
}

int** allocateMatrix(int rows, int cols)
{
	int** matrix = new int*[rows];
	for (int i = 0; i < rows; ++i)
	{
		matrix[i] = new int[cols];
	}
	return matrix;
}

void freeMatrix(int** matrix, int rows)
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

void fillMatrixRandom(int** matrix, int rows, int cols, int minValue, int maxValue)
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			matrix[i][j] = minValue + std::rand() % (maxValue - minValue + 1);
		}
	}
}

void printMatrix(int** matrix, int rows, int cols, const char title[])
{
	std::cout << title << "\n";
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			std::cout << matrix[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << "\n";
}

void reverseEvenColumns(int** matrix, int rows, int cols)
{
	for (int col = 1; col < cols; col += 2)
	{
		for (int top = 0, bottom = rows - 1; top < bottom; ++top, --bottom)
		{
			int temp = matrix[top][col];
			matrix[top][col] = matrix[bottom][col];
			matrix[bottom][col] = temp;
		}
	}
}

int main()
{
	initRandom();

	int n = readPositive("Enter 1D array size (n > 0): ");
	int* arr = new int[n];

	fillArrayRandom(arr, n, -50, 50);
	printArray(arr, n, "Original 1D array:");

	removeEvenElements(arr, n);
	printArray(arr, n, "1D array after removing even elements:");

	int rows = readPositive("Enter matrix rows (m > 0): ");
	int cols = readPositive("Enter matrix cols (k > 0): ");
	int** matrix = allocateMatrix(rows, cols);

	fillMatrixRandom(matrix, rows, cols, -50, 50);
	printMatrix(matrix, rows, cols, "Original matrix:");

	reverseEvenColumns(matrix, rows, cols);
	printMatrix(matrix, rows, cols, "Matrix after reversing even columns (2, 4, 6, ...):");

	delete[] arr;
	freeMatrix(matrix, rows);
	return 0;
}
